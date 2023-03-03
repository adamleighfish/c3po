#ifndef c3po_transform_h
#define c3po_transform_h

#include "geometry.h"
#include "interaction.h"

#include "Imath/ImathMatrix.h"

using Matrix4x4 = Imath::M44f;

class Transform {
public:
    Transform();
    Transform(float const mat[4][4]);
    Transform(Matrix4x4 const& m);
    Transform(Matrix4x4 const& m, Matrix4x4 const& m_inv);

    // Inverse in-place.
    Transform& inverse();

    // Return the inverse, leaving this unmodified.
    Transform inversed() const;

    // Transpose in-place.
    Transform& transpose();

    // Returns the transpose, leaving this unmodified.
    Transform transposed() const;

    // Translate in-place by the vector t.
    Transform& translate(Vec3f const& t);

    // Returns a new translation transform which translate by the vector t.
    static Transform translation(Vec3f const& t);

    // Scale in-place by the vector s.
    Transform& scale(Vec3f const& s);

    // Returns a new scaling transform which scales by the vector s.
    static Transform scaling(Vec3f const& s);

    // Rotate in-place using the euler angles in vector r.
    Transform& rotate(Vec3f const& r);

    // Returns a new rotation transform which rotates by the euler angles in
    // vector r.
    static Transform rotation(Vec3f const& r);

    // Returns the transform that rotates the point `pos` so that it points
    // toward `look` while maintaining the `up` direction vector.
    //
    // Useful for placing a camera in a scene and pointing it in the direction
    // you want.
    static Transform look_at(Point3f const& pos, Point3f const& look,
                             Vec3f const& up);

    // Applies the tranform to a the point `p` and returns the result.
    //
    // Point transformation applies the homogeneous weight which is frequently
    // just 1 but can be other values in perspective transforms.
    Point3f apply_p(Point3f const& p) const;

    // Applies the tranform to a the vector `v` and returns the result.
    Vec3f apply_v(Vec3f const& v) const;

    // Applies the tranform to a the normal `n` and returns the result.
    //
    // When tranformation are applied to a normal vector the inverse transpose
    // matrix is used instead of the normal transformation matrix. This is to
    // maintain the perpendicular aspect of normal vectors.
    Normal3f apply_n(Normal3f const& n) const;

    // Applies the transform to the ray `r` and returns the result.
    Ray apply(Ray const& r) const;

    // Applies the transform to the bounds `b` and returns the result.
    Bounds3f apply(Bounds3f const& b) const;

    // Applies the transform to the surface interaction `si` and returns the
    // results.
    SurfaceInteraction apply(SurfaceInteraction const& si) const;

    // Transform composition.
    Transform operator*(Transform const& t) const;

    // Returns true if applying this transform will swap the handedness.
    bool swaps_handedness() const;

    bool operator==(Transform const& t) const;
    bool operator!=(Transform const& t) const;

private:
    Matrix4x4 m;
    Matrix4x4 m_inv;
};

#endif