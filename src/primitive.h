#ifndef C3PO_PRIMITIVE_H
#define C3PO_PRIMITIVE_H

#include "geometry.h"

namespace c3po {

class AreaLight;
class Material;
class Shape;
class SurfaceInteraction;

class Primitive {
public:
    virtual Bounds3f world_bounds() const = 0;
    virtual bool intersect(Ray const& r, SurfaceInteraction& isect) const = 0;
    virtual bool intersects(Ray const& r) const = 0;
    virtual AreaLight const* get_area_light() const = 0;
    virtual Material const* get_material() const = 0;
    virtual void
    compute_scattering_function(SurfaceInteraction& isect) const = 0;
};

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

} // namespace c3po

#endif