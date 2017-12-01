//
//  material.h
//  toytracer
//
//  Created by Adam Fish on 11/4/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef material_h
#define material_h

#include "geometry.h"
#include "texture.h"

/**
 *
 *
 */

Vec3f RandomInUnitSphere() {
    Vec3f P;
    do {
        P = 2.0 * Vec3f(drand48(), drand48(), drand48()) - Vec3f(1.0, 1.0, 1.0);
    } while (P.LengthSquared() >= 1.0);
    return Normalize(P);
}

Vec3f Reflect(const Vec3f& V, const Vec3f& N) {
    return V - 2 * Dot(V, N) * N;
}

bool Refract(const Vec3f& V, const Vec3f& N, double ni_over_nt, Vec3f& Refracted) {
    Vec3f UV = Normalize(V);
    double dt = Dot(UV, N);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    if (discriminant > 0.0) {
        Refracted = ni_over_nt * (UV - N * dt) - N * sqrt(discriminant);
        return true;
    }
    return false;
}

double Schlick(double cosine, double ref_idx) {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

/**
 *
 *
 */

class Material {
public:
    virtual bool Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const = 0;
    virtual Vec3f Emitted(double u, double v, const Vec3f& P) const { return Vec3f(0, 0, 0); }
};

/**
 *
 *
 */

class Lambertian: public Material {
public:
    Texture* Albedo;
    
    Lambertian(Texture* A): Albedo(A) {}
    
    virtual bool Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const;
};

bool Lambertian::Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attentuation, Ray& Scattered) const {
    Vec3f Target = rec.P + rec.Normal + RandomInUnitSphere();
    Scattered = Ray(rec.P, Target - rec.P, Rin.time);
    Attentuation = Albedo->Value(0, 0, rec.P);
    return true;
}

/**
 *
 *
 */

class Metal: public Material {
public:
    Texture* Albedo;
    const double fuzz;
    
    Metal(Texture* A, double fuzz = 0.0): Albedo(A), fuzz(fuzz) {}
    
    virtual bool Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const;
};

bool Metal::Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const {
    Vec3f Reflected = Reflect(Normalize(Rin.D), rec.Normal);
    Scattered = Ray(rec.P, Reflected + fuzz * RandomInUnitSphere(), Rin.time);
    Attenuation = Albedo->Value(0, 0, rec.P);
    return Dot(Scattered.D, rec.Normal) > 0.0;
}

/**
 *
 *
 */

class Dielectric: public Material {
public:
    double ref_idx;
    
    Dielectric(double ref_idx): ref_idx(ref_idx) {}
    
    virtual bool Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const;
};

bool Dielectric::Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const {
    Vec3f OutwardNormal;
    Vec3f Reflected = Reflect(Rin.D, rec.Normal);
    double ni_over_nt;
    Attenuation = Vec3f(1.0, 1.0, 1.0);
    Vec3f Refracted;
    double reflect_prob;
    double cosine;
    
    if (Dot(Rin.D, rec.Normal) > 0.0) {
        OutwardNormal = -rec.Normal;
        ni_over_nt = ref_idx;
        cosine = Dot(Rin.D, rec.Normal / Rin.D.Length());
        cosine = sqrt(1.0 - ref_idx * ref_idx * (1.0 - cosine * cosine));
    } else {
        OutwardNormal = rec.Normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -Dot(Rin.D, rec.Normal / Rin.D.Length());
    }
    
    if (Refract(Rin.D, OutwardNormal, ni_over_nt, Refracted)) {
        reflect_prob = Schlick(cosine, ref_idx);
    } else {
        reflect_prob = 1.0;
    }
    
    if (drand48() < reflect_prob) {
        Scattered = Ray(rec.P, Reflected, Rin.time);
    } else {
        Scattered = Ray(rec.P, Refracted, Rin.time);
    }

    return true;
}

/**
 *
 *
 */

class DiffuseLight: public Material {
public:
    Texture* Emit;
    
    DiffuseLight(Texture* A): Emit(A) {}
    
    virtual bool Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const { return false; }
    virtual Vec3f Emitted(double u, double v, const Vec3f& P) const { return Emit->Value(u, v, P); }
};

/**
 *
 *
 */

class Isotropic: public Material {
public:
    Texture* Albedo;
    
    Isotropic(Texture* A): Albedo(A) {}
    
    virtual bool Scatter(const Ray& Rin, const HitRecord& rec, Vec3f& Attenuation, Ray& Scattered) const {
        Scattered = Ray(rec.P, RandomInUnitSphere(), 0);
        Attenuation = Albedo->Value(rec.u, rec.v, rec.P);
        return true;
    }
};

#endif /* material_h */
