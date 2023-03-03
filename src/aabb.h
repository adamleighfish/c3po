#ifndef c3po_aabb_h
#define c3po_aabb_h

#include "geometry.h"

struct HitRecord;

class AABB {
public:
    Vec3f min, max;

    AABB() = default;
    explicit AABB(Vec3f const& min, Vec3f const& max);

    bool hit(Ray const& r, float t_min, float t_max, HitRecord& rec) const;
};

AABB get_bounding_box(const AABB& box_a, const AABB& box_b);

#endif
