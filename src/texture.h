#ifndef c3po_texture_h
#define c3po_texture_h

#include "geometry.h"

class Texture {
public:
    virtual Vec3f sample(float u, float v, Vec3f const& point) const = 0;
};

class ConstantTexture : public Texture {
public:
    ConstantTexture(Vec3f const& color);

    Vec3f sample(float u, float v, Vec3f const& point) const override;

private:
    const Vec3f color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(Texture* odd, Texture* even);

    Vec3f sample(float u, float v, Vec3f const& point) const override;

private:
    const Texture* const odd;
    const Texture* const even;
};

#endif
