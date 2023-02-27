//
//  texture.h
//  c3p0
//
//  Created by Adam Fish on 11/7/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include "geometry.h"
#include "perlin.h"

class Texture {
public:
    virtual Vec3f Value(double u, double v, Vec3f const& P) const = 0;
};

/**
 *
 *
 */

class ConstantTexture : public Texture {
public:
    Vec3f Color;

    ConstantTexture() {}
    ConstantTexture(Vec3f const& Color) : Color(Color) {}

    virtual Vec3f Value(double u, double v, Vec3f const& P) const {
        return Color;
    };
};

/**
 *
 *
 */

class CheckerTexture : public Texture {
public:
    Texture* Odd;
    Texture* Even;

    CheckerTexture() {}
    CheckerTexture(Texture* Odd, Texture* Even) : Odd(Odd), Even(Even) {}

    virtual Vec3f Value(double u, double v, Vec3f const& P) const {
        double sines = sin(10 * P.x) * sin(10 * P.y) * sin(10 * P.z);
        if (sines < 0.0) {
            return Odd->Value(u, v, P);
        } else {
            return Even->Value(u, v, P);
        }
    }
};

/**
 *
 *
 */

class NoiseTexture : public Texture {
public:
    Perlin perlin;
    double scale;

    NoiseTexture() {}
    NoiseTexture(double scale) : scale(scale) {}

    virtual Vec3f Value(double u, double v, Vec3f const& P) const {
        return Vec3f(1, 1, 1) * 0.5 *
               (1 + sin(scale * P.z + 10 * perlin.Turb(P)));
    }
};

#endif /* texture_h */
