#include "geometry.h"

#include <limits>
#include <utility>

Ray::Ray() : t_max(std::numeric_limits<float>::max()), time(0.f){};

Ray::Ray(Point3f const& origin, Vec3f const& dir, float t_max, float time)
    : origin(origin), dir(dir), t_max(t_max), time(time) {}

Vec3f Ray::operator()(float t) const { return origin + dir * t; }

Normal3f face_forward(Normal3f const& n, Vec3f const& v) {
    return (n.dot(v) < 0.f) ? -n : n;
}

bool intersect_p(Bounds3f const& bounds, Ray const& ray, float* hit_t0,
                 float* hit_t1) {
    float t0 = 0.f;
    float t1 = ray.t_max;
    for (int i = 0; i < 3; ++i) {
        float const inv_ray_dir = 1 / ray.dir[i];

        float t_near = (bounds.min[i] - ray.origin[i]) * inv_ray_dir;
        float t_far = (bounds.max[i] - ray.origin[i]) * inv_ray_dir;

        if (t_near > t_far) {
            std::swap(t_near, t_far);
        }

        t0 = t_near > t0 ? t_near : t0;
        t1 = t_far < t1 ? t_far : t1;

        if (t0 > t1) {
            return false;
        }
    }

    if (hit_t0) {
        *hit_t0 = t0;
    }

    if (hit_t1) {
        *hit_t1 = t1;
    }

    return true;
}
