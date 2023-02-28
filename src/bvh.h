#ifndef c3po_bvh_h
#define c3po_bvh_h

#include "aabb.h"
#include "hitable.h"

#include <memory>
#include <vector>

class BVHNode : public Hitable {
public:

    BVHNode(std::vector<std::shared_ptr<Hitable>> list, double t0, double t1);

    bool hit(Ray const& r, double t_min, double t_max,
             HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& bbox) const override;

private:
    std::shared_ptr<Hitable> _left;
    std::shared_ptr<Hitable> _right;

    AABB _bbox;
};

#endif
