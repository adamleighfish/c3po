#ifndef c3po_rectangle_h
#define c3po_rectangle_h

#include "hitable.h"

class Material;

class RectXY : public Hitable {
public:
    RectXY(float x0, float x1, float y0, float y1, float k,
           std::shared_ptr<Material> mat);

    bool hit(Ray const& r, float t0, float t1, HitRecord& rec) const override;
    bool bounding_box(float t0, float t1, AABB& box) const override;

private:
    float x0, x1, y0, y1, k;
    std::shared_ptr<Material> mat;
};

class RectXZ : public Hitable {
public:
    RectXZ(float x0, float x1, float z0, float z1, float k,
           std::shared_ptr<Material> mat);

    virtual bool hit(Ray const& r, float t0, float t1, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const;

private:
    float x0, x1, z0, z1, k;
    std::shared_ptr<Material> mat;
};

class RectYZ : public Hitable {
public:
    RectYZ(float x0, float x1, float z0, float z1, float k,
           std::shared_ptr<Material> mat);

    virtual bool hit(Ray const& r, float t0, float t1, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const;

private:
    float y0, y1, z0, z1, k;
    std::shared_ptr<Material> mat;
};

#endif
