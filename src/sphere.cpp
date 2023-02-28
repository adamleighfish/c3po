#include "sphere.h"

#include "material.h"

#include <cmath>

Sphere::Sphere(Vec3 const& center, double radius, Material* mat_ptr)
    : center(center), radius(radius), mat_ptr(mat_ptr) {}

bool Sphere::hit(Ray const& r, double t_min, double t_max,
                 HitRecord& rec) const {
    Vec3 origin_to_center = r.origin - center;
    double a = dot(r.dir, r.dir);
    double b = dot(origin_to_center, r.dir);
    double c = dot(origin_to_center, origin_to_center) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant)) / a;
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

bool Sphere::bounding_box(double t0, double t1, AABB& bbox) const {
    bbox = AABB(center - Vec3(radius, radius, radius),
                center + Vec3(radius, radius, radius));
    return true;
}
