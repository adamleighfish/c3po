#ifndef c3po_sphere_h
#define c3po_sphere_h

#include "shape.h"

#include "geometry.h"

#include <memory>

class SurfaceInteraction;
class Transform;

class Sphere : public Shape {
public:
    Sphere(std::shared_ptr<Transform> object_to_world,
           std::shared_ptr<Transform> world_to_object, bool reverse_orientation,
           float radius);

    Bounds3f object_bounds() const override;

    bool intersect(Ray const& ray, float& t_hit,
                   SurfaceInteraction& isect) const override;

    bool intersects(Ray const& ray) const override;
    
    float area() const override;

private:
    float const radius;
};

#endif
