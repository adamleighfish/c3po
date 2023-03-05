#include "primitive.h"

#include "interaction.h"
#include "shape.h"

#include <iostream>

namespace c3po {

Primitive::~Primitive() = default;

GeometricPrimitive::GeometricPrimitive(std::shared_ptr<Shape> shape,
                                       std::shared_ptr<Material> material,
                                       std::shared_ptr<AreaLight> area_light)
    : shape(std::move(shape)), material(std::move(material)),
      area_light(std::move(area_light)) {}

Bounds3f GeometricPrimitive::world_bounds() const {
    return shape->world_bounds();
}

bool GeometricPrimitive::intersect(Ray const& r,
                                   SurfaceInteraction& isect) const {
    float t_hit;
    if (!shape->intersect(r, t_hit, isect)) {
        return false;
    }

    r.t_max = t_hit;
    isect.primitive = this;
    return true;
}

bool GeometricPrimitive::intersects(Ray const& r) const {
    return shape->intersects(r);
}

AreaLight const* GeometricPrimitive::get_area_light() const {
    return area_light.get();
}

Material const* GeometricPrimitive::get_material() const {
    return material.get();
}

void GeometricPrimitive::compute_scattering_function(
    SurfaceInteraction& isect) const {}

AreaLight const* Aggregate::get_area_light() const {
    std::cerr << "Aggregate::get_area_light() called" << std::endl;
    exit(1);
    return nullptr;
};

Material const* Aggregate::get_material() const {
    std::cerr << "Aggregate::get_material() called" << std::endl;
    exit(1);
    return nullptr;
};

void Aggregate::compute_scattering_function(SurfaceInteraction& isect) const {
    std::cerr << "Aggregate::compute_scattering_function() called" << std::endl;
    exit(1);
};

} // namespace c3po
