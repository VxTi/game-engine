cmake_minimum_required(VERSION 3.3)

project(graphics_test)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo")
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/libraries)

add_executable(graphics_test src/main.cpp src/rendering/VBO.cpp src/rendering/Shader.cpp src/io/Files.cpp src/io/Files.h src/rendering/Renderer.cpp src/rendering/Rendering.h src/world/entity/Entity.cpp src/world/entity/Entity.h src/world/entity/Player.cpp src/world/SimplexNoise.cpp src/world/SimplexNoise.h src/world/WorldObject.cpp src/world/WorldObject.h src/rendering/font/DrawableFont.h src/rendering/font/TrueType.cpp src/world/entity/Player.h src/world/World.h src/world/World.cpp
        src/rendering/Frustum.cpp
        src/rendering/model/Model.cpp
        src/rendering/model/Model.h
        src/rendering/VBO.h
        src/rendering/Texture.cpp
        src/rendering/Texture.h
        include/stb/stb_image.h
        src/rendering/Shader.h
        src/rendering/model/Mesh.cpp
        src/rendering/model/Mesh.h
        src/math/QuadTree.h
        src/math/QuadTree.cpp
)

target_link_libraries(graphics_test ${PROJECT_SOURCE_DIR}/libraries/libglfw3.a)
