#ifndef c3po_camera_h
#define c3po_camera_h

#include "geometry.h"

class Camera {
public:
    Camera(Vec3 const& look_from, Vec3 const& look_at, Vec3 const& v_up,
           float v_fov, float aspect, float aperture, float focus_dist,
           float t0 = 0.0, float t1 = 0.0);

    Ray gen_ray(float s, float t);

private:
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    float lens_radius;
    float time0, time1;
};

#endif
