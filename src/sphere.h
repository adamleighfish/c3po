#ifndef sphere_h
#define sphere_h

#include <cmath>

#include "hitable.h"

class Sphere : public Hitable {
public:
    Vec3f Center;
    double radius;
    Material* mat_ptr;

    Sphere() {}
    Sphere(Vec3f const& Center, double radius, Material* mat_ptr)
        : Center(Center), radius(radius), mat_ptr(mat_ptr) {}

    virtual bool Hit(Ray const& R, double t_min, double t_max,
                     HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

bool Sphere::Hit(Ray const& R, double t_min, double t_max,
                 HitRecord& rec) const {
    Vec3f OC = R.O - Center;
    double a = Dot(R.D, R.D);
    double b = Dot(OC, R.D);
    double c = Dot(OC, OC) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.P = R(rec.t);
            rec.Normal = (rec.P - Center) / radius;
            rec.mat_ptr = mat_ptr;
            SetSphereUV(rec.Normal, rec.u, rec.v);
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.P = R(rec.t);
            rec.Normal = (rec.P - Center) / radius;
            rec.mat_ptr = mat_ptr;
            SetSphereUV(rec.Normal, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

bool Sphere::BoundingBox(double t0, double t1, AABB& box) const {
    box = AABB(Center - Vec3f(radius, radius, radius),
               Center + Vec3f(radius, radius, radius));
    return true;
}

#endif
