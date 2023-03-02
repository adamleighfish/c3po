#include "hitable.h"

#include <numbers>

void get_sphere_uv(Imath::V3f const& point, float& u, float& v) {
    double phi = atan2(point.z, point.x);
    double theta = asin(point.y);
    u = 1 - (phi + std::numbers::pi) / (2 * std::numbers::pi);
    v = (theta + (std::numbers::pi / 2)) / std::numbers::pi;
}

Hitable::~Hitable() = default;

FlippedNormals::FlippedNormals(std::shared_ptr<Hitable> ptr) : ptr(ptr) {}

bool FlippedNormals::hit(Ray const& r, float t_min, float t_max,
                         HitRecord& rec) const {
    if (ptr->hit(r, t_min, t_max, rec)) {
        rec.normal = -rec.normal;
        return true;
    }
    return false;
}

bool FlippedNormals::bounding_box(float t0, float t1, AABB& bbox) const {
    return ptr->bounding_box(t0, t1, bbox);
}
