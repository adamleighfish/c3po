#include "hitable.h"

#include <numbers>

void get_sphere_uv(Vec3 const& point, double& u, double& v) {
    double phi = atan2(point.z, point.x);
    double theta = asin(point.y);
    u = 1 - (phi + std::numbers::pi) / (2 * std::numbers::pi);
    v = (theta + (std::numbers::pi / 2)) / std::numbers::pi;
}

Hitable::~Hitable() = default;

FlippedNormals::FlippedNormals(std::shared_ptr<Hitable> ptr) : ptr(ptr) {}

bool FlippedNormals::hit(Ray const& r, double t_min, double t_max,
                         HitRecord& rec) const {
    if (ptr->hit(r, t_min, t_max, rec)) {
        rec.normal = -rec.normal;
        return true;
    }
    return false;
}

bool FlippedNormals::bounding_box(double t0, double t1, AABB& bbox) const {
    return ptr->bounding_box(t0, t1, bbox);
}
