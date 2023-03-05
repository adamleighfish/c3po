#include "bvh.h"

#include "geometry.h"

#include <algorithm>
#include <iomanip>

namespace c3po {

struct BucketInfo {
    int count;
    Bounds3f bounds;

    BucketInfo() : count(0), bounds() {}
};

struct BuildNode {
    Bounds3f bounds;
    std::unique_ptr<BuildNode> children[2];
    int split_axis;
    int first_prim_offset;
    int n_prims;

    void init_leaf(int first, int n, Bounds3f const& b) {
        bounds = b;
        children[0] = nullptr;
        children[1] = nullptr;
        first_prim_offset = first;
        n_prims = n;
    }

    void init_interior(int axis, std::unique_ptr<BuildNode> c0,
                       std::unique_ptr<BuildNode> c1) {
        bounds = c0->bounds;
        bounds.extendBy(c1->bounds);
        children[0] = std::move(c0);
        children[1] = std::move(c1);
        split_axis = axis;
        n_prims = 0;
    }
};

struct PrimInfo {
    size_t index;
    Bounds3f bounds;
    Point3f centroid;

    PrimInfo() = delete;
    PrimInfo(size_t index, Bounds3f const& bounds)
        : index(index), bounds(bounds),
          centroid(.5f * bounds.min + .5f * bounds.max) {}
};

BVH::BVH(std::vector<std::shared_ptr<Primitive>> p, int max_prims_in_node)
    : prims(std::move(p)), max_prims_in_node(std::min(max_prims_in_node, 255)) {

    if (prims.empty()) {
        return;
    }

    // initialize the prim infos with bounds and center information
    std::vector<PrimInfo> prim_infos;
    prim_infos.reserve(prims.size());
    for (size_t i = 0; i < prims.size(); ++i) {
        prim_infos.push_back({
            i,
            prims[i]->world_bounds(),
        });
    }

    // recursively build the bvh, keeping track of the number of nodes
    int total_nodes = 0;
    std::vector<std::shared_ptr<Primitive>> ordered_prims;
    ordered_prims.reserve(prims.size());
    std::unique_ptr<BuildNode> root = recursive_build(
        prim_infos, 0, prims.size(), total_nodes, ordered_prims);
    prims.swap(ordered_prims);

    std::cout << std::fixed << std::setprecision(2) << "BVH created with "
              << total_nodes << " nodes for " << prims.size() << " primitives ("
              << float(total_nodes) * sizeof(LinearNode) / (1024.f * 1024.f)
              << " MB)" << std::endl;

    nodes.reserve(total_nodes);
    for (int i = 0; i < total_nodes; ++i) {
        nodes.push_back(std::make_unique<LinearNode>());
    }

    // flatten the tree by doing a depth first traversal and store that
    // representation for faster intersection testing, it is much more memory
    // cache friendly
    int offset = 0;
    flatten_tree(root, offset);
}

Bounds3f BVH::world_bounds() const {
    return nodes.empty() ? Bounds3f() : nodes[0]->bounds;
}

bool BVH::intersect(Ray const& r, SurfaceInteraction& isect) const {
    bool hit = false;
    Vec3f const inv_dir(1.f / r.dir.x, 1.f / r.dir.y, 1.f / r.dir.z);
    int const dir_is_neg[3] = {inv_dir.x < 0.f, inv_dir.y < 0.f,
                               inv_dir.z < 0.f};

    int to_visit_offset = 0;
    int current_node_index = 0;
    int nodes_to_visit[64];
    while (true) {
        LinearNode const* node = nodes[current_node_index].get();
        if (intersects_p(node->bounds, r, inv_dir, dir_is_neg)) {
            if (node->n_prims > 0) {
                for (int i = 0; i < node->n_prims; ++i) {
                    if (prims[node->prims_offset + i]->intersect(r, isect)) {
                        hit = true;
                    }
                }

                if (to_visit_offset == 0) {
                    break;
                }
                current_node_index = nodes_to_visit[--to_visit_offset];
            } else {
                if (dir_is_neg[node->axis]) {
                    nodes_to_visit[to_visit_offset++] = current_node_index + 1;
                    current_node_index = node->second_child_offet;
                } else {
                    nodes_to_visit[to_visit_offset++] =
                        node->second_child_offet;
                    current_node_index = current_node_index + 1;
                }
            }
        } else {
            if (to_visit_offset == 0) {
                break;
            }
            current_node_index = nodes_to_visit[--to_visit_offset];
        }
    }

    return hit;
}

bool BVH::intersects(Ray const& r) const {
    Vec3f const inv_dir(1.f / r.dir.x, 1.f / r.dir.y, 1.f / r.dir.z);
    int const dir_is_neg[3] = {inv_dir.x < 0.f, inv_dir.y < 0.f,
                               inv_dir.z < 0.f};

    int to_visit_offset = 0;
    int current_node_index = 0;
    std::vector<int> nodes_to_visit;
    nodes_to_visit.reserve(64);

    while (true) {
        LinearNode const* node = nodes[current_node_index].get();
        if (intersects_p(node->bounds, r, inv_dir, dir_is_neg)) {
            if (node->n_prims > 0) {
                // at a leaf node so check against the prims
                for (int i = 0; i < node->n_prims; ++i) {
                    if (prims[node->prims_offset + i]->intersects(r)) {
                        return true;
                    }
                }
                if (to_visit_offset == 0) {
                    break;
                }
                current_node_index = nodes_to_visit[--to_visit_offset];

            } else {
                //
                if (dir_is_neg[node->axis]) {
                    nodes_to_visit[to_visit_offset++] = current_node_index + 1;
                    current_node_index = node->second_child_offet;
                } else {
                    nodes_to_visit[to_visit_offset++] =
                        node->second_child_offet;
                    current_node_index = current_node_index + 1;
                }
            }

        } else {
            // no hit so go to the next node available if one exists
            if (to_visit_offset == 0) {
                break;
            }
            current_node_index = nodes_to_visit[--to_visit_offset];
        }
    }

    return false;
}

std::unique_ptr<BuildNode>
BVH::recursive_build(std::vector<PrimInfo>& prim_infos, int start, int end,
                     int& total_nodes,
                     std::vector<std::shared_ptr<Primitive>>& ordered_prims) {

    std::unique_ptr<BuildNode> node = std::make_unique<BuildNode>();
    total_nodes++;

    // compute the total bounds for this node
    Bounds3f bounds;
    for (int i = start; i < end; ++i) {
        bounds.extendBy(prim_infos[i].bounds);
    }

    // if we just have a single node we can make a leaf and return
    int const n_prims = end - start;
    if (n_prims == 1) {
        // init the leaf node for this prim and return
        int const prim_offset = ordered_prims.size();
        int const prim_num = prim_infos[start].index;
        ordered_prims.push_back(prims[prim_num]);
        node->init_leaf(prim_offset, n_prims, bounds);
        return std::move(node);
    }

    // compute bounds of the prim centroids and use the dimension with the
    // longest extent for the split
    Bounds3f centroid_bounds;
    for (int i = start; i < end; ++i) {
        centroid_bounds.extendBy(prim_infos[i].centroid);
    }

    // multiple prims are on the same point in space so pack them all in one
    // node
    int const dim = centroid_bounds.majorAxis();
    if (centroid_bounds.max[dim] == centroid_bounds.min[dim]) {
        int const prim_offset = ordered_prims.size();
        for (int i = start; i < end; ++i) {
            int const prim_num = prim_infos[i].index;
            ordered_prims.push_back(prims[prim_num]);
        }
        node->init_leaf(prim_offset, n_prims, bounds);
        return std::move(node);
    }

    // partition the prims into two sets using a surface area heuristic
    int mid = (start + end) / 2;
    if (n_prims <= 2) {
        // partition into equally sized subsets
        std::nth_element(&prim_infos[start], &prim_infos[mid],
                         &prim_infos[end - 1] + 1,
                         [dim](PrimInfo const& a, PrimInfo const& b) {
                             return a.centroid[dim] < b.centroid[dim];
                         });
    } else {
        // initial the buckets for the approximate SAH
        int constexpr N_BUCKETS = 12;
        BucketInfo buckets[N_BUCKETS];

        // compute the number of prims in each bucket and its bounding box
        for (int i = start; i < end; ++i) {
            int b = N_BUCKETS *
                    offset(centroid_bounds, prim_infos[i].centroid)[dim];
            if (b == N_BUCKETS) {
                b = N_BUCKETS - 1;
            }
            buckets[b].count++;
            buckets[b].bounds.extendBy(prim_infos[i].bounds);
        }

        // compute the cost for spliting at each bucket (cost of a split is 1
        // and the cost of a traversal is 1/8)
        //
        // note, the cost of splitting after the last bucket is not considered
        // because it does not split the set at all
        float cost[N_BUCKETS - 1];
        for (int i = 0; i < N_BUCKETS - 1; ++i) {
            Bounds3f b0;
            int count0 = 0;
            for (int j = 0; j <= i; ++j) {
                b0.extendBy(buckets[j].bounds);
                count0 += buckets[j].count;
            }
            Bounds3f b1;
            int count1 = 0;
            for (int j = i + 1; j < N_BUCKETS; ++j) {
                b1.extendBy(buckets[j].bounds);
                count1 += buckets[j].count;
            }

            cost[i] = .125f +
                      (count0 * surface_area(b0) + count1 * surface_area(b1)) /
                          surface_area(bounds);
        }

        // find the bucket with the minimal cost
        float min_cost = cost[0];
        int min_cost_split = 0;
        for (int i = 1; i < N_BUCKETS - 1; ++i) {
            if (cost[i] < min_cost) {
                min_cost = cost[i];
                min_cost_split = i;
            }
        }

        // either create leaf nodes or split at the bucket, which ever is
        // cheaper
        float const leaf_cost = float(n_prims);
        if (n_prims > max_prims_in_node || min_cost < leaf_cost) {
            PrimInfo* p_mid =
                std::partition(&prim_infos[start], &prim_infos[end - 1] + 1,
                               [=](PrimInfo const& pi) {
                                   int b = N_BUCKETS * offset(centroid_bounds,
                                                              pi.centroid)[dim];
                                   if (b == N_BUCKETS) {
                                       b = N_BUCKETS - 1;
                                   }
                                   return b <= min_cost_split;
                               });
            mid = p_mid - &prim_infos[0];

        } else {
            int const prim_offset = ordered_prims.size();
            for (int i = start; i < end; ++i) {
                int const prim_num = prim_infos[i].index;
                ordered_prims.push_back(prims[prim_num]);
            }
            node->init_leaf(prim_offset, n_prims, bounds);
            return std::move(node);
        }
    }

    node->init_interior(
        dim,
        recursive_build(prim_infos, start, mid, total_nodes, ordered_prims),
        recursive_build(prim_infos, mid, end, total_nodes, ordered_prims));
    return std::move(node);
}

int BVH::flatten_tree(std::unique_ptr<BuildNode> const& node, int& offset) {
    LinearNode* linear_node = nodes[offset].get();
    linear_node->bounds = node->bounds;
    int const my_offset = offset++;
    if (node->n_prims > 0) {
        linear_node->prims_offset = node->first_prim_offset;
        linear_node->n_prims = node->n_prims;
    } else {
        linear_node->axis = node->split_axis;
        linear_node->n_prims = 0;
        flatten_tree(node->children[0], offset);
        linear_node->second_child_offet =
            flatten_tree(node->children[1], offset);
    }

    return my_offset;
}

} // namespace c3po
