#include "sphere.h"

#include "interaction.h"
#include "transform.h"
#include "utility.h"

#include <numbers>

Sphere::Sphere(std::shared_ptr<Transform> object_to_world,
               std::shared_ptr<Transform> world_to_object,
               bool reverse_orientation, float radius)
    : Shape(std::move(object_to_world), std::move(world_to_object),
            reverse_orientation),
      radius(radius) {}

Bounds3f Sphere::object_bounds() const {
    return Bounds3f(Vec3f(-radius), Vec3f(radius));
}

bool Sphere::intersect(Ray const& r, float& t_hit,
                       SurfaceInteraction& isect) const {

    // transform the ray to object space
    Ray const ray = world_to_object->apply(r);

    // compute the quadratic sphere coefficients
    float const a = ray.dir.dot(ray.dir);
    float const b = 2 * (ray.dir.dot(ray.origin));
    float const c = ray.origin.dot(ray.origin) - radius * radius;

    // solve the quadratic equation for t values
    float t0;
    float t1;
    if (!quadratic(a, b, c, t0, t1)) {
        return false;
    }

    // check that the t values are in bounds
    if (t0 > ray.t_max || t1 <= 0.f) {
        return false;
    }

    // save the nearest intersection
    float t_shape_hit = t0;
    if (t_shape_hit <= 0.f) {
        t_shape_hit = t1;
        if (t_shape_hit > ray.t_max) {
            return false;
        }
    }

    // compute sphere hit position
    Point3f p_hit = ray(t_shape_hit);
    p_hit *= radius / distance(p_hit, Point3f(0.f));
    if (p_hit.x == 0.f && p_hit.y == 0.f) {
        p_hit.x = 1e-5f * radius;
    }

    // compute sphere hit phi
    float phi = std::atan2f(p_hit.y, p_hit.x);
    if (phi < 0.f) {
        phi += 2.f * float(std::numbers::pi);
    }

    // find the parametric representation of the sphere hit
    float const u = phi / float(2 * std::numbers::pi);
    float const theta = std::acosf(clamp(p_hit.z / radius, -1.f, 1.f));
    float const v = (theta - 1.f) / 2.f;

    // compute sphere dpdu and dpdv
    float const z_radius = std::sqrtf(p_hit.x * p_hit.x + p_hit.y * p_hit.y);
    float const inv_z_radius = 1.f / z_radius;
    float const cos_phi = p_hit.x * inv_z_radius;
    float const sin_phi = p_hit.y * inv_z_radius;
    Vec3f const dpdu(-2.f * float(std::numbers::pi) * p_hit.y,
                        2.f * float(std::numbers::pi) * p_hit.x, 0.f);
    Vec3f const dpdv(2.f * p_hit.z * cos_phi, 2.f * p_hit.z * sin_phi,
                        -2.f * radius * std::sinf(theta));

    // initialize surface interaction from parametric information
    isect = object_to_world->apply(SurfaceInteraction(
        p_hit, Point2f(u, v), -ray.dir, dpdu, dpdv, ray.time, this));

    // update t_hit for quadric intersection
    t_hit = t_shape_hit;

    return true;
}

bool Sphere::intersects(Ray const& r) const {
    // transform the ray to object space
    Ray const ray = world_to_object->apply(r);

    // compute the quadratic sphere coefficients
    float const a = ray.dir.dot(ray.dir);
    float const b = 2 * (ray.dir.dot(ray.origin));
    float const c = ray.origin.dot(ray.origin) - radius * radius;

    // solve the quadratic equation for t values
    float t0;
    float t1;
    if (!quadratic(a, b, c, t0, t1)) {
        return false;
    }

    // check that the t values are in bounds
    if (t0 > ray.t_max || t1 <= 0.f) {
        return false;
    }

    // save the nearest intersection
    float t_shape_hit = t0;
    if (t_shape_hit <= 0.f) {
        t_shape_hit = t1;
        if (t_shape_hit > ray.t_max) {
            return false;
        }
    }

    return true;
}

float Sphere::area() const {
    return 4.f * float(std::numbers::pi) * radius * radius;
}
