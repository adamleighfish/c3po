#include "geometry.h"

#include <limits>
#include <utility>

namespace c3po {

Vec3f cross(Vec3f const& a, Vec3f const& b) { return b.cross(a); }

float distance(Point3f const& a, Point3f const& b) {
    return std::sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) +
                      (a.z - b.z) * (a.z - b.z));
}

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

bool intersects_p(Bounds3f const& bounds, Ray const& ray, Vec3f const& inv_dir,
                  int const dir_is_neg[3]) {
    float t_min =
        ((dir_is_neg[0] ? bounds.max.x : bounds.min.x) - ray.origin.x) *
        inv_dir.x;
    float t_max =
        ((dir_is_neg[0] ? bounds.min.x : bounds.max.x) - ray.origin.x) *
        inv_dir.x;
    float const ty_min =
        ((dir_is_neg[1] ? bounds.max.y : bounds.min.y) - ray.origin.y) *
        inv_dir.y;
    float const ty_max =
        ((dir_is_neg[1] ? bounds.min.y : bounds.max.y) - ray.origin.y) *
        inv_dir.y;

    if (t_min > ty_max || ty_min > t_max) {
        return false;
    }
    t_min = std::min(t_min, ty_min);
    t_max = std::max(t_max, ty_max);

    float const tz_min =
        ((dir_is_neg[2] ? bounds.max.z : bounds.min.z) - ray.origin.z) *
        inv_dir.z;
    float const tz_max =
        ((dir_is_neg[2] ? bounds.min.z : bounds.max.z) - ray.origin.z) *
        inv_dir.z;

    if (t_min > tz_max || tz_min > t_max) {
        return false;
    }

    t_min = std::min(t_min, tz_max);
    t_max = std::max(t_max, tz_max);

    return (t_min < ray.t_max) && (t_max > 0.f);
}

float surface_area(Bounds3f const& b) {
    const Vec3f d = b.max - b.min;
    return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

Vec3f offset(Bounds3f const& b, Point3f const& p) {
    Vec3f o = p - b.min;
    if (b.max.x > b.min.x) {
        o.x /= b.max.x - b.min.x;
    }
    if (b.max.y > b.min.y) {
        o.y /= b.max.y - b.min.y;
    }
    if (b.max.z > b.min.z) {
        o.z /= b.max.z - b.min.z;
    }

    return o;
}

} // namespace c3po
