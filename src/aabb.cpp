#include "aabb.h"

AABB::AABB(Vec3f const& min, Vec3f const& max) : min(min), max(max) {}

bool AABB::hit(Ray const& r, double t_min, double t_max) const {
    for (int axis = 0; axis < 3; ++axis) {
        const double invD = 1.0 / r.D[axis];
        double t0 = (min[axis] - r.O[axis]) * invD;
        double t1 = (max[axis] - r.O[axis]) * invD;
        if (invD < 0.0) {
            std::swap(t0, t1);
        }
        t_min = std::max(t0, t_min);
        t_max = std::min(t1, t_max);
        if (t_max <= t_min) {
            return false;
        }
    }
    return true;
}

AABB get_bounding_box(const AABB& box_a, const AABB& box_b) {
    const Vec3f small(std::min(box_a.min.x, box_b.min.x),
                      std::min(box_a.min.y, box_b.min.y),
                      std::min(box_a.min.z, box_b.min.z));
    const Vec3f large(std::max(box_a.max.x, box_b.max.x),
                      std::max(box_a.max.y, box_b.max.y),
                      std::max(box_a.max.z, box_b.max.z));
    return AABB(small, large);
}
