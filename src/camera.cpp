#include "camera.h"

#include "utility.h"

#include <numbers>

Vec3 rand_vec_in_unit_disk() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(rand_double(0.0, 1.0), rand_double(0.0, 1.0), 0) -
            Vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

Camera::Camera(Vec3 const& look_from, Vec3 const& look_at, Vec3 const& v_up,
               double v_fov, double aspect, double aperture, double focus_dist,
               double t0, double t1) {
    lens_radius = aperture / 2.0;
    double theta = v_fov * std::numbers::pi / 180;
    double half_height = tan(theta / 2.0);
    double half_width = aspect * half_height;
    origin = look_from;
    w = norm(look_from - look_at);
    u = norm(cross(v_up, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width * focus_dist * u -
                        half_height * focus_dist * v - focus_dist * w;
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
    time0 = t0;
    time1 = t1;
}

Ray Camera::gen_ray(double s, double t) {
    double time = time0 + rand_double(0.0, 1.0) * (time1 - time0);
    Vec3 rd = lens_radius * rand_vec_in_unit_disk();
    Vec3 offset = u * rd.x + v * rd.y;
    return Ray(origin + offset,
               lower_left_corner + s * horizontal + t * vertical - origin -
                   offset,
               time);
}
