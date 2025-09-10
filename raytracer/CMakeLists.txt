cmake_minimum_required(VERSION 3.29)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(LIBOMP_PREFIX /opt/homebrew/opt/libomp)

include_directories(acceleration)
include_directories(build)
include_directories(cameras)
include_directories(geometry)
include_directories(happly-master)
include_directories(lenses)
include_directories(lights)
include_directories(materials)
include_directories(samplers)
include_directories(utilities)
include_directories(world)

add_executable(raytracer
        acceleration/Accelerator.cpp
        acceleration/Accelerator.hpp
        build/build.cpp
        cameras/Camera.hpp
        cameras/Parallel.cpp
        cameras/Parallel.hpp
        cameras/Perspective.cpp
        cameras/Perspective.hpp
        geometry/Geometry.cpp
        geometry/Geometry.hpp
        geometry/Plane.cpp
        geometry/Plane.hpp
        geometry/Sphere.cpp
        geometry/Sphere.hpp
        geometry/Triangle.cpp
        geometry/Triangle.hpp
        happly-master/happly.h
        lenses/Lens.cpp
        lenses/Lens.hpp
        lights/Light.cpp
        lights/Light.hpp
        materials/Cosine.cpp
        materials/Cosine.hpp
        materials/Glossy.cpp
        materials/Glossy.hpp
        materials/Material.hpp
        materials/Matte.cpp
        materials/Matte.hpp
        materials/Wall.cpp
        materials/Wall.hpp
        samplers/Sampler.cpp
        samplers/Sampler.hpp
        samplers/Simple.cpp
        samplers/Simple.hpp
        utilities/BBox.cpp
        utilities/BBox.hpp
        utilities/Constants.hpp
        utilities/helpers.cpp
        utilities/helpers.hpp
        utilities/Image.cpp
        utilities/Image.hpp
        utilities/Point3D.cpp
        utilities/Point3D.hpp
        utilities/Ray.cpp
        utilities/Ray.hpp
        utilities/RGBColor.cpp
        utilities/RGBColor.hpp
        utilities/ShadeInfo.cpp
        utilities/ShadeInfo.hpp
        utilities/SimulationParameters.hpp
        utilities/Vector3D.cpp
        utilities/Vector3D.hpp
        world/ViewPlane.cpp
        world/ViewPlane.hpp
        world/World.cpp
        world/World.hpp
        #    ply_parser.cpp
        raytracer.cpp
        utilities/Utils.cpp
        utilities/Utils.h
        utilities/FileReader.h
        utilities/FileReader.cpp
        utilities/FileParser.h
        utilities/FileParser.cpp)

target_compile_options(raytracer PRIVATE
        -Xpreprocessor -fopenmp
        -I${LIBOMP_PREFIX}/include
)

target_include_directories(raytracer PRIVATE /opt/homebrew/opt/libomp/include)
target_link_directories(raytracer PRIVATE /opt/homebrew/opt/libomp/lib)
target_link_libraries(raytracer PRIVATE
        ${LIBOMP_PREFIX}/lib/libomp.dylib
)