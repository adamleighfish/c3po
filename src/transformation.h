//
//  transformation.h
//  c3p0
//
//  Created by Adam Fish on 11/9/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef transformation_h
#define transformation_h

#include <limits>

#include "hitable.h"

/**
 *
 *
 */

class Translate: public Hitable {
public:
    Hitable* ptr;
    Vec3f Offset;
    
    Translate(Hitable* ptr, const Vec3f& Offset): ptr(ptr), Offset(Offset) {}
    
    virtual bool Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

bool Translate::Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const {
    Ray Moved(R.O - Offset, R.D, R.time);
    if (ptr->Hit(Moved, t_min, t_max, rec)) {
        rec.P += Offset;
        return true;
    }
    return false;
}

bool Translate::BoundingBox(double t0, double t1, AABB& box) const {
    if (ptr->BoundingBox(t0, t1, box)) {
        box = AABB(box.Min + Offset, box.Max + Offset);
        return true;
    }
    return false;
}

/**
 *
 *
 */

class RotateY: public Hitable {
public:
    Hitable* ptr;
    double sin_theta;
    double cos_theta;
    bool has_box;
    AABB bbox;
    
    RotateY(Hitable* ptr, double angle);
    
    virtual bool Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

RotateY::RotateY(Hitable* ptr, double angle) : ptr(ptr) {
    double radians = (M_PI / 180.0) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    has_box = ptr->BoundingBox(0, 1, bbox);
    Vec3f Min(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    Vec3f Max(-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max(), -std::numeric_limits<double>::max());
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                double x = i * bbox.Max.x + (1 - i) * bbox.Min.x;
                double y = j * bbox.Max.y + (1 - j) * bbox.Min.y;
                double z = k * bbox.Max.z + (1 - k) * bbox.Min.z;
                double new_x = cos_theta * x + sin_theta * z;
                double new_z = cos_theta * z - sin_theta * x;
                
                Vec3f Tester(new_x, y, new_z);
                for (int c = 0; c < 3; ++c) {
                    if (Tester[c] > Max[c]) {
                        Max.SetValue(c, Tester[c]);
                    }
                    if (Tester[c] < Min[c]) {
                        Min.SetValue(c, Tester[c]);
                    }
                }
            }
        }
    }
    bbox = AABB(Min, Max);
}

bool RotateY::Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const {
    Vec3f Origin = R.O;
    Vec3f Direction = R.D;
    Origin.x = cos_theta * R.O.x - sin_theta * R.O.z;
    Origin.z = sin_theta * R.O.x + cos_theta * R.O.z;
    Direction.x = cos_theta * R.D.x - sin_theta * R.D.z;
    Direction.z = sin_theta * R.D.x + cos_theta * R.D.z;
    
    Ray Rotated(Origin, Direction, R.time);
    if (ptr->Hit(Rotated, t_min, t_max, rec)) {
        Vec3f P = rec.P;
        Vec3f Normal = rec.Normal;
        P.x = cos_theta * rec.P.x + sin_theta * rec.P.z;
        P.z = -sin_theta * rec.P.x + cos_theta * rec.P.z;
        Normal.x = cos_theta * rec.Normal.x + sin_theta * rec.Normal.z;
        Normal.z = -sin_theta * rec.Normal.x + cos_theta * rec.Normal.z;
        rec.P = P;
        rec.Normal = Normal;
        return true;
    }
    return false;
}

bool RotateY::BoundingBox(double t0, double t1, AABB& box) const {
    box = bbox;
    return has_box;
}

#endif /* transformation_h */
