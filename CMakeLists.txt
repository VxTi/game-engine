cmake_minimum_required(VERSION 3.16)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# -----------------------
# Fetch GLM
# -----------------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.2
)
FetchContent_MakeAvailable(glm)

# -----------------------
# Fetch stb
# -----------------------
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_MakeAvailable(stb)

# stb has no CMake target, so expose its include folder manually
set(STB_INCLUDE_DIR ${stb_SOURCE_DIR})

# -----------------------
# Fetch GLFW
# -----------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# -----------------------
# Collect sources
# -----------------------
file(GLOB_RECURSE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

list(REMOVE_DUPLICATES PROJECT_SOURCES)

# Exclude build/_deps directory from sources 3
foreach(src ${PROJECT_SOURCES})
    string(FIND "${src}" "/build/" found)
    if (NOT found EQUAL -1)
        list(REMOVE_ITEM PROJECT_SOURCES "${src}")
    endif()
endforeach()


# Adds the PROJECT_ROOT macro to the compiler definitions
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

# -----------------------
# Include paths
# -----------------------
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include     # enables #include <engine/...>
    ${STB_INCLUDE_DIR}                      # stb headers
)

# glm and glfw provide CMake targets
target_link_libraries(${PROJECT_NAME} PRIVATE glm glfw)

# -----------------------
# macOS frameworks
# -----------------------
if (APPLE)
   target_link_libraries(${PROJECT_NAME} PRIVATE
       "-framework Cocoa"
       "-framework IOKit"
       "-framework OpenGL"
       "-framework CoreVideo"
   )
   target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
endif()
