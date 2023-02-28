#include "texture.h"

ConstantTexture::ConstantTexture(Vec3 const& color) : color(color) {}

Vec3 ConstantTexture::sample(double u, double v, Vec3 const& point) const {
    return color;
};

CheckerTexture::CheckerTexture(Texture* odd, Texture* even)
    : odd(odd), even(even) {}

Vec3 CheckerTexture::sample(double u, double v, Vec3 const& point) const {
    double sines = sin(10 * point.x) * sin(10 * point.y) * sin(10 * point.z);
    if (sines < 0.0) {
        return odd->sample(u, v, point);
    } else {
        return even->sample(u, v, point);
    }
}

NoiseTexture::NoiseTexture(double scale) : perlin(), scale(scale) {}

Vec3 NoiseTexture::sample(double u, double v, Vec3 const& point) const {
    return Vec3(1, 1, 1) * 0.5 *
           (1 + sin(scale * point.z + 10 * perlin.turb(point)));
}
