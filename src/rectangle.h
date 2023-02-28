#ifndef c3po_rectangle_h
#define c3po_rectangle_h

#include "hitable.h"

class Material;

class RectXY : public Hitable {
public:
    RectXY(double x0, double x1, double y0, double y1, double k,
           std::shared_ptr<Material> mat);

    bool hit(Ray const& r, double t0, double t1, HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& box) const override;

private:
    double x0, x1, y0, y1, k;
    std::shared_ptr<Material> mat;
};

class RectXZ : public Hitable {
public:
    RectXZ(double x0, double x1, double z0, double z1, double k,
           std::shared_ptr<Material> mat);

    virtual bool hit(Ray const& r, double t0, double t1, HitRecord& rec) const;
    virtual bool bounding_box(double t0, double t1, AABB& box) const;

private:
    double x0, x1, z0, z1, k;
    std::shared_ptr<Material> mat;
};

class RectYZ : public Hitable {
public:
    RectYZ(double x0, double x1, double z0, double z1, double k,
           std::shared_ptr<Material> mat);

    virtual bool hit(Ray const& r, double t0, double t1, HitRecord& rec) const;
    virtual bool bounding_box(double t0, double t1, AABB& box) const;

private:
    double y0, y1, z0, z1, k;
    std::shared_ptr<Material> mat;
};

#endif
