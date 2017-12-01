//
//  aabb.h
//  toytracer
//
//  Created by Adam Fish on 11/6/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef aabb_h
#define aabb_h

#include "geometry.h"

class AABB {
public:
    Vec3f Min, Max;
    
    AABB() {}
    AABB(const Vec3f& Min, const Vec3f& Max): Min(Min), Max(Max) {}
    
    bool Hit(const Ray& R, double tmin, double tmax) const {
        for (int a = 0; a < 3; ++a) {
            double invD = 1.0 / R.D[a];
            double t0 = (Min[a] - R.O[a]) * invD;
            double t1 = (Max[a] - R.O[a]) * invD;
            if (invD < 0.0) {
                std::swap(t0, t1);
            }
            tmin = t0 > tmin ? t0: tmin;
            tmax = t1 < tmax ? t1: tmax;
            if (tmax <= tmin) {
                return false;
            }
        }
        return true;
    }
};

AABB SurroundingBox(const AABB& box0, const AABB& box1) {
    Vec3f Small(std::min(box0.Min.x, box1.Min.x),
                std::min(box0.Min.y, box1.Min.y),
                std::min(box0.Min.z, box1.Min.z));
    Vec3f Big(std::max(box0.Max.x, box1.Max.x),
              std::max(box0.Max.y, box1.Max.y),
              std::max(box0.Max.z, box1.Max.z));
    return AABB(Small, Big);
}

#endif /* aabb_h */
