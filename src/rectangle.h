#ifndef rectangle_h
#define rectangle_h

#include "hitable.h"
#include "material.h"

/**
 *
 *
 */

class RectXY : public Hitable {
public:
    double x0, x1, y0, y1, k;
    Material* mat_ptr;

    RectXY() {}
    RectXY(double x0, double x1, double y0, double y1, double k,
           Material* mat_ptr)
        : x0(x0), x1(x1), y0(y0), y1(y1), k(k), mat_ptr(mat_ptr) {}

    virtual bool Hit(Ray const& R, double t0, double t1, HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

bool RectXY::Hit(Ray const& R, double t0, double t1, HitRecord& rec) const {
    double t = (k - R.O.z) / R.D.z;
    if (t < t0 or t > t1) {
        return false;
    }

    double x = R.O.x + t * R.D.x;
    double y = R.O.y + t * R.D.y;
    if (x < x0 or x > x1 or y < y0 or y > y1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.mat_ptr = mat_ptr;
    rec.P = R(t);
    rec.Normal = Vec3f(0, 0, 1);
    return true;
}

bool RectXY::BoundingBox(double t0, double t1, AABB& box) const {
    box = AABB(Vec3f(x0, y0, k - 0.0001), Vec3f(x1, y1, k + 0.0001));
    return true;
}

/**
 *
 *
 */

class RectXZ : public Hitable {
public:
    double x0, x1, z0, z1, k;
    Material* mat_ptr;

    RectXZ() {}
    RectXZ(double x0, double x1, double z0, double z1, double k,
           Material* mat_ptr)
        : x0(x0), x1(x1), z0(z0), z1(z1), k(k), mat_ptr(mat_ptr) {}

    virtual bool Hit(Ray const& R, double t0, double t1, HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

bool RectXZ::Hit(Ray const& R, double t0, double t1, HitRecord& rec) const {
    double t = (k - R.O.y) / R.D.y;
    if (t < t0 or t > t1) {
        return false;
    }

    double x = R.O.x + t * R.D.x;
    double z = R.O.z + t * R.D.z;
    if (x < x0 or x > x1 or z < z0 or z > z1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mat_ptr;
    rec.P = R(t);
    rec.Normal = Vec3f(0, 1, 0);
    return true;
}

bool RectXZ::BoundingBox(double t0, double t1, AABB& box) const {
    box = AABB(Vec3f(x0, k - 0.0001, z0), Vec3f(x1, k + 0.0001, z1));
    return true;
}

/**
 *
 *
 */

class RectYZ : public Hitable {
public:
    double y0, y1, z0, z1, k;
    Material* mat_ptr;

    RectYZ() {}
    RectYZ(double x0, double x1, double z0, double z1, double k,
           Material* mat_ptr)
        : y0(x0), y1(x1), z0(z0), z1(z1), k(k), mat_ptr(mat_ptr) {}

    virtual bool Hit(Ray const& R, double t0, double t1, HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

bool RectYZ::Hit(Ray const& R, double t0, double t1, HitRecord& rec) const {
    double t = (k - R.O.x) / R.D.x;
    if (t < t0 or t > t1) {
        return false;
    }

    double y = R.O.y + t * R.D.y;
    double z = R.O.z + t * R.D.z;
    if (y < y0 or y > y1 or z < z0 or z > z1) {
        return false;
    }

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mat_ptr;
    rec.P = R(t);
    rec.Normal = Vec3f(1, 0, 0);
    return true;
}

bool RectYZ::BoundingBox(double t0, double t1, AABB& box) const {
    box = AABB(Vec3f(k - 0.0001, y0, z0), Vec3f(k + 0.0001, y1, z1));
    return true;
}

#endif
