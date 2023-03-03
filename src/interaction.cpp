#include "interaction.h"

#include "shape.h"

Interaction::Interaction() : p(), n(), wo(), time(0.f) {}

Interaction::Interaction(Point3f const& p, Normal3f const& n, Vec3f const& wo,
                         float time)
    : p(p), n(n), wo(wo), time(time) {}

SurfaceInteraction::SurfaceInteraction()
    : Interaction(), uv(), dpdu(), dpdv(), shape(nullptr) {}

SurfaceInteraction::SurfaceInteraction(Point3f const& p, Point2f const& uv,
                                       Vec3f const& wo, Vec3f const& dpdu,
                                       Vec3f const& dpdv, float time,
                                       Shape const*)
    : Interaction(p, dpdu.cross(dpdv).normalize(), wo, time), uv(uv),
      dpdu(dpdu), dpdv(dpdv), shape(shape) {

    // Flip the normal vector if the shape has a reverse orientation or the
    // shape's transform with swap the handedness. This is an exclusive or
    // because if both happen they cancel out.
    if (shape &&
        (shape->reverse_orientation ^ shape->transform_swaps_handedness)) {
        n *= -1;
    }
}
