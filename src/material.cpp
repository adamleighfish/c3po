#include "material.h"

#include "hitable.h"
#include "texture.h"
#include "utility.h"

Vec3f rand_vec_in_unit_sphere() {
    Vec3f p;
    do {
        p = (Vec3f(rand_float(0.0, 1.0), rand_float(0.0, 1.0),
                        rand_float(0.0, 1.0)) *
             2) -
            Vec3f(1.0);
    } while (p.length2() >= 1.0);
    return p.normalize();
}

Vec3f reflect(Vec3f const& v, Vec3f const& n) {
    return v - 2 * v.dot(n) * n;
}

bool refract(Vec3f const& v, Vec3f const& n, float ni_over_nt, Vec3f& refracted) {
    Vec3f uv = v.normalized();
    float dt = uv.dot(n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    if (discriminant > 0.0) {
        refracted = (uv - n * dt) * ni_over_nt - n * sqrt(discriminant);
        return true;
    }
    return false;
}

float schlick(float cosine, float ref_idx) {
    float r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * powf((1.0 - cosine), 5.0);
}

Lambertian::Lambertian(std::shared_ptr<Texture> t) : texture(t) {}

bool Lambertian::scatter(Ray const& r, HitRecord const& rec, Vec3f& attentuation,
                         Ray& scattered) const {
    Vec3f target = rec.point + rec.normal + rand_vec_in_unit_sphere();
    scattered = Ray(rec.point, target - rec.point, r.time);
    attentuation = texture->sample(0, 0, rec.point);
    return true;
}

Metal::Metal(std::shared_ptr<Texture> t, float fuzz)
    : texture(t), fuzz(fuzz) {}

bool Metal::scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                    Ray& scattered) const {
    Vec3f reflected = reflect(r.dir.normalized(), rec.normal);
    scattered =
        Ray(rec.point, reflected + rand_vec_in_unit_sphere() * fuzz, r.time);
    attenuation = texture->sample(0, 0, rec.point);
    return scattered.dir.dot(rec.normal) > 0.0;
}

Dielectric::Dielectric(float ref_idx) : ref_idx(ref_idx) {}

bool Dielectric::scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                         Ray& scattered) const {
    Vec3f outward_normal;
    Vec3f reflected = reflect(r.dir, rec.normal);
    float ni_over_nt;
    attenuation = Vec3f(1.0, 1.0, 1.0);
    Vec3f refracted;
    float reflect_prob;
    float cosine;

    if (r.dir.dot(rec.normal) > 0.0) {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        cosine = r.dir.dot(rec.normal / r.dir.length());
        cosine = sqrt(1.0 - ref_idx * ref_idx * (1.0 - cosine * cosine));
    } else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -(r.dir.dot(rec.normal / r.dir.length()));
    }

    if (refract(r.dir, outward_normal, ni_over_nt, refracted)) {
        reflect_prob = schlick(cosine, ref_idx);
    } else {
        reflect_prob = 1.0;
    }

    if (rand_float(0.0, 1.0) < reflect_prob) {
        scattered = Ray(rec.point, reflected, r.time);
    } else {
        scattered = Ray(rec.point, refracted, r.time);
    }

    return true;
}

DiffuseLight::DiffuseLight(std::shared_ptr<Texture> t) : texture(t) {}

bool DiffuseLight::scatter(Ray const& r, HitRecord const& rec,
                           Vec3f& attenuation, Ray& scattered) const {
    return false;
}

Vec3f DiffuseLight::emit(float u, float v, Vec3f const& point) const {
    return texture->sample(u, v, point);
}

Isotropic::Isotropic(Texture* t) : texture(t) {}

bool Isotropic::scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                        Ray& scattered) const {
    scattered = Ray(rec.point, rand_vec_in_unit_sphere(), 0);
    attenuation = texture->sample(rec.u, rec.v, rec.point);
    return true;
}
