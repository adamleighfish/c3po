#include "aggregate.h"

Aggregate::Aggregate(const std::vector<Hitable*>& list) : list(list) {}

bool Aggregate::hit(Ray const& r, double t_min, double t_max,
                    HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list.size(); ++i) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool Aggregate::bounding_box(double t0, double t1, AABB& bbox) const {
    if (list.size() < 1)
        return false;

    AABB temp_box;
    bool first_true = list[0]->bounding_box(t0, t1, temp_box);

    if (!first_true) {
        return false;
    }

    bbox = temp_box;
    for (int i = 1; i < list.size(); ++i) {
        if (list[i]->bounding_box(t0, t1, temp_box)) {
            bbox = get_bounding_box(bbox, temp_box);
        } else {
            return false;
        }
    }
    return true;
}
