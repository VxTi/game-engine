#include "GLFW/glfw3.h"
#include <engine/renderer/culling/frustum.h>
#include <engine/renderer/scene.h>
#include <engine/renderer/shader.h>
#include <engine/renderer/window.h>
#include <engine/world/entity/player.h>
#include <engine/world/world.h>
#include <iostream>

#define VSYNC_ENABLED 1

bool wireframe = false;

const glm::vec2 scrollFactor = glm::vec2(1.f, 1.f);

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);

/**
 * Main function of the application.
 */
int main() {
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    exit(1);
  }

  Window window = {
      .width = 1200,
      .height = 700,
      .title = "Window Test",
  };

  auto observer = Player();
  auto world = World();

  observer.frictionConstant = 10.0f;
  observer.position = vec3(10, 10, 10);

  glfwSetErrorCallback([](int error, const char *description) {
    std::cout << "GLFW Error " << error << " - " << description << std::endl;
  });
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window.glfwWindow =
      glfwCreateWindow(window.width, window.height, window.title, NULL, NULL);

  if (!window.glfwWindow) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    exit(1);
  }
  glfwSetWindowUserPointer(window.glfwWindow, &window);
  glfwSetFramebufferSizeCallback(window.glfwWindow, [](GLFWwindow *_windowPtr,
                                                       int width, int height) {
    glViewport(0, 0, width, height);
    auto window = static_cast<Window *>(glfwGetWindowUserPointer(_windowPtr));

    if (window) {
      window->width = width;
      window->height = height;
    }
  });

  glfwSetScrollCallback(window.glfwWindow, [](GLFWwindow *_windowPtr,
                                              double xoffset, double yoffset) {
    observer.pitch = fmod(observer.pitch - yoffset * scrollFactor.x, 360.0f);
    observer.yaw = fmod(observer.yaw + xoffset * scrollFactor.y, 360.0f);
    observer.pitch = glm::clamp(observer.pitch, -90.0f, 90.0f);
  });

  glfwSetKeyCallback(window.glfwWindow, keyCallback);

  glfwMakeContextCurrent(window.glfwWindow);
  glfwGetFramebufferSize(window.glfwWindow, &window.width, &window.height);
  glViewport(0, 0, window.width, window.height);
  glfwSwapInterval(VSYNC_ENABLED);

  //
  // Final phase - Initialize rendering related
  //
  world.startWorldGeneration(&observer);
  world.worldObjects.push_back(&observer);
  auto scene = Scene(observer);

  std::chrono::duration lastTime =
      std::chrono::system_clock::now().time_since_epoch();
  std::chrono::duration currentTime =
      std::chrono::system_clock::now().time_since_epoch();
  float deltaTime = 1.0;
  float timePassed = 0.0;

  glEnable(GL_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glCullFace(GL_CCW);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  while (!glfwWindowShouldClose(window.glfwWindow)) {
    scene.draw(window, world);
  }
  glfwDestroyWindow(window.glfwWindow);
  glfwTerminate();

  return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
    case GLFW_KEY_L:
      wireframe = !wireframe;
      glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
      break;
    default:
      break;
    }
  }
}
