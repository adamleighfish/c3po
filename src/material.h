#ifndef c3po_material_h
#define c3po_material_h

#include "geometry.h"

#include "Imath/ImathVec.h"

#include <memory>

class Texture;
struct HitRecord;

class Material {
public:
    virtual bool scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                         Ray& scattered) const = 0;
    virtual Vec3f emit(float u, float v, Vec3f const& p) const {
        return Vec3f(0);
    }
};

class Lambertian : public Material {
public:
    Lambertian(std::shared_ptr<Texture> t);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                 Ray& scattered) const override;

private:
    std::shared_ptr<Texture> texture;
};

class Metal : public Material {
public:
    Metal(std::shared_ptr<Texture> t, float fuzz = 0.0);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                 Ray& scattered) const override;

private:
    std::shared_ptr<Texture> texture;
    const float fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(float ref_idx);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                 Ray& scattered) const override;

private:
    float ref_idx;
};

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> t);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                 Ray& scattered) const override;
    Vec3f emit(float u, float v, Vec3f const& p) const override;

private:
    std::shared_ptr<Texture> texture;
};

class Isotropic : public Material {
public:
    Texture* texture;

    Isotropic(Texture* t);

    bool scatter(Ray const& r, HitRecord const& rec, Vec3f& attenuation,
                 Ray& scattered) const override;
};

#endif
