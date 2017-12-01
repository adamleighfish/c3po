//
//  toytracer.cpp
//  toytracer
//
//  Created by Adam Fish on 11/2/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <ctime>
#include <future>

#include "omp.h"
#include "sphere.h"
#include "aggregate.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "rectangle.h"
#include "box.h"
#include "transformation.h"

using namespace std;

Hitable* CornellBox() {
    Hitable** list = new Hitable*[8];
    Material* red = new Lambertian(new ConstantTexture(Vec3f(0.65, 0.05, 0.05)));
    Material* white = new Lambertian(new ConstantTexture(Vec3f(0.73, 0.73, 0.73)));
    Material* green = new Lambertian(new ConstantTexture(Vec3f(0.12, 0.45, 0.15)));
    Material* light = new DiffuseLight(new ConstantTexture(Vec3f(15, 15, 15)));
    
    list[0] = new FlippedNormals(new RectYZ(0, 555, 0, 555, 555, red));
    list[1] = new RectYZ(0, 555, 0, 555, 0, green);
    list[2] = new RectXZ(213, 343, 227, 332, 554, light);
    list[3] = new RectXZ(0, 555, 0, 555, 0, white);
    list[4] = new FlippedNormals(new RectXZ(0, 555, 0, 555, 555, white));
    list[5] = new FlippedNormals(new RectXY(0, 555, 0, 555, 555, white));
    list[6] = new Translate(new RotateY(new Box(Vec3f(0, 0, 0), Vec3f(165, 165, 165), white), -18), Vec3f(130, 0, 65));
    list[7] = new Translate(new RotateY(new Box(Vec3f(0, 0, 0), Vec3f(165, 330, 165), white), 15), Vec3f(265, 0, 295));
    
    return new BVHNode(list, 8, 0, 0);
}

Vec3f Color(const Ray& R, Hitable* world, int depth) {
    HitRecord rec;
    if (world->Hit(R, 0.001, MAXFLOAT, rec)) {
        Ray Scattered;
        Vec3f Attentuation;
        Vec3f Emitted = rec.mat_ptr->Emitted(rec.u, rec.v, rec.P);
        if (depth < 50 and rec.mat_ptr->Scatter(R, rec, Attentuation, Scattered)) {
            return Emitted + Attentuation * Color(Scattered, world, depth + 1);
        }
        return Emitted;
    }
    return Vec3f(0, 0, 0);
}

Vec3f Sample(int x, int y, int nx, int ny, int ns, Camera& cam, Hitable* world) {
    Vec3f col;
    for (int s = 0; s < ns; ++s) {
        double u = double(x + drand48()) / double(nx);
        double v = double(y + drand48()) / double(ny);
        Ray R = cam.GetRay(u, v);
        col += Color(R, world, 0);
    }
    col /= ns;
    return Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
}

int main(int argc, const char** argv) {
    time_t begin = time(0);
    cout << "Building scene...\n";
    
    int nx = 800;
    int ny = 800;
    int ns = 10;
    
    Vec3f LookFrom(278, 278, -800);
    Vec3f LookAt(278, 278, 0);
    Vec3f Vup(0, 1, 0);
    double vfov = 40.0;
    double aspect = double(nx) / double(ny);
    double aperture = 0.0;
    double dist_to_focus = 10.0;
    double start_time = 0.0;
    double end_time = 1.0;
    
    Camera cam(LookFrom, LookAt, Vup, vfov, aspect, aperture, dist_to_focus, start_time, end_time);
    Hitable* world = CornellBox();
    
    time_t scene_time = time(0);
    cout << "Finished building scene: " << double(scene_time - begin) << " secs\n";
    
    int buff_size = nx * ny;
    vector<Vec3f> buffer(buff_size);
    
    cout << "Starting render:\n";
    time_t start_render = time(0);
    
    #pragma omp parallel for
    for (int i = 0; i < buff_size; ++i) {
        int col = i % nx;
        int row = ny - (i / nx) - 1;
        buffer[i] = Sample(col, row, nx, ny, ns, cam, world);
    }

    time_t end_render = time(0);
    cout << "Render time: " << double(end_render - start_render) << " secs\n";
    
    /**
     * Writing buffer to file
     */
    ofstream myfile;
    myfile.open("out.ppm");
    myfile << "P3\n" << nx << " " << ny << "\n255\n";
    for (auto i: buffer) {
        myfile << i << "\n";
    }
    myfile.close();
    
    time_t end = time(0);
    cout << "Total time: " << double(end - begin) << " secs\n";
    return 0;
}
