#include "interaction.h"

#include "primitive.h"
#include "shape.h"

namespace c3po {

Interaction::Interaction() : p(), n(), wo(), time(0.f) {}

Interaction::Interaction(Point3f const& p, Normal3f const& n, Vec3f const& wo,
                         float time)
    : p(p), n(n), wo(wo), time(time) {}

SurfaceInteraction::SurfaceInteraction()
    : Interaction(), uv(), dpdu(), dpdv(), shape(nullptr), primitive(nullptr),
      bsdf(nullptr) {}

SurfaceInteraction::SurfaceInteraction(Point3f const& p, Point2f const& uv,
                                       Vec3f const& wo, Vec3f const& dpdu,
                                       Vec3f const& dpdv, float time,
                                       Shape const* shape)
    : Interaction(p, cross(dpdu, dpdv).normalize(), wo, time), uv(uv),
      dpdu(dpdu), dpdv(dpdv), shape(shape), primitive(nullptr), bsdf(nullptr) {

    // Flip the normal vector if the shape has a reverse orientation or the
    // shape's transform with swap the handedness. This is an exclusive or
    // because if both happen they cancel out.
    if (shape &&
        (shape->reverse_orientation ^ shape->transform_swaps_handedness)) {
        n *= -1;
    }
}

} // namespace c3po
