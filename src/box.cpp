#include "box.h"

#include "bvh.h"
#include "rectangle.h"

Box::Box(Vec3 const& p_min, Vec3 const& p_max, Material* mat_ptr)
    : p_min(p_min), p_max(p_max) {
    Hitable** list = new Hitable*[6];
    list[0] = new RectXY(p_min.x, p_max.x, p_min.y, p_max.y, p_max.z, mat_ptr);
    list[1] = new FlippedNormals(
        new RectXY(p_min.x, p_max.x, p_min.y, p_max.y, p_min.z, mat_ptr));
    list[2] = new RectXZ(p_min.x, p_max.x, p_min.z, p_max.z, p_max.y, mat_ptr);
    list[3] = new FlippedNormals(
        new RectXZ(p_min.x, p_max.x, p_min.z, p_max.z, p_min.y, mat_ptr));
    list[4] = new RectYZ(p_min.y, p_max.y, p_min.z, p_max.z, p_max.x, mat_ptr);
    list[5] = new FlippedNormals(
        new RectYZ(p_min.y, p_max.y, p_min.z, p_max.z, p_min.x, mat_ptr));

    bvh_ptr = new BVHNode(list, 6, 0, 0);
}

bool Box::hit(Ray const& r, double t_min, double t_max, HitRecord& rec) const {
    return bvh_ptr->hit(r, t_min, t_max, rec);
}

bool Box::bounding_box(double t0, double t1, AABB& bbox) const {
    bbox = AABB(p_min, p_max);
    return true;
}
