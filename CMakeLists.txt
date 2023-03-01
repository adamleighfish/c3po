cmake_minimum_required(VERSION 3.16)
project (c3po)
find_package(OpenEXR CONFIG REQUIRED)

if (NOT OpenEXR_FOUND) 
	add_subdirectory(third_party/openexr)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(src)

target_link_libraries(c3po PRIVATE OpenEXR::OpenEXR)
