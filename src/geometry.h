#ifndef C3PO_GEOMETRY_H
#define C3PO_GEOMETRY_H

#include "Imath/ImathBox.h"
#include "Imath/ImathVec.h"

namespace c3po {

// XXX: Just using aliases for now, may need to make thin wrapper classes in
// the future to avoid implicit conversions.
using Vec2f = Imath::V2f;
using Point2f = Imath::V2f;
using Vec3f = Imath::V3f;
using Point3f = Imath::V3f;
using Normal3f = Imath::V3f;
using Bounds3f = Imath::Box3f;

// Left-handed cross product
//
// Imath only provides right-handed cross product function for compatibility
// with OpenGL. However, I don't care about OpenGL and most of the resources on
// raytracing (pbrt, renderman, etc.) use a left-handed coordinate system so
// this wrapper function takes care of that.
Vec3f cross(Vec3f const& a, Vec3f const& b);

// Returns the distance between the two points.
float distance(Point3f const& a, Point3f const& v);

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

// Special intersects methods that take a precomputed `inv_dir` && `dir_is_neg`
// values and only returns if a hit occurs.
//
// Used in the BVH to increase performace by ~15% according to pbrt.
bool intersects_p(Bounds3f const& bounds, Ray const& ray, Vec3f const& inv_dir,
                  int const dir_is_neg[3]);

// Computes the surface area of the bounds `b`.
float surface_area(Bounds3f const& b);

// Returns the continuous position of the point `p` relative to the corners of
// the bounds `b`.
//
// Minimum corner has the value (0, 0, 0) and the maximum corner has the value
// (1, 1, 1)
Vec3f offset(Bounds3f const& b, Point3f const& p);

} // namespace c3po

#endif
