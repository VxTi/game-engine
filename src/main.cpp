#include <iostream>
#include "rendering/Rendering.h"
#include "rendering/VBO.h"
#include "io/Files.h"
#include "rendering/Shader.h"
#include "world/entity/Player.h"
#include "world/SimplexNoise.h"
#include "world/World.h"

using namespace std::chrono;

#define VSYNC_ENABLED 0

#define NEAR_PLANE 0.1f
#define FAR_PLANE 10000.0f
#define FOV 70.0f

/** Window related variables */
GLint width, height;
GLFWwindow *window;

/** World object related variables */
Player player = Player();
World world;

/** Rendering related variables */
Shader *worldShader, *skyboxShader;
Renderer renderer;
VBO *skybox;
Frustum viewFrustum;


const std::string shaderDirectory = "/Users/lucawarm/Jetbrains/CLion/graphics-test/shaders/";
const glm::vec2 scrollFactor = glm::vec2(1.f, 1.f);

void buildSkybox();
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

int main()
{
    if ( !glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    player.frictionConstant = 5.0f;

    glfwSetErrorCallback([](int error, const char *description) {
        std::cout << "GLFW Error " << error << " - " << description << std::endl;
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1200, 700, "Window Test", NULL, NULL);

    if ( !window ) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        ::width = width;
        ::height = height;
    });

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
        player.pitch = fmod(player.pitch - yoffset * scrollFactor.x, 360.0f);
        player.yaw = fmod(player.yaw + xoffset * scrollFactor.y, 360.0f);
        player.pitch = glm::clamp(player.pitch, -90.0f, 90.0f);
    });

    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(VSYNC_ENABLED);

    skyboxShader = new Shader(shaderDirectory, "skybox");
    worldShader = new Shader(shaderDirectory, "world_rendering");

    world.generate(player);

    buildSkybox();

    viewFrustum = createFrustum(NEAR_PLANE, FAR_PLANE, FOV, (float) width / (float) height);

    duration lastTime = system_clock::now().time_since_epoch();
    duration currentTime = system_clock::now().time_since_epoch();
    float deltaTime = 1.0;
    float timePassed = 0.0;

    // Wireframes
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    renderer.setRenderMode(RENDER_MODE_3D);

    while ( !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        /** Skybox rendering */
        skyboxShader->bind();
        renderer.resetMatrices();
        renderer.rotate(glm::vec4(glm::radians(player.pitch), glm::radians(player.yaw), 0.0, 0.0));
        renderer.scale(FAR_PLANE);
        renderer.computeMatrices(FOV, NEAR_PLANE, FAR_PLANE, (float) width, (float) height);
        renderer.pushMatrices(skyboxShader->getProgramId());
        glUniform2fv(glGetUniformLocation(skyboxShader->getProgramId(), "u_CameraRotation"), 2, glm::value_ptr(glm::vec2(player.pitch, player.yaw)));
        skybox->draw(deltaTime);

        renderer.resetMatrices();
        renderer.translate(glm::vec4(player.position, 1.0f));
        renderer.rotate(glm::vec4(glm::radians(player.pitch), glm::radians(player.yaw), 0.0, 0.0));

        /** World rendering section */
        worldShader->bind();
        renderer.computeMatrices(FOV, NEAR_PLANE, FAR_PLANE, (float) width, (float) height);
        renderer.pushMatrices(worldShader->getProgramId());

        // Provide camera position to shader

        worldShader->uniformVec3("u_CameraPosition", player.position.x, player.position.y, player.position.z);
        worldShader->uniformFloat("u_time", timePassed);

        world.render(deltaTime, &player, viewFrustum);
        player.update(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update delta time
        lastTime = currentTime;
        currentTime = system_clock::now().time_since_epoch();
        deltaTime = (float) duration_cast<microseconds>(currentTime - lastTime).count() / 1000000.0f;
        timePassed += deltaTime;
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    // Free up some memory
    delete skybox;
    delete skyboxShader;
    delete worldShader;

    return 0;
}

void buildSkybox()
{
    skybox = new VBO();

    skybox->withVertices((Vertex[8]) {
            { -1, -1, -1 },
            { 1,  -1, -1 },
            { 1,  -1, 1 },
            { -1, -1, 1 },
            { -1, 1,  -1 },
            { 1,  1,  -1 },
            { 1,  1,  1 },
            { -1, 1,  1 },
    }, 8);
    skybox->withIndices((unsigned int[36]) {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            5, 4, 7, 7, 6, 5,
            4, 0, 3, 3, 7, 4,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4
    }, 36);
    skybox->build();
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if ( action == GLFW_PRESS ) {
        switch ( key ) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default:
                break;
        }
    }
}
