#ifndef C3PO_INTERACTION_H
#define C3PO_INTERACTION_H

#include "geometry.h"

namespace c3po {

class BSDF;
class Shape;
class Primitive;

class Interaction {
public:
    Point3f p;
    Normal3f n;
    Vec3f wo;
    float time;

    Interaction();
    Interaction(Point3f const& p, Normal3f const& n, Vec3f const& wo,
                float time);
};

class SurfaceInteraction : public Interaction {
public:
    Point2f uv;
    Vec3f dpdu;
    Vec3f dpdv;
    Shape const* shape;
    Primitive const* primitive;
    BSDF const* bsdf;

    SurfaceInteraction();
    SurfaceInteraction(Point3f const& p, Point2f const& uv, Vec3f const& wo,
                       Vec3f const& dpdu, Vec3f const& dpdv, float time,
                       Shape const* = nullptr);
};

} // namespace c3po

#endif