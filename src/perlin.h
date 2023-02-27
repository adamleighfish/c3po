#ifndef perlin_h
#define perlin_h

#include <random>

#include "geometry.h"

inline double PerlinInterp(Vec3f C[2][2][2], double u, double v, double w) {
    double accum = 0.0;
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                Vec3f WeightV(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         Dot(C[i][j][k], WeightV);
            }
        }
    }
    return accum;
}

class Perlin {
public:
    static int* perm_x;
    static int* perm_y;
    static int* perm_z;
    static Vec3f* ranvec;

    double Noise(Vec3f const& P) const {
        double u = P.x - floor(P.x);
        double v = P.y - floor(P.y);
        double w = P.z - floor(P.z);
        int i = floor(P.x);
        int j = floor(P.y);
        int k = floor(P.z);
        Vec3f C[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    C[di][dj][dk] =
                        ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
                               perm_z[(k + dk) & 255]];
                }
            }
        }
        return PerlinInterp(C, u, v, w);
    }

    double Turb(Vec3f const& P, int depth = 7) const {
        double accum = 0.0;
        Vec3f TempP = P;
        double weight = 1.0;
        for (int i = 0; i < depth; ++i) {
            accum += weight * Noise(TempP);
            weight *= 0.5;
            TempP *= 2;
        }
        return abs(accum);
    }
};

static Vec3f* PerlinGenerate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    Vec3f* P = new Vec3f[256];
    for (int i = 0; i < 256; ++i) {
        P[i] = Normalize(Vec3f(-1.0 + 2 * dis(gen), -1.0 + 2 * dis(gen),
                               -1.0 + 2 * dis(gen)));
    }
    return P;
}

void Permute(int* p, int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = n - 1; i >= 0; --i) {
        int target = int(dis(gen) * (i + 1));
        int temp = p[i];
        p[i] = p[target];
        p[target] = temp;
    }
}

static int* PerlinGeneratePerm() {
    int* p = new int[256];
    for (int i = 0; i < 256; ++i) {
        p[i] = i;
    }
    Permute(p, 256);
    return p;
}

int* Perlin::perm_x = PerlinGeneratePerm();
int* Perlin::perm_y = PerlinGeneratePerm();
int* Perlin::perm_z = PerlinGeneratePerm();
Vec3f* Perlin::ranvec = PerlinGenerate();

#endif
