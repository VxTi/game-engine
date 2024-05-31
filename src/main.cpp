#include <iostream>
#include "rendering/Rendering.h"
#include "rendering/VBO.h"
#include "io/Files.h"
#include "rendering/Shader.h"
#include "world/entity/Player.h"
#include "world/SimplexNoise.h"
#include "world/World.h"
#include "rendering/culling/Frustum.h"

using namespace std::chrono;

#define VSYNC_ENABLED 1

#define NEAR_PLANE 0.1f
#define FAR_PLANE 50000.0f
#define SKYBOX_SIZE (FAR_PLANE / 2)
#define FOV 70.0f

bool wireframe = false;

/** Window related variables */
GLint width, height;
GLFWwindow *mainWindow;

/** World object related variables */
Player player = Player();
World *world;
glm::vec3 sunPosition = glm::normalize(glm::vec3(5.0f, 5.0f, 3.0f));

/** Rendering related variables */
Shader *worldShader, *skyboxShader;
Renderer renderer;
VBO *skybox;
Frustum *viewFrustum;


const std::string shaderDirectory = "/Users/lucawarm/Jetbrains/CLion/graphics-test/shaders/";
const glm::vec2 scrollFactor = glm::vec2(1.f, 1.f);

void assembleSkyboxMesh();

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

int main()
{
    if ( !glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    world = new World();
    player.frictionConstant = 10.0f;
    player.position = vec3(10, 10, 10);

    glfwSetErrorCallback([](int error, const char *description) {
        std::cout << "GLFW Error " << error << " - " << description << std::endl;
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mainWindow = glfwCreateWindow(1200, 700, "Window Test", NULL, NULL);

    if ( !mainWindow ) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwSetFramebufferSizeCallback(mainWindow, [](GLFWwindow *window, int width, int height) {
        ::width = width;
        ::height = height;
    });

    glfwSetScrollCallback(mainWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
        player.pitch = fmod(player.pitch - yoffset * scrollFactor.x, 360.0f);
        player.yaw = fmod(player.yaw + xoffset * scrollFactor.y, 360.0f);
        player.pitch = glm::clamp(player.pitch, -90.0f, 90.0f);
    });

    glfwSetKeyCallback(mainWindow, keyCallback);

    glfwMakeContextCurrent(mainWindow);
    glfwGetFramebufferSize(mainWindow, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(VSYNC_ENABLED);

    skyboxShader = new Shader(shaderDirectory, "skybox");
    worldShader = new Shader(shaderDirectory, "world_rendering");

    world->startWorldGeneration(&player);
    world->worldObjects->push_back(&player);

    assembleSkyboxMesh();

    viewFrustum = new Frustum(
            &player,
            glm::mat4(1.0f) ,
            glm::mat4(1.0f)
            );

    duration lastTime = system_clock::now().time_since_epoch();
    duration currentTime = system_clock::now().time_since_epoch();
    float deltaTime = 1.0;
    float timePassed = 0.0;

    renderer.setRenderMode(RENDER_MODE_3D);

    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    while ( !glfwWindowShouldClose(mainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        /** Skybox rendering */
        skyboxShader->bind();
        renderer.resetMatrices();
        renderer.rotateX(glm::radians(player.pitch));
        renderer.rotateY(glm::radians(player.yaw));
        renderer.scale(SKYBOX_SIZE);
        renderer.computeMatrices(FOV, NEAR_PLANE, FAR_PLANE, (float) width, (float) height);
        renderer.updateFrustum(viewFrustum);
        skyboxShader->uniformVec2("u_Resolution", (float) width, (float) height);
        skyboxShader->uniformFloat("u_SunSize", .15f);
        skyboxShader->uniformVec3("u_SunPosition", sunPosition.x, sunPosition.y, sunPosition.z);
        renderer.pushMatrices(skyboxShader->getProgramId());
        skybox->draw(deltaTime);


        renderer.resetMatrices();
        renderer.translate(glm::vec4(player.position, 1.0f));
        renderer.rotate(glm::vec4(glm::radians(player.pitch), glm::radians(player.yaw), 0.0, 0.0));

        /** World rendering section */
        worldShader->bind();
        renderer.computeMatrices(FOV, NEAR_PLANE, FAR_PLANE, (float) width, (float) height);
        renderer.pushMatrices(worldShader->getProgramId());

        // Provide camera position to shader
        worldShader->uniformVec3("u_SunPosition", sunPosition.x, sunPosition.y, sunPosition.z);
        worldShader->uniformVec3("u_CameraPosition", player.position.x, player.position.y, player.position.z);
        worldShader->uniformFloat("u_time", timePassed);

        world->render(deltaTime, viewFrustum);
        world->update(deltaTime);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();

        // Update delta time
        lastTime = currentTime;
        currentTime = system_clock::now().time_since_epoch();
        deltaTime = (float) duration_cast<microseconds>(currentTime - lastTime).count() / 1000000.0f;
        timePassed += deltaTime;
    }
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    // Free up some memory
    delete skybox;
    delete skyboxShader;
    delete worldShader;
    delete world;

    return 0;
}

/**
 * Function for creating the skybox mesh
 */
void assembleSkyboxMesh()
{
    skybox = new VBO();

    skybox->withVertices((vertex_t[8]) {
            { -1, -1, -1, .5, .5, .5 },   // - - -
            { 1,  -1, -1, -.5, .5, .5 },  // + - -
            { 1,  -1, 1, -.5, .5, -.5 },  // + - +
            { -1, -1, 1, .5, .5, -.5},    // - - +

            { -1, 1,  -1, .5, -.5, .5 },
            { 1,  1,  -1, -.5, -.5,  .5 },
            { 1,  1,  1, -.5, -.5, -.5},
            { -1, 1,  1, .5, -.5, -.5},
    }, 8);
    skybox->withIndices((unsigned int[36]) {
            0, 3, 1, 1, 3, 2, // Bottom
            4, 5, 6, 6, 7, 4, // Top

            3, 0, 4, 4, 7, 3, // -X

            1, 2, 5, 5, 2, 6, // +X
            0, 1, 4, 1, 5, 4, // -Z
            2, 3, 7, 7, 6, 2 // +Z

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
            case GLFW_KEY_L:
                wireframe = !wireframe;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
                break;
            default:
                break;
        }
    }
}
