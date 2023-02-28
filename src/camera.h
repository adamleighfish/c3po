#ifndef c3po_camera_h
#define c3po_camera_h

#include "geometry.h"

class Camera {
public:
    Camera(Vec3 const& look_from, Vec3 const& look_at, Vec3 const& v_up,
           double v_fov, double aspect, double aperture, double focus_dist,
           double t0 = 0.0, double t1 = 0.0);

    Ray gen_ray(double s, double t);

private:
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lens_radius;
    double time0, time1;
};

#endif
