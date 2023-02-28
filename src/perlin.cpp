#include "perlin.h"

#include "utility.h"

void permute(int* p, int n) {
    for (int i = n - 1; i >= 0; --i) {
        int target = int(rand_double(0.0, 1.0) * (i + 1));
        int temp = p[i];
        p[i] = p[target];
        p[target] = temp;
    }
}

static int* perlin_generate_perm() {
    int* p = new int[256];
    for (int i = 0; i < 256; ++i) {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

static Vec3* perlin_generate() {
    Vec3* point = new Vec3[256];
    for (int i = 0; i < 256; ++i) {
        point[i] = norm(Vec3(-1.0 + 2 * rand_double(0.0, 1.0),
                             -1.0 + 2 * rand_double(0.0, 1.0),
                             -1.0 + 2 * rand_double(0.0, 1.0)));
    }
    return point;
}

int* Perlin::perm_x = perlin_generate_perm();
int* Perlin::perm_y = perlin_generate_perm();
int* Perlin::perm_z = perlin_generate_perm();
Vec3* Perlin::ranvec = perlin_generate();

double perlin_interp(Vec3 c[2][2][2], double u, double v, double w) {
    double accum = 0.0;
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                Vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         dot(c[i][j][k], weight_v);
            }
        }
    }
    return accum;
}

double Perlin::noise(Vec3 const& point) const {
    double u = point.x - floor(point.x);
    double v = point.y - floor(point.y);
    double w = point.z - floor(point.z);
    int i = floor(point.x);
    int j = floor(point.y);
    int k = floor(point.z);
    Vec3 c[2][2][2];
    for (int di = 0; di < 2; ++di) {
        for (int dj = 0; dj < 2; ++dj) {
            for (int dk = 0; dk < 2; ++dk) {
                c[di][dj][dk] =
                    ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
                           perm_z[(k + dk) & 255]];
            }
        }
    }
    return perlin_interp(c, u, v, w);
}

double Perlin::turb(Vec3 const& point, int depth) const {
    double accum = 0.0;
    Vec3 temp = point;
    double weight = 1.0;
    for (int i = 0; i < depth; ++i) {
        accum += weight * noise(temp);
        weight *= 0.5;
        temp *= 2;
    }
    return abs(accum);
}
