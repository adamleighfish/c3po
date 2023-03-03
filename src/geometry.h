#ifndef geometry_h
#define geometry_h

#include "Imath/ImathBox.h"
#include "Imath/ImathVec.h"

#include <limits>

// XXX: Just using aliases for now, may need to make thin wrapper classes in
// the future to avoid implicit conversions.
using Vec2f = Imath::V2f;
using Point2f = Imath::V2f;
using Vec3f = Imath::V3f;
using Point3f = Imath::V3f;
using Normal3f = Imath::V3f;
using Bounds3f = Imath::Box3f;

class Ray {
public:
    Point3f origin;
    Vec3f dir;
    mutable float t_max;
    float time;

    Ray() : t_max(std::numeric_limits<float>::max()), time(0.f){};
    Ray(Point3f const& origin, Vec3f const& dir,
        float t_max = std::numeric_limits<float>::max(), float time = 0.0)
        : origin(origin), dir(dir), t_max(t_max), time(time) {}

    Vec3f operator()(float t) const { return origin + dir * t; }
};

// Flips the normal vector `n` to face the same direction as `v`.
Normal3f face_forward(Normal3f const& n, Vec3f const& v) {
    return (n.dot(v) < 0.f) ? -n : n;
}

#endif
