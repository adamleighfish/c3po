//
//  hitable.h
//  toytracer
//
//  Created by Adam Fish on 11/4/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef hitable_h
#define hitable_h

#include "geometry.h"
#include "aabb.h"

class Material;

void SetSphereUV(const Vec3f& P, double& u, double& v) {
    double phi = atan2(P.z, P.x);
    double theta = asin(P.y);
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI_2) / M_PI;
}

struct HitRecord {
    double t;
    double u;
    double v;
    Vec3f P;
    Vec3f Normal;
    Material* mat_ptr;
};

class Hitable {
public:
    virtual bool Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const = 0;
};

class FlippedNormals: public Hitable {
public:
    Hitable* ptr;
    
    FlippedNormals(Hitable* ptr): ptr(ptr) {}
    
    virtual bool Hit(const Ray& R, double t_min, double t_max, HitRecord& rec) const {
        if (ptr->Hit(R, t_min, t_max, rec)) {
            rec.Normal = -rec.Normal;
            return true;
        }
        return false;
    }
    
    virtual bool BoundingBox(double t0, double t1, AABB& box) const {
        return ptr->BoundingBox(t0, t1, box);
    }
};

#endif /* hitable_h */
