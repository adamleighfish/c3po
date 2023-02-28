#ifndef c3po_sphere_h
#define c3po_sphere_h

#include "geometry.h"
#include "hitable.h"

class Material;

class Sphere : public Hitable {
public:
    Sphere(Vec3 const& center, double radius, Material* mat_ptr);

    bool hit(Ray const& r, double t_min, double t_max,
             HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& box) const override;

private:
    Vec3 center;
    double radius;
    Material* mat_ptr;
};

#endif
