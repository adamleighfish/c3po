#include "shape.h"

#include "interaction.h"
#include "transform.h"

namespace c3po {

Shape::Shape(std::shared_ptr<Transform> object_to_world,
             std::shared_ptr<Transform> world_to_object,
             bool reverse_orientation)
    : object_to_world(std::move(object_to_world)),
      world_to_object(std::move(world_to_object)),
      reverse_orientation(reverse_orientation),
      transform_swaps_handedness(this->object_to_world->swaps_handedness()) {}

Bounds3f Shape::world_bounds() const {
    return object_to_world->apply(object_bounds());
}

bool Shape::intersects(Ray const& ray) const {
    float t_hit = ray.t_max;
    SurfaceInteraction isect;
    return intersect(ray, t_hit, isect);
}

} // namespace
