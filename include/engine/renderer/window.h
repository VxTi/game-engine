#ifndef ENGINE_RENDERER_WINDOW_H
#define ENGINE_RENDERER_WINDOW_H

#include "GLFW/glfw3.h"
#include <OpenGL/gltypes.h>

typedef struct {
  GLsizei width;
  GLsizei height;
  const char *title;
  GLFWwindow *glfwWindow;
} Window;

#endif // ENGINE_RENDERER_WINDOW_H
