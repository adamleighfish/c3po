#ifndef c3po_texture_h
#define c3po_texture_h

#include "geometry.h"

class Texture {
public:
    virtual Vec3 sample(float u, float v, Vec3 const& point) const = 0;
};

class ConstantTexture : public Texture {
public:
    ConstantTexture(Vec3 const& color);

    Vec3 sample(float u, float v, Vec3 const& point) const override;

private:
    const Vec3 color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(Texture* odd, Texture* even);

    Vec3 sample(float u, float v, Vec3 const& point) const override;

private:
    const Texture* const odd;
    const Texture* const even;
};

#endif
