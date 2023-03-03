#include "camera.h"

#include "utility.h"

#include <numbers>

Vec3f rand_vec_in_unit_disk() {
    Vec3f p;
    do {
        p = Vec3f(rand_float(0.0, 1.0), rand_float(0.0, 1.0), 0) * 2.0 -
            Vec3f(1, 1, 0);
    } while (p.dot(p) >= 1.0);
    return p;
}

Camera::Camera(Vec3f const& look_from, Vec3f const& look_at, Vec3f const& v_up,
               float v_fov, float aspect, float aperture, float focus_dist,
               float t0, float t1) {
    lens_radius = aperture / 2.0;
    float theta = v_fov * std::numbers::pi / 180;
    float half_height = tan(theta / 2.0);
    float half_width = aspect * half_height;
    origin = look_from;
    w = (look_from - look_at).normalize();
    u = v_up.cross(w).normalize();
    v = w.cross(u);
    lower_left_corner = origin - half_width * focus_dist * u -
                        half_height * focus_dist * v - focus_dist * w;
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
    time0 = t0;
    time1 = t1;
}

Ray Camera::gen_ray(float s, float t) {
    float time = time0 + rand_float(0.0, 1.0) * (time1 - time0);
    Vec3f rd = lens_radius * rand_vec_in_unit_disk();
    Vec3f offset = u * rd.x + v * rd.y;
    return Ray(origin + offset,
               lower_left_corner + s * horizontal + t * vertical - origin -
                   offset,
               time);
}
