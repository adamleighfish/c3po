#ifndef c3po_transformation_h
#define c3po_transformation_h

#include "hitable.h"

class Translate : public Hitable {
public:

    Translate(std::shared_ptr<Hitable> ptr, Vec3 const& offset);

    bool hit(Ray const& r, double t_min, double t_max,
             HitRecord& rec) const override;

    bool bounding_box(double t0, double t1, AABB& bbox) const override;

private:
    std::shared_ptr<Hitable> ptr;
    Vec3 offset;
};

class RotateY : public Hitable {
public:
    RotateY(std::shared_ptr<Hitable> ptr, double angle);

    virtual bool hit(Ray const& r, double t_min, double t_max,
                     HitRecord& rec) const;
    virtual bool bounding_box(double t0, double t1, AABB& bbox) const;

private:
    std::shared_ptr<Hitable> ptr;
    double sin_theta;
    double cos_theta;
    bool has_box;
    AABB box;
};

#endif
