#ifndef c3po_perlin_h
#define c3po_perlin_h

#include "geometry.h"

class Perlin {
public:
    Perlin() = default;

    double noise(Vec3 const& point) const;

    double turb(Vec3 const& point, int depth = 7) const;

private:
    static int* perm_x;
    static int* perm_y;
    static int* perm_z;
    static Vec3* ranvec;
};

#endif
