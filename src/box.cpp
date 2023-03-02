#include "box.h"

#include "bvh.h"
#include "rectangle.h"

#include <vector>

Box::Box(Vec3 const& p_min, Vec3 const& p_max, std::shared_ptr<Material> mat)
    : p_min(p_min), p_max(p_max) {

    std::vector<std::shared_ptr<Hitable>> list;
    list.push_back(std::make_shared<RectXY>(p_min.x, p_max.x, p_min.y, p_max.y,
                                            p_max.z, mat));
    list.push_back(std::make_shared<FlippedNormals>(std::make_shared<RectXY>(
        p_min.x, p_max.x, p_min.y, p_max.y, p_min.z, mat)));
    list.push_back(std::make_shared<RectXZ>(p_min.x, p_max.x, p_min.z, p_max.z,
                                            p_max.y, mat));
    list.push_back(std::make_shared<FlippedNormals>(std::make_shared<RectXZ>(
        p_min.x, p_max.x, p_min.z, p_max.z, p_min.y, mat)));
    list.push_back(std::make_shared<RectYZ>(p_min.y, p_max.y, p_min.z, p_max.z,
                                            p_max.x, mat));
    list.push_back(std::make_shared<FlippedNormals>(std::make_shared<RectYZ>(
        p_min.y, p_max.y, p_min.z, p_max.z, p_min.x, mat)));

    bvh = std::make_shared<BVHNode>(list, 0, 0);
}

bool Box::hit(Ray const& r, float t_min, float t_max, HitRecord& rec) const {
    return bvh->hit(r, t_min, t_max, rec);
}

bool Box::bounding_box(float t0, float t1, AABB& bbox) const {
    bbox = AABB(p_min, p_max);
    return true;
}
