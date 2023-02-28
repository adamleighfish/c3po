#include "material.h"

#include "hitable.h"
#include "texture.h"
#include "utility.h"

Vec3 rand_vec_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(rand_double(0.0, 1.0), rand_double(0.0, 1.0),
                       rand_double(0.0, 1.0)) -
            Vec3(1.0, 1.0, 1.0);
    } while (p.len_squared() >= 1.0);
    return norm(p);
}

Vec3 reflect(Vec3 const& v, Vec3 const& n) { return v - 2 * dot(v, n) * n; }

bool refract(Vec3 const& v, Vec3 const& n, double ni_over_nt, Vec3& refracted) {
    Vec3 uv = norm(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    if (discriminant > 0.0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

double schlick(double cosine, double ref_idx) {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

Lambertian::Lambertian(std::shared_ptr<Texture> t) : texture(t) {}

bool Lambertian::scatter(Ray const& r, HitRecord const& rec, Vec3& attentuation,
                         Ray& scattered) const {
    Vec3 target = rec.point + rec.normal + rand_vec_in_unit_sphere();
    scattered = Ray(rec.point, target - rec.point, r.time);
    attentuation = texture->sample(0, 0, rec.point);
    return true;
}

Metal::Metal(std::shared_ptr<Texture> t, double fuzz) : texture(t), fuzz(fuzz) {}

bool Metal::scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                    Ray& scattered) const {
    Vec3 reflected = reflect(norm(r.dir), rec.normal);
    scattered =
        Ray(rec.point, reflected + fuzz * rand_vec_in_unit_sphere(), r.time);
    attenuation = texture->sample(0, 0, rec.point);
    return dot(scattered.dir, rec.normal) > 0.0;
}

Dielectric::Dielectric(double ref_idx) : ref_idx(ref_idx) {}

bool Dielectric::scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                         Ray& scattered) const {
    Vec3 outward_normal;
    Vec3 reflected = reflect(r.dir, rec.normal);
    double ni_over_nt;
    attenuation = Vec3(1.0, 1.0, 1.0);
    Vec3 refracted;
    double reflect_prob;
    double cosine;

    if (dot(r.dir, rec.normal) > 0.0) {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        cosine = dot(r.dir, rec.normal / r.dir.len());
        cosine = sqrt(1.0 - ref_idx * ref_idx * (1.0 - cosine * cosine));
    } else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -dot(r.dir, rec.normal / r.dir.len());
    }

    if (refract(r.dir, outward_normal, ni_over_nt, refracted)) {
        reflect_prob = schlick(cosine, ref_idx);
    } else {
        reflect_prob = 1.0;
    }

    if (rand_double(0.0, 1.0) < reflect_prob) {
        scattered = Ray(rec.point, reflected, r.time);
    } else {
        scattered = Ray(rec.point, refracted, r.time);
    }

    return true;
}

DiffuseLight::DiffuseLight(std::shared_ptr<Texture> t) : texture(t) {}

bool DiffuseLight::scatter(Ray const& r, HitRecord const& rec,
                           Vec3& attenuation, Ray& scattered) const {
    return false;
}

Vec3 DiffuseLight::emit(double u, double v, Vec3 const& point) const {
    return texture->sample(u, v, point);
}

Isotropic::Isotropic(Texture* t) : texture(t) {}

bool Isotropic::scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                        Ray& scattered) const {
    scattered = Ray(rec.point, rand_vec_in_unit_sphere(), 0);
    attenuation = texture->sample(rec.u, rec.v, rec.point);
    return true;
}
