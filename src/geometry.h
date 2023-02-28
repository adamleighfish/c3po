#ifndef geometry_h
#define geometry_h

#include <iostream>

#include "utility.h"

class Vec3 {
public:
    double x, y, z;

    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    double const r() const { return x; }
    double const g() const { return y; }
    double const b() const { return z; }

    Vec3 const& operator+() const { return *this; }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

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

    Vec3 operator+(Vec3 const& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3& operator+=(Vec3 const& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3 operator+(double t) const { return Vec3(x + y, y + t, z + t); }
    Vec3& operator+=(double t) {
        x += t;
        y += t;
        z += t;
        return *this;
    }

    Vec3 operator-(Vec3 const& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3& operator-=(Vec3 const& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3 operator-(double t) const { return Vec3(x - t, y - t, z - t); }
    Vec3& operator-=(double t) {
        x -= t;
        y -= t;
        z -= t;
        return *this;
    }

    Vec3 operator*(Vec3 const& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    Vec3& operator*=(Vec3 const& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vec3 operator*(double t) const { return Vec3(x * t, y * t, z * t); }
    Vec3& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec3 operator/(double t) const { return Vec3(x / t, y / t, z / t); }
    Vec3& operator/=(double t) {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }

    void operator=(Vec3 const& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    bool operator==(Vec3 const& v) const {
        return x == v.x and y == v.y and z == v.z;
    }
    bool operator!=(Vec3 const& v) const {
        return x != v.x or y != v.y or z != v.z;
    }

    double len_squared() const { return x * x + y * y + z * z; }
    double len() const { return std::sqrt(len_squared()); }
};

inline Vec3 operator*(double t, Vec3 const& v) { return v * t; }

inline double dot(Vec3 const& v1, Vec3 const& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vec3 cross(Vec3 const& v1, Vec3 const& v2) {
    return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}

inline Vec3 norm(Vec3 const& v) { return v / v.len(); }

inline Vec3 lerp(Vec3 const& v1, Vec3 const& v2, double t) {
    return (1.0 - t) * v1 + t * v2;
}

inline std::istream& operator>>(std::istream& is, Vec3& v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, Vec3 const& v) {
    os << RoundAndClamp(v.x) << " " << RoundAndClamp(v.y) << " "
       << RoundAndClamp(v.z);
    return os;
}

class Ray {
public:
    Vec3 origin;
    Vec3 dir;
    double time;

    Ray() : time(0.0){};
    explicit Ray(Vec3 const& origin, Vec3 const& dir, double time = 0.0)
        : origin(origin), dir(dir), time(time) {}


    Vec3 operator()(double t) const { return origin + dir * t; }
};

#endif
