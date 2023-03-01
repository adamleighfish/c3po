# C-3PO
A physically-based C++ path tracer based off the path tracer outlined in Peter Shirley's In One Weekend series of books and Matt Pharr's Physically Based Rendering.

---

## Features

- Multithreaded, tiled based rendering
- Diffuse, metal, and glass materials
- Rectangles and spheres
- Perlin noise textures
- Camera depth of field
- Ray-object acceleration via BVH
- Object transformations
- Light emitting materials

---

## Sample Images

A comparison of different sample rates on an 800x800 Cornell Box render. Left image used 10 samples per pixel, middle used 100, and right used 1000. Render times were 1 second, 10 seconds, and 98 seconds respectively with an Intel i9-10900k.

![Sample rate comparison](https://i.imgur.com/2o21fUY.jpg)

---

# Building

I use vcpkg to manage dependecies so you'll need to make sure any third party libraries need are discoverable with CMake's find_package command. The current dependecies are:
 - OpenEXR

After your dependecies are good then you should be able to build this project like any standard CMake project.

---

## Future Features

- Denoiser 
- BSSRDF 
- Monte Carlo Sampling
- Triangles
- Multiple output image formats
