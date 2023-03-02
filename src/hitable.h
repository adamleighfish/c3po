#ifndef c3po_hitable_h
#define c3po_hitable_h

#include "aabb.h"
#include "geometry.h"

#include <memory>

class Material;

void get_sphere_uv(Imath::V3f const& point, float& u, float& v);

struct HitRecord {
    float t;
    float u;
    float v;
    Imath::V3f point;
    Imath::V3f normal;
    Material* mat_ptr;
};

class Hitable {
public:
    virtual ~Hitable();

    virtual bool hit(Ray const& r, float t_min, float t_max,
                     HitRecord& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, AABB& bbox) const = 0;
};

class FlippedNormals : public Hitable {
public:
    FlippedNormals(std::shared_ptr<Hitable> ptr);

    bool hit(Ray const& r, float t_min, float t_max,
             HitRecord& rec) const override;

    bool bounding_box(float t0, float t1, AABB& box) const override;

private:
    std::shared_ptr<Hitable> ptr;
};

#endif
