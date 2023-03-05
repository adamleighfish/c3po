#ifndef C3PO_BVH_H
#define C3PO_BVH_H

#include "primitive.h"

#include <memory>
#include <vector>

namespace c3po {

struct BuildNode;
struct PrimInfo;

struct LinearNode {
    Bounds3f bounds;
    union {
        int prims_offset;       // leaf
        int second_child_offet; // interior
    };
    uint16_t n_prims; // 0 -> interior node
    uint8_t axis;     // interior node: xyz
    uint8_t pad[1];   // ensure 32 bytes total size
};

class BVH : public Aggregate {
public:
    BVH(std::vector<std::shared_ptr<Primitive>> prims, int max_prims_in_node);

    Bounds3f world_bounds() const override;

    bool intersect(Ray const& r, SurfaceInteraction& isect) const override;

    bool intersects(Ray const& r) const override;

private:
    std::unique_ptr<BuildNode>
    recursive_build(std::vector<PrimInfo>& prim_infos, int start, int end,
                    int& total_nodes,
                    std::vector<std::shared_ptr<Primitive>>& ordered_prims);

    int flatten_tree(std::unique_ptr<BuildNode> const& node, int& offset);

    std::vector<std::shared_ptr<Primitive>> prims;
    int const max_prims_in_node;
    std::vector<std::unique_ptr<LinearNode>> nodes;
};

} // namespace c3po

#endif
