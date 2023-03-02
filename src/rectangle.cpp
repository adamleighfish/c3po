#include "rectangle.h"

#include "material.h"

RectXY::RectXY(float x0, float x1, float y0, float y1, float k,
               std::shared_ptr<Material> mat)
    : x0(x0), x1(x1), y0(y0), y1(y1), k(k), mat(mat) {}

bool RectXY::hit(Ray const& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin.z) / r.dir.z;
    if (t < t0 or t > t1) {
        return false;
    }

    float x = r.origin.x + t * r.dir.x;
    float y = r.origin.y + t * r.dir.y;
    if (x < x0 or x > x1 or y < y0 or y > y1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.mat_ptr = mat.get();
    rec.point = r(t);
    rec.normal = Vec3(0, 0, 1);

    return true;
}

bool RectXY::bounding_box(float t0, float t1, AABB& bbox) const {
    bbox = AABB(Vec3(x0, y0, k - 0.0001), Vec3(x1, y1, k + 0.0001));
    return true;
}

RectXZ::RectXZ(float x0, float x1, float z0, float z1, float k,
               std::shared_ptr<Material> mat)
    : x0(x0), x1(x1), z0(z0), z1(z1), k(k), mat(mat) {}

bool RectXZ::hit(Ray const& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin.y) / r.dir.y;
    if (t < t0 or t > t1) {
        return false;
    }

    float x = r.origin.x + t * r.dir.x;
    float z = r.origin.z + t * r.dir.z;
    if (x < x0 or x > x1 or z < z0 or z > z1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mat.get();
    rec.point = r(t);
    rec.normal = Vec3(0, 1, 0);
    return true;
}

bool RectXZ::bounding_box(float t0, float t1, AABB& bbox) const {
    bbox = AABB(Vec3(x0, k - 0.0001, z0), Vec3(x1, k + 0.0001, z1));
    return true;
}

RectYZ::RectYZ(float x0, float x1, float z0, float z1, float k,
               std::shared_ptr<Material> mat)
    : y0(x0), y1(x1), z0(z0), z1(z1), k(k), mat(mat) {}

bool RectYZ::hit(Ray const& r, float t0, float t1, HitRecord& rec) const {
    float t = (k - r.origin.x) / r.dir.x;
    if (t < t0 or t > t1) {
        return false;
    }

    float y = r.origin.y + t * r.dir.y;
    float z = r.origin.z + t * r.dir.z;
    if (y < y0 or y > y1 or z < z0 or z > z1) {
        return false;
    }

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mat.get();
    rec.point = r(t);
    rec.normal = Vec3(1, 0, 0);
    return true;
}

bool RectYZ::bounding_box(float t0, float t1, AABB& bbox) const {
    bbox = AABB(Vec3(k - 0.0001, y0, z0), Vec3(k + 0.0001, y1, z1));
    return true;
}
