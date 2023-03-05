cmake_minimum_required(VERSION 3.16)
project (c3po)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(
	c3po 
	src/main.cpp 
	"src/bvh.cpp"
	"src/geometry.cpp"
	"src/interaction.cpp"
	"src/shape.cpp" 
	"src/sphere.cpp" 
	"src/transform.cpp"
	"src/utility.cpp"
	"src/loader.cpp"
	"src/primitive.cpp"
)

find_package(OpenEXR CONFIG REQUIRED)
target_link_libraries(c3po PRIVATE OpenEXR::OpenEXR)

find_package(tinyobjloader CONFIG REQUIRED)
target_link_libraries(c3po PRIVATE tinyobjloader::tinyobjloader)
