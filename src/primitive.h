#ifndef C3PO_PRIMITIVE_H
#define C3PO_PRIMITIVE_H

#include "geometry.h"

namespace c3po {

class AreaLight;
class Material;
class Shape;
class SurfaceInteraction;

// Abstract interace for a primitive, which is the combination information
// needed for rendering (such as geometry, materials, texturesl, and lighting).
class Primitive {
public:
    virtual ~Primitive();
    virtual Bounds3f world_bounds() const = 0;
    virtual bool intersect(Ray const& r, SurfaceInteraction& isect) const = 0;
    virtual bool intersects(Ray const& r) const = 0;
    virtual AreaLight const* get_area_light() const = 0;
    virtual Material const* get_material() const = 0;
    virtual void
    compute_scattering_function(SurfaceInteraction& isect) const = 0;
};

// A geometric primitive is the main primitive that is rendered in a scene.
//
// It contains geometric information about a shape and the additional
// information needed for shading.
class GeometricPrimitive : public Primitive {
public:
    GeometricPrimitive(std::shared_ptr<Shape> shape,
                       std::shared_ptr<Material> material,
                       std::shared_ptr<AreaLight> area_light = nullptr);

    Bounds3f world_bounds() const override;

    bool intersect(Ray const& r, SurfaceInteraction& isect) const override;

    bool intersects(Ray const& r) const override;

    AreaLight const* get_area_light() const override;

    Material const* get_material() const override;

    void compute_scattering_function(SurfaceInteraction& isect) const override;

private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> area_light;
};

// An aggregate is a collection of primitive, providing an api so that the
// rendering can act like it is only dealing with one primitive.
//
// This is separated out to a separate empty class to allow multiple type of
// aggregates to be implemented. Right now I'm only going to implement a BVH but
// I may play around with more advanced acceleration structures in the future.
class Aggregate : public Primitive {

    // All of these functions should only ever be called on the underlying
    // geometric primitive and not the aggregate itself. Therefore they all log
    // and error and end the program.
    AreaLight const* get_area_light() const override;
    Material const* get_material() const override;
    void compute_scattering_function(SurfaceInteraction& isect) const override;
};

} // namespace c3po

#endif