#ifndef c3po_interaction_h
#define c3po_interaction_h

#include "geometry.h"

class Shape;

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
    Normal3f dndu;
    Normal3f dndv;
    Shape const* shape;
    
    SurfaceInteraction();
    SurfaceInteraction(Point3f const& p, Point2f const& uv, Vec3f const& wo,
                       Vec3f const& dpdu, Vec3f const& dpdv,
                       Normal3f const& dndu, Normal3f const& dndv, float time,
                       Shape const* = nullptr);
};

#endif