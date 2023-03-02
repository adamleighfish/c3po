#ifndef geometry_h
#define geometry_h

#include "Imath/ImathVec.h"

using Vec3 = Imath::V3f;

class Ray {
public:
    Vec3 origin;
    Vec3 dir;
    float time;

    Ray() : time(0.0){};
    explicit Ray(Vec3 const& origin, Vec3 const& dir, float time = 0.0)
        : origin(origin), dir(dir), time(time) {}


    Vec3 operator()(double t) const { return origin + dir * t; }
};

#endif
