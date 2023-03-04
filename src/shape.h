#ifndef C3PO_SHAPE_H
#define C3PO_SHAPE_H

#include "geometry.h"

#include <memory>

namespace c3po {

class SurfaceInteraction;
class Transform;

// Abstract Shape interface
//
// All shapes are described in object space and provide the transformation to
// and from world space. Transforms are stored in shared_ptr so that they can be
// shared between multiple items in a scene.
class Shape {
public:
    std::shared_ptr<Transform> const object_to_world;
    std::shared_ptr<Transform> const world_to_object;
    bool const reverse_orientation;
    bool const transform_swaps_handedness;

    Shape(std::shared_ptr<Transform> object_to_world,
          std::shared_ptr<Transform> world_to_object, bool reverse_orientation);

    // Returns the axis-aligned bounding box for this shape in object space.
    virtual Bounds3f object_bounds() const = 0;

    // Returns the axis aligned bounding box for this shape in world space.
    //
    // This should be overridden if the shape can compute a tighter world space
    // bounds.
    Bounds3f world_bounds() const;

    // Attempts to intersect ray `r` with this shape.
    //
    // If a hit occurs then returns true and records the hit location in `t_hit`
    // and the surface details in `isect`.
    //
    // The ray is assumed to be in world space;
    virtual bool intersect(Ray const& r, float& t_hit,
                           SurfaceInteraction& isect) const = 0;

    // Returns true if `ray` intersects with this shape.
    //
    // The ray is assumed to be in world space;
    virtual bool intersects(Ray const& r) const;

    virtual float area() const = 0;
};

} // namespace c3po

#endif