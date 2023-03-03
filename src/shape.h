#ifndef c3po_shape_h
#define c3po_shape_h

#include <memory>

class Transform;

// Abstract Shape interface
//
// All shapes are described in object space and provide the transformation to
// and from world space. Transforms are stored in shared_ptr so that they can be
// shared between multiple items in a scene.
class Shape {
public:
    const std::shared_ptr<Transform> object_to_world;
    const std::shared_ptr<Transform> world_to_object;
    const bool reverse_orientation;
    const bool transform_swaps_handedness;

    Shape(const std::shared_ptr<Transform> object_to_world,
          const std::shared_ptr<Transform> world_to_object,
          bool reverse_orientation);

};

#endif