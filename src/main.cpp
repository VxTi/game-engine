#include <iostream>
#include "rendering/Renderer.h"
#include "io/Files.h"
#include "world/entity/Player.h"
#include "world/SimplexNoise.h"
#include "world/World.h"

using namespace std::chrono;

#define VSYNC_ENABLED 0

GLint width, height;
GLFWwindow *window;

World world;
Renderer renderer;

Player player = Player();

const std::string shaderDirectory = "/Users/lucawarm/Jetbrains/CLion/graphics-test/shaders/";
const glm::vec2 scrollFactor = glm::vec2(1.f, 1.f);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    player.frictionConstant = 5.0f;

    glfwSetErrorCallback([](int error, const char* description) {
        std::cout << "GLFW Error " << error << " - " << description << std::endl;
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1200, 700, "Window Test", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        ::width = width;
        ::height = height;
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        player.pitch = fmod(player.pitch - yoffset * scrollFactor.x, 360.0f);
        player.yaw = fmod(player.yaw + xoffset * scrollFactor.y, 360.0f);
        player.pitch = glm::clamp(player.pitch, -90.0f, 90.0f);
    });

    glfwSetKeyCallback(window, keyCallback);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(VSYNC_ENABLED);

    Shader shader = Shader(shaderDirectory, "3d");
    shader.bind();

    renderer.setRenderMode(RENDER_MODE_3D);

    world.generate();


    duration lastTime = system_clock::now().time_since_epoch();
    duration currentTime = system_clock::now().time_since_epoch();
    float deltaTime = 1.0;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        renderer.translate(glm::vec4(player.position, 1.0f));
        renderer.rotate(glm::vec4(glm::radians(player.pitch), glm::radians(player.yaw), 0.0, 0.0));

        renderer.computeMatrices(70.0f,0.1f,1000.0f,(float) width,(float) height);
        renderer.pushMatrices(shader.getProgramId());

        world.render(deltaTime);
        player.update(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update delta time
        lastTime = currentTime;
        currentTime = system_clock::now().time_since_epoch();
        deltaTime = (float)duration_cast<microseconds>(currentTime - lastTime).count() / 1000000.0f;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
