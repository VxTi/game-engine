#include <iostream>
#include "rendering/Rendering.h"
#include "io/Files.h"
#include "world/entity/Entity.h"

using namespace std::chrono;

GLint width, height;
GLFWwindow *window;
Rendering renderer;
Player player = Player();

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    glfwSetErrorCallback([](int error, const char* description) {
        std::cout << "GLFW Error " << error << " - " << description << std::endl;
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1200, 700, "Window Test", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {

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

    std::vector<Vertex> vertices = {
            {-10.0, 0.0, 0.0},
            {10, 0.0, 0.0},
            {10, 10, 0.0},
            {-10, 10, 0.0},
    };
    std::vector<int> indices = {
            0, 1, 2,
            2, 3, 0
    };
    VBO vbo;
    vbo.withVertices(vertices);
    vbo.withIndices(indices);
    vbo.build();
    Shader shader = Shader("/Users/lucawarm/Jetbrains/CLion/graphics-test/shaders", "3d");
    shader.bind();

    renderer.setRenderMode(RENDER_MODE_3D);


    duration lastTime = system_clock::now().time_since_epoch();
    float deltaTime = 1.0;

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        renderer.translate(glm::vec4(player.position, 1.0f));
        renderer.rotate(glm::vec4(player.rotation, 1.0f));

        player.update(deltaTime);

        renderer.computeMatrices(70.0f,0.1f,1000.0f,(float) width,(float) height);
        renderer.pushMatrices(shader.getProgramId());
        vbo.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
        deltaTime = (
                duration_cast<nanoseconds>(
                        system_clock::now().time_since_epoch()
                        ).count() - duration_cast<nanoseconds>(lastTime).count()
                ) / 1000000000.0;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if ( action == GLFW_PRESS || action == GLFW_REPEAT ) {
        if ( key == GLFW_KEY_W ) {
            player.applyForce(glm::vec3(0.0f, 0.0f, -1.0f));
        } else if ( key == GLFW_KEY_S ) {
            player.applyForce(glm::vec3(0.0f, 0.0f, 1.0f));
        } else if ( key == GLFW_KEY_A ) {
            player.applyForce(glm::vec3(-1.0f, 0.0f, 0.0f));
        } else if ( key == GLFW_KEY_D ) {
            player.applyForce(glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
}
