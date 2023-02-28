#ifndef aggregate_h
#define aggregate_h

#include "hitable.h"

class Aggregate : public Hitable {
public:
    Hitable** list;
    int list_size;

    Aggregate(){};
    Aggregate(Hitable** list, int list_size)
        : list(list), list_size(list_size){};

    virtual bool Hit(Ray const& R, double t_min, double t_max,
                     HitRecord& rec) const;
    virtual bool BoundingBox(double t0, double t1, AABB& box) const;
};

bool Aggregate::Hit(Ray const& R, double t_min, double t_max,
                    HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; ++i) {
        if (list[i]->Hit(R, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool Aggregate::BoundingBox(double t0, double t1, AABB& box) const {
    if (list_size < 1)
        return false;

    AABB temp_box;
    bool first_true = list[0]->BoundingBox(t0, t1, temp_box);

    if (!first_true) {
        return false;
    }

    box = temp_box;
    for (int i = 1; i < list_size; ++i) {
        if (list[i]->BoundingBox(t0, t1, temp_box)) {
            box = get_bounding_box(box, temp_box);
        } else {
            return false;
        }
    }
    return true;
}

#endif
