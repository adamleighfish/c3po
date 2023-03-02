#ifndef c3po_bvh_h
#define c3po_bvh_h

#include "aabb.h"
#include "hitable.h"

#include <memory>
#include <vector>

class BVHNode : public Hitable {
public:

    BVHNode(std::vector<std::shared_ptr<Hitable>> list, float t0, float t1);

    bool hit(Ray const& r, float t_min, float t_max,
             HitRecord& rec) const override;
    bool bounding_box(float t0, float t1, AABB& bbox) const override;

private:
    std::shared_ptr<Hitable> _left;
    std::shared_ptr<Hitable> _right;

    AABB _bbox;
};

#endif
