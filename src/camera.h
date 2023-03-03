#ifndef c3po_camera_h
#define c3po_camera_h

#include "geometry.h"

class Camera {
public:
    Camera(Vec3f const& look_from, Vec3f const& look_at, Vec3f const& v_up,
           float v_fov, float aspect, float aperture, float focus_dist,
           float t0 = 0.0, float t1 = 0.0);

    Ray gen_ray(float s, float t);

private:
    Vec3f origin;
    Vec3f lower_left_corner;
    Vec3f horizontal;
    Vec3f vertical;
    Vec3f u, v, w;
    float lens_radius;
    float time0, time1;
};

#endif
