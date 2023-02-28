#include "bvh.h"

#include <algorithm>
#include <iostream>

BVHNode::BVHNode(std::vector<std::shared_ptr<Hitable>> list, double t0,
                 double t1) {

    int axis = rand_int(0, 2);
    std::sort(list.begin(), list.end(),
              [axis](std::shared_ptr<Hitable> const& a, std::shared_ptr<Hitable> const& b) {
                  AABB box_left;
                  AABB box_right;
                  if (!a->bounding_box(0, 0, box_left) ||
                      !b->bounding_box(0, 0, box_right)) {
                      std::cerr << "ERROR: could not find bounding box\n";
                      return false;
                  }

                  if (axis == 0) {
                      return box_left.min.x < box_right.min.x;
                  } else if (axis == 1) {
                      return box_left.min.y < box_right.min.y;
                  } else {
                      return box_left.min.z < box_right.min.z;
                  }
              });

    const int n = list.size();
    if (n == 1) {
        _left = _right = list[0];
    } else if (n == 2) {
        _left = list[0];
        _right = list[1];
    } else {
        _left =
            std::make_shared<BVHNode>(std::vector<std::shared_ptr<Hitable>>(
                                          list.begin(), list.begin() + (n / 2)),
                                      t0, t1);
        _right =
            std::make_shared<BVHNode>(std::vector<std::shared_ptr<Hitable>>(
                                          list.begin() + (n / 2), list.end()),
                                      t0, t1);
    }

    AABB box_left, box_right;
    if (!_left->bounding_box(t0, t1, box_left) ||
        !_right->bounding_box(t0, t1, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    _bbox = get_bounding_box(box_left, box_right);
}

bool BVHNode::hit(Ray const& r, double t_min, double t_max,
                  HitRecord& rec) const {
    if (_bbox.hit(r, t_min, t_max, rec)) {
        HitRecord left_rec, right_rec;
        bool hit_left = _left->hit(r, t_min, t_max, left_rec);
        bool hit_right = _right->hit(r, t_min, t_max, right_rec);

        if (hit_left and hit_right) {
            if (left_rec.t < right_rec.t) {
                rec = left_rec;
            } else {
                rec = right_rec;
            }
            return true;
        } else if (hit_left) {
            rec = left_rec;
            return true;
        } else if (hit_right) {
            rec = right_rec;
            return true;
        }
    }
    return false;
}

bool BVHNode::bounding_box(double t0, double t1, AABB& bbox) const {
    bbox = _bbox;
    return true;
}
