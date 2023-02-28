#include "transformation.h"

#include <limits>
#include <numbers>

Translate::Translate(std::shared_ptr<Hitable> ptr, Vec3 const& offset)
    : ptr(ptr), offset(offset) {}

bool Translate::hit(Ray const& r, double t_min, double t_max,
                    HitRecord& rec) const {
    Ray moved(r.origin - offset, r.dir, r.time);
    if (ptr->hit(moved, t_min, t_max, rec)) {
        rec.point += offset;
        return true;
    }
    return false;
}

bool Translate::bounding_box(double t0, double t1, AABB& bbox) const {
    if (ptr->bounding_box(t0, t1, bbox)) {
        bbox = AABB(bbox.min + offset, bbox.max + offset);
        return true;
    }
    return false;
}

RotateY::RotateY(std::shared_ptr<Hitable> ptr, double angle) : ptr(ptr) {
    double radians = (std::numbers::pi / 180.0) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    has_box = ptr->bounding_box(0, 1, box);
    Vec3 min(std::numeric_limits<double>::max(),
             std::numeric_limits<double>::max(),
             std::numeric_limits<double>::max());
    Vec3 max(-std::numeric_limits<double>::max(),
             -std::numeric_limits<double>::max(),
             -std::numeric_limits<double>::max());

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                double x = i * box.max.x + (1 - i) * box.min.x;
                double y = j * box.max.y + (1 - j) * box.min.y;
                double z = k * box.max.z + (1 - k) * box.min.z;
                double new_x = cos_theta * x + sin_theta * z;
                double new_z = cos_theta * z - sin_theta * x;

                Vec3 tmp(new_x, y, new_z);
                for (int c = 0; c < 3; ++c) {
                    if (tmp[c] > max[c]) {
                        max.SetValue(c, tmp[c]);
                    }
                    if (tmp[c] < min[c]) {
                        min.SetValue(c, tmp[c]);
                    }
                }
            }
        }
    }
    box = AABB(min, max);
}

bool RotateY::hit(Ray const& r, double t_min, double t_max,
                  HitRecord& rec) const {
    Vec3 origin = r.origin;
    Vec3 dir = r.dir;
    origin.x = cos_theta * r.origin.x - sin_theta * r.origin.z;
    origin.z = sin_theta * r.origin.x + cos_theta * r.origin.z;
    dir.x = cos_theta * r.dir.x - sin_theta * r.dir.z;
    dir.z = sin_theta * r.dir.x + cos_theta * r.dir.z;

    Ray rotated(origin, dir, r.time);
    if (ptr->hit(rotated, t_min, t_max, rec)) {
        Vec3 point = rec.point;
        Vec3 normal = rec.normal;
        point.x = cos_theta * rec.point.x + sin_theta * rec.point.z;
        point.z = -sin_theta * rec.point.x + cos_theta * rec.point.z;
        normal.x = cos_theta * rec.normal.x + sin_theta * rec.normal.z;
        normal.z = -sin_theta * rec.normal.x + cos_theta * rec.normal.z;
        rec.point = point;
        rec.normal = normal;
        return true;
    }
    return false;
}

bool RotateY::bounding_box(double t0, double t1, AABB& bbox) const {
    bbox = box;
    return has_box;
}
