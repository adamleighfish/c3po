#ifndef c3po_aggregate_h
#define c3po_aggregate_h

#include "hitable.h"

#include <vector>

// A simple list of hitable scene objects.
class Aggregate : public Hitable {
public:
    Aggregate(const std::vector<Hitable*>& list);

    // Iterates over the list in order until it hits an object.
    bool hit(Ray const& R, double t_min, double t_max,
             HitRecord& rec) const override;

    // Calculates the bounding box that encompasses all objects in the
    // aggregate.
    //
    // Returns true if the bounding box could be constructed and false
    // otherwise.
    bool bounding_box(double t0, double t1, AABB& box) const override;

private:
    const std::vector<Hitable*> list;
};

#endif
