#ifndef c3po_texture_h
#define c3po_texture_h

#include "geometry.h"
#include "perlin.h"

class Texture {
public:
    virtual Vec3 sample(double u, double v, Vec3 const& point) const = 0;
};

class ConstantTexture : public Texture {
public:
    ConstantTexture(Vec3 const& color);

    Vec3 sample(double u, double v, Vec3 const& point) const override;

private:
    const Vec3 color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(Texture* odd, Texture* even);

    Vec3 sample(double u, double v, Vec3 const& point) const override;

private:
    const Texture* const odd;
    const Texture* const even;
};

class NoiseTexture : public Texture {
public:
    NoiseTexture(double scale);

    Vec3 sample(double u, double v, Vec3 const& point) const override;

private:
    const Perlin perlin;
    const double scale;
};

#endif
