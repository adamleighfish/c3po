#include "transform.h"

#include "Imath/ImathBoxAlgo.h"
#include "Imath/ImathMatrixAlgo.h"

Transform::Transform() = default;

Transform::Transform(float const mat[4][4]) {
    m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0],
                  mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1],
                  mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2],
                  mat[3][3]);
    m_inv = m.inverse();
}

Transform::Transform(Matrix4x4 const& m) : m(m), m_inv(m.inverse()) {}

Transform::Transform(Matrix4x4 const& m, Matrix4x4 const& m_inv)
    : m(m), m_inv(m_inv) {}

Transform& Transform::inverse() {
    std::swap(m, m_inv);
    return *this;
}

Transform Transform::inversed() const { return Transform(m_inv, m); }

Transform& Transform::transpose() {
    m.transpose();
    m_inv.transpose();
    return *this;
}

Transform Transform::transposed() const {
    return Transform(m.transposed(), m_inv.transposed());
}

Transform& Transform::translate(Vec3f const& t) {
    m.translate(t);
    m_inv.translate(Vec3f(-t.x, -t.y, -t.z));
    return *this;
}

Transform Transform::translation(Vec3f const& t) {
    Matrix4x4 m;
    m.translate(t);
    Matrix4x4 m_inv;
    m_inv.translate(Vec3f(-t.x, -t.y, -t.z));
    return Transform(m, m_inv);
}

Transform& Transform::scale(Vec3f const& s) {
    m.scale(s);
    m_inv.scale(Vec3f(1 / s.x, 1 / s.y, 1 / s.z));
    return *this;
}

Transform Transform::scaling(Vec3f const& s) {
    Matrix4x4 m;
    m.scale(s);
    Matrix4x4 m_inv;
    m_inv.scale(Vec3f(1 / s.x, 1 / s.y, 1 / s.z));
    return Transform(m, m_inv);
}

Transform& Transform::rotate(Vec3f const& r) {
    m.rotate(r);
    m_inv = m.transposed();
    return *this;
}

Transform Transform::rotation(Vec3f const& r) {
    Matrix4x4 m;
    m.rotate(r);
    Matrix4x4 m_inv = m.transposed();
    return Transform(m, m_inv);
}

Transform Transform::look_at(Point3f const& pos, Point3f const& look,
                             Vec3f const& up) {
    return Imath::rotationMatrixWithUpDir(pos, look, up);
}

Point3f Transform::apply_p(Point3f const& p) const {
    Point3f res;
    m.multVecMatrix(p, res);
    return res;
}

Vec3f Transform::apply_v(Vec3f const& v) const {
    Vec3f res;
    m.multDirMatrix(v, res);
    return res;
}

Normal3f Transform::apply_n(Normal3f const& n) const {
    Normal3f res;
    m.transposed().multDirMatrix(n, res);
    return res;
}

Ray Transform::apply(Ray const& r) const {
    Point3f origin = (*this).apply_p(r.origin);
    Vec3f dir = this->apply_v(r.dir);
    return Ray(origin, dir, r.t_max, r.time);
}

Bounds3f Transform::apply(Bounds3f const& b) const {
    return Imath::transform(b, m);
}

SurfaceInteraction Transform::apply(SurfaceInteraction const& si) const {
    SurfaceInteraction res;
    res.p = this->apply_p(si.p);
    res.n = this->apply_n(si.n).normalize();
    res.wo = this->apply_v(si.wo);
    res.time = si.time;
    res.uv = si.uv;
    res.dpdu = this->apply_v(si.dpdu);
    res.dpdv = this->apply_v(si.dpdv);
    res.shape = si.shape;

    return res;
}

Transform Transform::operator*(Transform const& t) const {
    return Transform(Matrix4x4::multiply(m, t.m),
                     Matrix4x4::multiply(t.m_inv, m_inv));
}

bool Transform::swaps_handedness() const { 

    const float det = m.fastMinor(0, 1, 2, 0, 1, 2);
    return det < 0.f;
}

bool Transform::operator==(Transform const& t) const {
    return (m == t.m && m_inv == t.m_inv);
}

bool Transform::operator!=(Transform const& t) const {
    return (m != t.m || m_inv != t.m_inv);
}
