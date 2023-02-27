#include <chrono>
#include <execution>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include "aggregate.h"
#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "material.h"
#include "rectangle.h"
#include "sphere.h"
#include "transformation.h"

using namespace std;

struct ImageTile {
    int const start_x;
    int const start_y;
    int const end_x;
    int const end_y;
    int const nx;
    int const ny;

    std::vector<Vec3f>& buff;

    ImageTile(int x0, int x1, int y0, int y1, int nx, int ny,
              std::vector<Vec3f>& buff)
        : start_x(x0), end_x(x1), start_y(y0), end_y(y1), nx(nx), ny(ny),
          buff(buff) {}
};

Hitable* CornellBox() {
    Hitable** list = new Hitable*[8];
    Material* red =
        new Lambertian(new ConstantTexture(Vec3f(0.65, 0.05, 0.05)));
    Material* white =
        new Lambertian(new ConstantTexture(Vec3f(0.73, 0.73, 0.73)));
    Material* green =
        new Lambertian(new ConstantTexture(Vec3f(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vec3f(15, 15, 15)));

    list[0] = new FlippedNormals(new RectYZ(0, 555, 0, 555, 555, red));
    list[1] = new RectYZ(0, 555, 0, 555, 0, green);
    list[2] = new RectXZ(213, 343, 227, 332, 554, light);
    list[3] = new RectXZ(0, 555, 0, 555, 0, white);
    list[4] = new FlippedNormals(new RectXZ(0, 555, 0, 555, 555, white));
    list[5] = new FlippedNormals(new RectXY(0, 555, 0, 555, 555, white));
    list[6] = new Translate(
        new RotateY(new Box(Vec3f(0, 0, 0), Vec3f(165, 165, 165), white), -18),
        Vec3f(130, 0, 65));
    list[7] = new Translate(
        new RotateY(new Box(Vec3f(0, 0, 0), Vec3f(165, 330, 165), white), 15),
        Vec3f(265, 0, 295));

    return new BVHNode(list, 8, 0, 0);
}

Vec3f Sample(Ray const& R, Hitable* world, int depth) {
    HitRecord rec;
    if (world->Hit(R, 0.001, std::numeric_limits<float>::max(), rec)) {
        Ray Scattered;
        Vec3f Attentuation;
        Vec3f Emitted = rec.mat_ptr->Emitted(rec.u, rec.v, rec.P);
        if (depth < 50 &&
            rec.mat_ptr->Scatter(R, rec, Attentuation, Scattered)) {
            return Emitted + Attentuation * Sample(Scattered, world, depth + 1);
        }
        return Emitted;
    }
    return Vec3f(0, 0, 0);
}

Vec3f RenderPixel(int x, int y, int nx, int ny, int ns, Camera& cam,
                  Hitable* world) {
    Vec3f col;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int s = 0; s < ns; ++s) {
        double u = double(x + dis(gen)) / double(nx);
        double v = double(y + dis(gen)) / double(ny);
        Ray R = cam.GetRay(u, v);
        col += Sample(R, world, 0);
    }
    col /= ns;
    return Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
}

void RenderTile(ImageTile& a, Camera& cam, Hitable* world, int ns) {
    for (int row = a.start_y; row <= a.end_y; ++row) {
        for (int col = a.start_x; col <= a.end_x; ++col) {
            int index = a.nx * (a.ny - row - 1) + col;
            a.buff[index] = RenderPixel(col, row, a.nx, a.ny, ns, cam, world);
        }
    }
}

int main(int argc, char const* argv[]) {
    int nx = 800;
    int ny = 800;
    int ns = 10;
    int tile_size = 32;

    Vec3f LookFrom(278, 278, -800);
    Vec3f LookAt(278, 278, 0);
    Vec3f Vup(0, 1, 0);
    double vfov = 40.0;
    double aspect = double(nx) / double(ny);
    double aperture = 0.0;
    double dist_to_focus = 10.0;
    double start_time = 0.0;
    double end_time = 1.0;

    Camera cam(LookFrom, LookAt, Vup, vfov, aspect, aperture, dist_to_focus,
               start_time, end_time);
    Hitable* world = CornellBox();

    /**
     * Divide the image into tiles for rendering
     */
    cout << "Starting setup:\n";
    auto setup_start = chrono::high_resolution_clock::now();

    int buff_size = nx * ny;
    vector<Vec3f> buffer(buff_size);
    vector<ImageTile> tiles;

    int tiles_wide = ceil(double(nx) / double(tile_size));
    int tiles_tall = ceil(double(ny) / double(tile_size));

    for (int j = 0; j < tiles_tall; ++j) {
        for (int i = 0; i < tiles_wide; ++i) {
            int start_x = i * tile_size;
            int start_y = j * tile_size;
            int end_x = start_x + tile_size - 1;
            int end_y = start_y + tile_size - 1;

            if (end_x >= nx) {
                end_x = nx - 1;
            }

            if (end_y >= ny) {
                end_y = ny - 1;
            }

            ImageTile s(start_x, end_x, start_y, end_y, nx, ny, buffer);
            tiles.push_back(s);
        }
    }

    auto setup_end = chrono::high_resolution_clock::now();
    cout << "Setup complete: "
         << std::chrono::duration_cast<chrono::milliseconds>(setup_end -
                                                             setup_start)
                .count()
         << " ms\n";

    /**
     * Render the tiles in parallel
     */
    cout << "Starting render:\n";

    auto render_start = chrono::high_resolution_clock::now();

    std::for_each(std::execution::par, tiles.begin(), tiles.end(),
                  [&](ImageTile tile) { RenderTile(tile, cam, world, ns); });

    auto render_end = chrono::high_resolution_clock::now();
    cout << "Render complete: "
         << std::chrono::duration_cast<chrono::seconds>(render_end -
                                                        render_start)
                .count()
         << " s\n";

    /**
     * Writing buffer to file
     */
    ofstream myfile;
    myfile.open("out.ppm");
    myfile << "P3\n" << nx << " " << ny << "\n255\n";
    for (auto i : buffer) {
        myfile << i << "\n";
    }
    myfile.close();

    return 0;
}
