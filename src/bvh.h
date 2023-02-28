#ifndef c3po_bvh_h
#define c3po_bvh_h

#include "aabb.h"
#include "hitable.h"

class BVHNode : public Hitable {
public:
    Hitable* left;
    Hitable* right;
    AABB box;

    BVHNode(Hitable** list, int n, double t0, double t1);

    bool hit(Ray const& r, double t_min, double t_max,
             HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& bbox) const override;
};

#endif
