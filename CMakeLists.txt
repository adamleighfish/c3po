cmake_minimum_required(VERSION 3.16)
project (c3po)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenEXR CONFIG REQUIRED)

add_executable(
	c3po 
	src/main.cpp 
	"src/aabb.cpp"
	"src/aggregate.cpp"
	"src/box.cpp" 
	"src/bvh.cpp"
	"src/camera.cpp"
	"src/hitable.cpp" 
	"src/material.cpp" 
	"src/perlin.cpp" 
	"src/rectangle.cpp" 
	"src/sphere.cpp" 
	"src/texture.cpp" 
	"src/transformation.cpp"
	"src/utility.cpp"
)

target_link_libraries(c3po PRIVATE OpenEXR::OpenEXR)
