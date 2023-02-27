//
//  geometry.h
//  c3po
//
//  Created by Adam Fish on 11/2/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef geometry_h
#define geometry_h

#include <iostream>

#include "utility.h"

class Vec3f {
public:
    double x, y, z;

    Vec3f() : x(0.0), y(0.0), z(0.0) {}
    Vec3f(double x, double y, double z) : x(x), y(y), z(z) {}

    double const r() const { return x; }
    double const g() const { return y; }
    double const b() const { return z; }

    Vec3f const& operator+() const { return *this; }
    Vec3f operator-() const { return Vec3f(-x, -y, -z); }

    double operator[](int i) const {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }

    void SetValue(int i, double val) {
        if (i == 0) {
            x = val;
        } else if (i == 1) {
            y = val;
        } else {
            z = val;
        }
    }

    Vec3f operator+(Vec3f const& v) const {
        return Vec3f(x + v.x, y + v.y, z + v.z);
    }
    Vec3f& operator+=(Vec3f const& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3f operator+(double t) const { return Vec3f(x + y, y + t, z + t); }
    Vec3f& operator+=(double t) {
        x += t;
        y += t;
        z += t;
        return *this;
    }

    Vec3f operator-(Vec3f const& v) const {
        return Vec3f(x - v.x, y - v.y, z - v.z);
    }
    Vec3f& operator-=(Vec3f const& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3f operator-(double t) const { return Vec3f(x - t, y - t, z - t); }
    Vec3f& operator-=(double t) {
        x -= t;
        y -= t;
        z -= t;
        return *this;
    }

    Vec3f operator*(Vec3f const& v) const {
        return Vec3f(x * v.x, y * v.y, z * v.z);
    }
    Vec3f& operator*=(Vec3f const& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vec3f operator*(double t) const { return Vec3f(x * t, y * t, z * t); }
    Vec3f& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec3f operator/(double t) const { return Vec3f(x / t, y / t, z / t); }
    Vec3f& operator/=(double t) {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }

    void operator=(Vec3f const& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    bool operator==(Vec3f const& v) const {
        return x == v.x and y == v.y and z == v.z;
    }
    bool operator!=(Vec3f const& v) const {
        return x != v.x or y != v.y or z != v.z;
    }

    double LengthSquared() const { return x * x + y * y + z * z; }
    double Length() const { return std::sqrt(LengthSquared()); }
};

inline Vec3f operator*(double t, Vec3f const& v) { return v * t; }

inline double Dot(Vec3f const& v1, Vec3f const& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vec3f Cross(Vec3f const& v1, Vec3f const& v2) {
    return Vec3f(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                 v1.x * v2.y - v1.y * v2.x);
}

inline Vec3f Normalize(Vec3f const& v) { return v / v.Length(); }

inline Vec3f Lerp(Vec3f const& v1, Vec3f const& v2, double t) {
    return (1.0 - t) * v1 + t * v2;
}

inline std::istream& operator>>(std::istream& is, Vec3f& v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, Vec3f const& v) {
    os << RoundAndClamp(v.x) << " " << RoundAndClamp(v.y) << " "
       << RoundAndClamp(v.z);
    return os;
}

class Ray {
public:
    Vec3f O;
    Vec3f D;
    double time;

    Ray(){};
    Ray(Vec3f const& O, Vec3f const& D, double time = 0.0)
        : O(O), D(D), time(time) {}

    Vec3f operator()(double t) const { return O + D * t; }
};

#endif /* geometry_h */
