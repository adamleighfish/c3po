#ifndef c3po_transformation_h
#define c3po_transformation_h

#include "hitable.h"

class Translate : public Hitable {
public:

    Translate(std::shared_ptr<Hitable> ptr, Vec3f const& offset);

    bool hit(Ray const& r, float t_min, float t_max,
             HitRecord& rec) const override;

    bool bounding_box(float t0, float t1, AABB& bbox) const override;

private:
    std::shared_ptr<Hitable> ptr;
    Vec3f offset;
};

class RotateY : public Hitable {
public:
    RotateY(std::shared_ptr<Hitable> ptr, float angle);

    virtual bool hit(Ray const& r, float t_min, float t_max,
                     HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& bbox) const;

private:
    std::shared_ptr<Hitable> ptr;
    float sin_theta;
    float cos_theta;
    bool has_box;
    AABB box;
};

#endif
