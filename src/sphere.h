#ifndef C3PO_SPHERE_H
#define C3PO_SPHERE_H

#include "shape.h"

#include "geometry.h"

#include <memory>

namespace c3po {

class SurfaceInteraction;
class Transform;

class Sphere : public Shape {
public:
    Sphere(std::shared_ptr<Transform> object_to_world,
           std::shared_ptr<Transform> world_to_object, bool reverse_orientation,
           float radius);

    Bounds3f object_bounds() const override;

    bool intersect(Ray const& r, float& t_hit,
                   SurfaceInteraction& isect) const override;

    bool intersects(Ray const& r) const override;

    float area() const override;

private:
    float const radius;
};

} // namespace c3po

#endif
