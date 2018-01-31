# C-3PO
A physically-based C++ path tracer based off the path tracer outlined in Peter Shirley's In One Weekend series of books and Matt Pharr's Physically Based Rendering.

---

## Features

- Multithreaded, tiled based rendering using OpenMP for parallelization
- Diffuse, metal, and glass materials
- Rectangles and spheres
- Perlin noise textures
- Camera depth of field
- Ray-object acceleration via BVH
- Object transformations
- Light emitting materials

---

## Sample Images

A comparison of different sample rates on an 800x800 Cornell Box render. Left image used 10 samples per pixel, middle used 100, and right used 1000. Render times were 30 seconds, 6 mintues, and 1 hour respectively.

![Sample rate comparison](https://i.imgur.com/2o21fUY.jpg)

---


## Future Features

- Denoiser 
- BSSRDF 
- Monte Carlo Sampling
- Triangles
- Multiple output image formats
