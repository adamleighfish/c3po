#ifndef c3po_box_h
#define c3po_box_h

#include "hitable.h"

#include <memory>

class Box : public Hitable {
public:
    Box(Vec3 const& p0, Vec3 const& p1, std::shared_ptr<Material> mat);

    bool hit(Ray const& r, double t_min, double t_max,
             HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& bbox) const override;

private:
    const Vec3 p_min;
    const Vec3 p_max;

    std::shared_ptr<Hitable> bvh;
};

#endif
