#ifndef camera_h
#define camera_h

#include <numbers>
#include <random>

#include "geometry.h"
#include "utility.h"

Vec3f RandomInUnitDisk() {
    Vec3f P;
    do {
        P = 2.0 * Vec3f(rand_double(0.0, 1.0), rand_double(0.0, 1.0), 0) -
            Vec3f(1, 1, 0);
    } while (Dot(P, P) >= 1.0);
    return P;
}

class Camera {
public:
    Vec3f Origin;
    Vec3f LowerLeftCorner;
    Vec3f Horizontal;
    Vec3f Vertical;
    Vec3f U, V, W;
    double lens_radius;
    double time0, time1;

    Camera(Vec3f const& LookFrom, Vec3f const& LookAt, Vec3f const& Vup,
           double vfov, double aspect, double aperture, double focus_dist,
           double t0 = 0.0, double t1 = 0.0) {
        lens_radius = aperture / 2.0;
        double theta = vfov * std::numbers::pi / 180;
        double half_height = tan(theta / 2.0);
        double half_width = aspect * half_height;
        Origin = LookFrom;
        W = Normalize(LookFrom - LookAt);
        U = Normalize(Cross(Vup, W));
        V = Cross(W, U);
        LowerLeftCorner = Origin - half_width * focus_dist * U -
                          half_height * focus_dist * V - focus_dist * W;
        Horizontal = 2 * half_width * focus_dist * U;
        Vertical = 2 * half_height * focus_dist * V;
        time0 = t0;
        time1 = t1;
    }

    Ray GetRay(double s, double t) {
        double time = time0 + rand_double(0.0, 1.0) * (time1 - time0);
        Vec3f RD = lens_radius * RandomInUnitDisk();
        Vec3f Offset = U * RD.x + V * RD.y;
        return Ray(Origin + Offset,
                   LowerLeftCorner + s * Horizontal + t * Vertical - Origin -
                       Offset,
                   time);
    }
};

#endif
