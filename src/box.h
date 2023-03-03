#ifndef c3po_box_h
#define c3po_box_h

#include "hitable.h"

#include <memory>

class Box : public Hitable {
public:
    Box(Vec3f const& p0, Vec3f const& p1, std::shared_ptr<Material> mat);

    bool hit(Ray const& r, float t_min, float t_max,
             HitRecord& rec) const override;
    bool bounding_box(float t0, float t1, AABB& bbox) const override;

private:
    const Vec3f p_min;
    const Vec3f p_max;

    std::shared_ptr<Hitable> bvh;
};

#endif
