#ifndef bvh_h
#define bvh_h

#include <stdlib.h>

#include <iostream>
#include <random>

#include "aabb.h"
#include "hitable.h"

int BoxCompareX(void const* a, void const* b) {
    AABB box_left, box_right;
    Hitable* a_hitable = *(Hitable**)a;
    Hitable* b_hitable = *(Hitable**)b;
    if (!a_hitable->BoundingBox(0, 0, box_left) or
        !b_hitable->BoundingBox(0, 0, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    if (box_left.min.x - box_right.min.x < 0.0) {
        return -1;
    }
    return 1;
}

int BoxCompareY(void const* a, void const* b) {
    AABB box_left, box_right;
    Hitable* a_hitable = *(Hitable**)a;
    Hitable* b_hitable = *(Hitable**)b;
    if (!a_hitable->BoundingBox(0, 0, box_left) or
        !b_hitable->BoundingBox(0, 0, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    if (box_left.min.y - box_right.min.y < 0.0) {
        return -1;
    }
    return 1;
}

int BoxCompareZ(void const* a, void const* b) {
    AABB box_left, box_right;
    Hitable* a_hitable = *(Hitable**)a;
    Hitable* b_hitable = *(Hitable**)b;
    if (!a_hitable->BoundingBox(0, 0, box_left) or
        !b_hitable->BoundingBox(0, 0, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    if (box_left.min.z - box_right.min.z < 0.0) {
        return -1;
    }
    return 1;
}

class BVHNode : public Hitable {
public:
    Hitable* left;
    Hitable* right;
    AABB box;

    BVHNode() {}
    BVHNode(Hitable** list, int n, double t0, double t1);

    virtual bool Hit(Ray const& R, double t_min, double t_max,
                     HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

BVHNode::BVHNode(Hitable** list, int n, double t0, double t1) {
    int axis = rand_int(0, 2);

    if (axis == 0) {
        qsort(list, n, sizeof(Hitable*), BoxCompareX);
    } else if (axis == 1) {
        qsort(list, n, sizeof(Hitable*), BoxCompareY);
    } else if (axis == 2) {
        qsort(list, n, sizeof(Hitable*), BoxCompareZ);
    }

    if (n == 1) {
        left = right = list[0];
    } else if (n == 2) {
        left = list[0];
        right = list[1];
    } else {
        left = new BVHNode(list, n / 2, t0, t1);
        right = new BVHNode(list + n / 2, n - n / 2, t0, t1);
    }

    AABB box_left, box_right;
    if (!left->BoundingBox(t0, t1, box_left) or
        !right->BoundingBox(t0, t1, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    box = get_bounding_box(box_left, box_right);
}

bool BVHNode::Hit(Ray const& R, double t_min, double t_max,
                  HitRecord& rec) const {
    if (box.hit(R, t_min, t_max)) {
        HitRecord left_rec, right_rec;
        bool hit_left = left->Hit(R, t_min, t_max, left_rec);
        bool hit_right = right->Hit(R, t_min, t_max, right_rec);

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

bool BVHNode::BoundingBox(double t0, double t1, AABB& b) const {
    b = box;
    return true;
}

#endif
