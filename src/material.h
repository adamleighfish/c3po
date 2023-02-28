#ifndef c3po_material_h
#define c3po_material_h

#include "geometry.h"

class Texture;
struct HitRecord;

class Material {
public:
    virtual bool scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                         Ray& scattered) const = 0;
    virtual Vec3 emit(double u, double v, Vec3 const& p) const {
        return Vec3(0, 0, 0);
    }
};

class Lambertian : public Material {
public:
    Lambertian(Texture* t);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                 Ray& scattered) const override;

private:
    Texture* texture;
};

class Metal : public Material {
public:
    Metal(Texture* t, double fuzz = 0.0);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                 Ray& scattered) const override;

private:
    Texture* texture;
    double const fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double ref_idx);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                 Ray& scattered) const override;

private:
    double ref_idx;
};

class DiffuseLight : public Material {
public:
    DiffuseLight(Texture* t);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                 Ray& scattered) const override;
    Vec3 emit(double u, double v, Vec3 const& p) const override;

private:
    Texture* texture;
};

class Isotropic : public Material {
public:
    Texture* texture;

    Isotropic(Texture* t);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3& attenuation,
                 Ray& scattered) const override;
};

#endif
