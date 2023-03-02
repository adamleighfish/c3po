#ifndef c3po_sphere_h
#define c3po_sphere_h

#include "geometry.h"
#include "hitable.h"

class Material;

class Sphere : public Hitable {
public:
    Sphere(Vec3 const& center, float radius, Material* mat_ptr);

    bool hit(Ray const& r, float t_min, float t_max,
             HitRecord& rec) const override;
    bool bounding_box(float t0, float t1, AABB& box) const override;

private:
    Vec3 center;
    float radius;
    Material* mat_ptr;
};

#endif
