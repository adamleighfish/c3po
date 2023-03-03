#ifndef geometry_h
#define geometry_h

#include "Imath/ImathBox.h"
#include "Imath/ImathVec.h"

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

    Ray();
    Ray(Point3f const& origin, Vec3f const& dir,
        float t_max = std::numeric_limits<float>::max(), float time = 0.f);

    Vec3f operator()(float t) const;
};

// Flips the normal vector `n` to face the same direction as `v`.
Normal3f face_forward(Normal3f const& n, Vec3f const& v);

// Test if `ray` intersects with `bounds`, return the result and the optional
// parametric values if an intersection occurs.
//
// This functions takes into account the ray's t_max value and ignores
// intersections that occur after it.
bool intersect_p(Bounds3f const& bounds, Ray const& ray,
                 float* hit_t0 = nullptr, float* hit_t1 = nullptr);

#endif
