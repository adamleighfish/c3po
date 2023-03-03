#include "texture.h"

ConstantTexture::ConstantTexture(Vec3f const& color) : color(color) {}

Vec3f ConstantTexture::sample(float u, float v, Vec3f const& point) const {
    return color;
};

CheckerTexture::CheckerTexture(Texture* odd, Texture* even)
    : odd(odd), even(even) {}

Vec3f CheckerTexture::sample(float u, float v, Vec3f const& point) const {
    float sines = sin(10 * point.x) * sin(10 * point.y) * sin(10 * point.z);
    if (sines < 0.0) {
        return odd->sample(u, v, point);
    } else {
        return even->sample(u, v, point);
    }
}

