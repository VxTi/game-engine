cmake_minimum_required(VERSION 3.3)

project(graphics_test)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo")
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/libraries)

add_executable(graphics_test src/main.cpp
        src/rendering/vbo.cpp
        src/rendering/shader.cpp
        src/io/Files.cpp src/io/Files.h
        src/rendering/renderer.cpp
        src/rendering/renderer.h
        src/world/entity/entity.cpp
        src/world/entity/entity.h
        src/world/entity/player.cpp
        src/world/noise.cpp
        src/world/noise.h
        src/rendering/font/DrawableFont.h
        src/rendering/font/TrueType.cpp
        src/world/entity/player.h
        src/world/world.h
        src/world/world.cpp
        src/rendering/culling/frustum.cpp
        src/rendering/model/model.cpp
        src/rendering/model/model.h
        src/rendering/vbo.h
        src/rendering/texture.cpp
        src/rendering/texture.h
        include/stb/stb_image.h
        src/rendering/shader.h
        src/rendering/model/mesh.cpp
        src/rendering/model/mesh.h
        src/math/OcTree.cpp
        src/math/OcTree.h
        src/rendering/culling/frustum.h
        src/math/transformation.h
        include/stb/stb_image.h
        src/rendering/culling/culling.h
        src/rendering/culling/occlusion.hpp
)

target_link_libraries(graphics_test ${PROJECT_SOURCE_DIR}/libraries/libglfw3.a)
