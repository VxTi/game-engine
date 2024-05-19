cmake_minimum_required(VERSION 3.3)

project(graphics_test)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo")
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/libraries)

add_executable(graphics_test src/main.cpp src/rendering/VBO.cpp src/rendering/Shader.cpp src/io/Files.cpp src/io/Files.h src/rendering/Renderer.cpp src/rendering/Rendering.h src/world/entity/AbstractBody.cpp src/world/entity/Entity.h src/world/entity/Player.cpp src/world/SimplexNoise.cpp src/world/SimplexNoise.h src/world/WorldObject.cpp src/world/WorldObject.h src/rendering/Drawable.h)

target_link_libraries(graphics_test ${PROJECT_SOURCE_DIR}/libraries/libglfw3.a)
