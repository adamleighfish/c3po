#include "sphere.h"

#include "material.h"

#include <cmath>

Sphere::Sphere(Vec3f const& center, float radius, Material* mat_ptr)
    : center(center), radius(radius), mat_ptr(mat_ptr) {}

bool Sphere::hit(Ray const& r, float t_min, float t_max,
                 HitRecord& rec) const {
    Vec3f origin_to_center = r.origin - center;
    float a = r.dir.dot(r.dir);
    float b = origin_to_center.dot(r.dir);
    float c = origin_to_center.dot(origin_to_center) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.point = r(rec.t);
            rec.normal = (rec.point - center) / radius;
            rec.mat_ptr = mat_ptr;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.point = r(rec.t);
            rec.normal = (rec.point - center) / radius;
            rec.mat_ptr = mat_ptr;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

bool Sphere::bounding_box(float t0, float t1, AABB& bbox) const {
    bbox = AABB(center - Vec3f(radius, radius, radius),
                center + Vec3f(radius, radius, radius));
    return true;
}
