#ifndef c3po_hitable_h
#define c3po_hitable_h

#include "aabb.h"
#include "geometry.h"

#include <memory>

class Material;

void get_sphere_uv(Vec3 const& point, double& u, double& v);

struct HitRecord {
    double t;
    double u;
    double v;
    Vec3 point;
    Vec3 normal;
    Material* mat_ptr;
};

class Hitable {
public:
    virtual ~Hitable();

    virtual bool hit(Ray const& r, double t_min, double t_max,
                     HitRecord& rec) const = 0;
    virtual bool bounding_box(double t0, double t1, AABB& bbox) const = 0;
};

class FlippedNormals : public Hitable {
public:
    FlippedNormals(std::shared_ptr<Hitable> ptr);

    bool hit(Ray const& r, double t_min, double t_max,
             HitRecord& rec) const override;

    bool bounding_box(double t0, double t1, AABB& box) const override;

private:
    std::shared_ptr<Hitable> ptr;
};

#endif
