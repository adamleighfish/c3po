//
//  bvh.h
//  c3p0
//
//  Created by Adam Fish on 11/6/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef bvh_h
#define bvh_h

#include <iostream>
#include <stdlib.h>

#include "hitable.h"
#include "aabb.h"

int BoxCompareX(const void* a, const void* b) {
    AABB box_left, box_right;
    Hitable* a_hitable = *(Hitable**)a;
    Hitable* b_hitable = *(Hitable**)b;
    if (!a_hitable->BoundingBox(0, 0, box_left) or !b_hitable->BoundingBox(0, 0, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    if (box_left.Min.x - box_right.Min.x < 0.0) {
        return -1;
    }
    return 1;
}

int BoxCompareY(const void* a, const void* b) {
    AABB box_left, box_right;
    Hitable* a_hitable = *(Hitable**)a;
    Hitable* b_hitable = *(Hitable**)b;
    if (!a_hitable->BoundingBox(0, 0, box_left) or !b_hitable->BoundingBox(0, 0, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    if (box_left.Min.y - box_right.Min.y < 0.0) {
        return -1;
    }
    return 1;
}

int BoxCompareZ(const void* a, const void* b) {
    AABB box_left, box_right;
    Hitable* a_hitable = *(Hitable**)a;
    Hitable* b_hitable = *(Hitable**)b;
    if (!a_hitable->BoundingBox(0, 0, box_left) or !b_hitable->BoundingBox(0, 0, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    if (box_left.Min.z - box_right.Min.z < 0.0) {
        return -1;
    }
    return 1;
}

class BVHNode: public Hitable {
public:
    Hitable* left;
    Hitable* right;
    AABB box;
    
    BVHNode() {}
    BVHNode(Hitable** list, int n, double t0, double t1);
    
    virtual bool Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

BVHNode::BVHNode(Hitable** list, int n, double t0, double t1) {
    int axis = int(drand48() * 3);
    
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
        left = new BVHNode(list, n/2, t0, t1);
        right = new BVHNode(list + n/2, n - n/2, t0, t1);
    }
    
    AABB box_left, box_right;
    if (!left->BoundingBox(t0, t1, box_left) or !right->BoundingBox(t0, t1, box_right)) {
        std::cerr << "ERROR: could not find bounding box\n";
    }
    box = SurroundingBox(box_left, box_right);
}

bool BVHNode::Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const {
    if (box.Hit(R, t_min, t_max)) {
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

#endif /* bvh_h */
