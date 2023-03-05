#include "bvh.h"
#include "interaction.h"
#include "primitive.h"
#include "sphere.h"
#include "transform.h"
#include "utility.h"

#include "ImfRgba.h"
#include "ImfRgbaFile.h"

#include "Imath/ImathColor.h"

#include <chrono>
#include <iostream>
#include <numbers>
#include <vector>

using Color3f = Imath::Color3f;

using namespace c3po;

Color3f ray_color(Ray const& r, Primitive const& scene) {
    SurfaceInteraction isect;
    if (scene.intersect(r, isect)) {
        return 0.5f *
               Color3f(isect.n.x + 1.f, isect.n.y + 1.f, isect.n.z + 1.f);
    }

    Vec3f const unit_dir = r.dir.normalized();
    float const t = 0.5f * (unit_dir.y + 1.f);
    return (1.f - t) * Color3f(1.f) + t * Color3f(0.5f, 0.7f, 1.f);
}

std::unique_ptr<BVH> make_scene() {
    std::vector<std::shared_ptr<Primitive>> prims;

    Vec3f delta{0.f, -100.5f, -1.f};
    {
        auto const obj_to_world = std::make_shared<Transform>();
        obj_to_world->translate(delta);
        auto const world_to_obj = std::make_shared<Transform>();
        world_to_obj->translate(-delta);
        auto const prim = std::make_shared<GeometricPrimitive>(
            std::make_shared<Sphere>(obj_to_world, world_to_obj, false, 100.f),
            nullptr);
        prims.push_back(prim);
    }
    delta = {0.f, 0.f, -1.f};
    {
        auto const obj_to_world = std::make_shared<Transform>();
        obj_to_world->translate(delta);
        auto const world_to_obj = std::make_shared<Transform>();
        world_to_obj->translate(-delta);
        auto const prim = std::make_shared<GeometricPrimitive>(
            std::make_shared<Sphere>(obj_to_world, world_to_obj, false, .5f),
            nullptr);
        prims.push_back(prim);
    }
    delta = {-1.f, 0.f, -1.f};
    {
        auto const obj_to_world = std::make_shared<Transform>();
        obj_to_world->translate(delta);
        auto const world_to_obj = std::make_shared<Transform>();
        world_to_obj->translate(-delta);
        auto const prim = std::make_shared<GeometricPrimitive>(
            std::make_shared<Sphere>(obj_to_world, world_to_obj, false, .5f),
            nullptr);
        prims.push_back(prim);
    }
    delta = {1.f, 0.f, -1.f};
    {
        auto const obj_to_world = std::make_shared<Transform>();
        obj_to_world->translate(delta);
        auto const world_to_obj = std::make_shared<Transform>();
        world_to_obj->translate(-delta);
        auto const prim = std::make_shared<GeometricPrimitive>(
            std::make_shared<Sphere>(obj_to_world, world_to_obj, false, .5f),
            nullptr);
        prims.push_back(prim);
    }

    return std::make_unique<BVH>(prims, 255);
}

int main(int argc, char const* argv[]) {

    // image
    float const aspect_ratio = 16.f / 9.f;
    int const image_width = 1280;
    int const image_height = int(image_width / aspect_ratio);

    std::cout << "Building BVH:\n";
    auto build_start = std::chrono::high_resolution_clock::now();

    std::unique_ptr<Aggregate> world = make_scene();

    auto build_end = std::chrono::high_resolution_clock::now();
    std::cout << "Complete! Took: "
              << std::chrono::duration_cast<std::chrono::seconds>(build_end -
                                                                  build_start)
                     .count()
              << " s\n";

    // camera
    float const viewport_height = 2.f;
    float const viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.f;

    Point3f const origin{0.f};
    Vec3f const horizontal{viewport_width, 0.f, 0.f};
    Vec3f const vertical{0.f, viewport_height, 0.f};
    Point3f const lower_left_corner =
        origin - horizontal / 2.f - vertical / 2.f - Vec3f(0.f, 0.f, focal_length);

    std::cout << "Starting render:\n";
    auto render_start = std::chrono::high_resolution_clock::now();

    std::vector<Color3f> buffer(image_width * image_height);
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            float const u = float(i) / (image_width - 1);
            float const v = float(j) / (image_height - 1);

            Ray const r(origin, lower_left_corner + u * horizontal +
                                    v * vertical - origin);
            int const index = image_width * (image_height - j - 1) + i;
            buffer[index] = ray_color(r, *world);
        }
    }

    auto render_end = std::chrono::high_resolution_clock::now();
    std::cout << "Complete! took: "
              << std::chrono::duration_cast<std::chrono::seconds>(render_end -
                                                                  render_start)
                     .count()
              << " s\n";

    std::vector<Imf::Rgba> pixels;
    for (auto const& p : buffer) {
        pixels.push_back({p.x, p.y, p.z, 0.0});
    }

    try {
        Imf::RgbaOutputFile file("output.exr", image_width, image_height,
                                 Imf::WRITE_RGB);
        file.setFrameBuffer(pixels.data(), 1, image_width);
        file.writePixels(image_height);
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
