#include "aggregate.h"
#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "material.h"
#include "rectangle.h"
#include "sphere.h"
#include "texture.h"
#include "transformation.h"

#include <ImfRgba.h>
#include <ImfRgbaFile.h>

#include <chrono>
#include <execution>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

struct ImageTile {
    int const start_x;
    int const start_y;
    int const end_x;
    int const end_y;
    int const nx;
    int const ny;

    std::vector<Vec3>& buff;

    ImageTile(int x0, int x1, int y0, int y1, int nx, int ny,
              std::vector<Vec3>& buff)
        : start_x(x0), end_x(x1), start_y(y0), end_y(y1), nx(nx), ny(ny),
          buff(buff) {}
};

std::unique_ptr<Hitable> cornell_box() {
    std::vector<std::shared_ptr<Hitable>> list;

    auto red = std::make_shared<Lambertian>(
        std::make_shared<ConstantTexture>(Vec3(0.65, 0.05, 0.05)));
    auto white = std::make_shared<Lambertian>(
        std::make_shared<ConstantTexture>(Vec3(0.73, 0.73, 0.73)));
    auto green = std::make_shared<Lambertian>(
        std::make_shared<ConstantTexture>(Vec3(0.12, 0.45, 0.15)));
    auto light = std::make_shared<DiffuseLight>(
        std::make_shared<ConstantTexture>(Vec3(15, 15, 15)));

    list.push_back(std::make_shared<FlippedNormals>(
        std::make_shared<RectYZ>(0, 555, 0, 555, 555, red)));
    list.push_back(std::make_shared<RectYZ>(0, 555, 0, 555, 0, green));
    list.push_back(std::make_shared<RectXZ>(213, 343, 227, 332, 554, light));
    list.push_back(std::make_shared<RectXZ>(0, 555, 0, 555, 0, white));
    list.push_back(std::make_shared<FlippedNormals>(
        std::make_shared<RectXZ>(0, 555, 0, 555, 555, white)));
    list.push_back(std::make_shared<FlippedNormals>(
        std::make_shared<RectXY>(0, 555, 0, 555, 555, white)));
    list.push_back(std::make_shared<Translate>(
        std::make_unique<RotateY>(
            std::make_unique<Box>(Vec3(0, 0, 0), Vec3(165, 165, 165), white),
            -18),
        Vec3(130, 0, 65)));
    list.push_back(std::make_shared<Translate>(
        std::make_unique<RotateY>(
            std::make_unique<Box>(Vec3(0, 0, 0), Vec3(165, 330, 165), white),
            15),
        Vec3(265, 0, 295)));

    return make_unique<BVHNode>(list, 0, 0);
}

Vec3 sample(Ray const& R, Hitable* world, int depth) {
    HitRecord rec;
    if (world->hit(R, 0.001, std::numeric_limits<float>::max(), rec)) {
        Ray scattered;
        Vec3 attentuation;
        Vec3 emitted = rec.mat_ptr->emit(rec.u, rec.v, rec.point);
        if (depth < 50 &&
            rec.mat_ptr->scatter(R, rec, attentuation, scattered)) {
            return emitted + attentuation * sample(scattered, world, depth + 1);
        }
        return emitted;
    }
    return Vec3(0, 0, 0);
}

Vec3 render_pixels(int x, int y, int nx, int ny, int ns, Camera& cam,
                   Hitable* world) {
    Vec3 col;
    for (int s = 0; s < ns; ++s) {
        double u = double(x + rand_double(0.0, 1.0)) / double(nx);
        double v = double(y + rand_double(0.0, 1.0)) / double(ny);
        Ray R = cam.gen_ray(u, v);
        col += sample(R, world, 0);
    }
    col /= ns;
    return Vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));
}

void render_tile(ImageTile& a, Camera& cam, Hitable* world, int ns) {
    for (int row = a.start_y; row <= a.end_y; ++row) {
        for (int col = a.start_x; col <= a.end_x; ++col) {
            int index = a.nx * (a.ny - row - 1) + col;
            a.buff[index] = render_pixels(col, row, a.nx, a.ny, ns, cam, world);
        }
    }
}

int main(int argc, char const* argv[]) {
    int nx = 800;
    int ny = 800;
    int ns = 1000;
    int tile_size = 32;

    Vec3 look_from(278, 278, -800);
    Vec3 look_at(278, 278, 0);
    Vec3 v_up(0, 1, 0);
    double v_fov = 40.0;
    double aspect = double(nx) / double(ny);
    double aperture = 0.0;
    double dist_to_focus = 10.0;
    double start_time = 0.0;
    double end_time = 1.0;

    Camera cam(look_from, look_at, v_up, v_fov, aspect, aperture, dist_to_focus,
               start_time, end_time);
    std::unique_ptr<Hitable> world = cornell_box();

    // Divide the image into tiles for rendering
    std::cout << "Starting setup:\n";
    auto setup_start = std::chrono::high_resolution_clock::now();

    int buff_size = nx * ny;
    std::vector<Vec3> buffer(buff_size);
    std::vector<ImageTile> tiles;

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

    auto setup_end = std::chrono::high_resolution_clock::now();
    std::cout << "Setup complete: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     setup_end - setup_start)
                     .count()
              << " ms\n";

    // Render the tiles in parallel
    std::cout << "Starting render:\n";

    auto render_start = std::chrono::high_resolution_clock::now();

    std::for_each(
        std::execution::par, tiles.begin(), tiles.end(),
        [&](ImageTile tile) { render_tile(tile, cam, world.get(), ns); });

    auto render_end = std::chrono::high_resolution_clock::now();
    std::cout << "Render complete: "
              << std::chrono::duration_cast<std::chrono::seconds>(render_end -
                                                                  render_start)
                     .count()
              << " s\n";

    std::vector<Imf::Rgba> pixels;
    for (Vec3 const& v : buffer) {
        pixels.push_back({ half(v.r()), half(v.g()), half(v.b()), 0.0});
    }

    try {
        Imf::RgbaOutputFile file("output.exr", nx, ny, Imf::WRITE_RGB);
        file.setFrameBuffer(pixels.data(), 1, nx);
        file.writePixels(ny);
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
