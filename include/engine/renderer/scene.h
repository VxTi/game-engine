//
// Created by Luca Warmenhoven on 17/05/2024.
//

#ifndef GAME_RENDERER_SCENE_H
#define GAME_RENDERER_SCENE_H

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <chrono>
#include <engine/math/transform.h>
#include <engine/renderer/culling/frustum.h>
#include <engine/renderer/shader.h>
#include <engine/renderer/vbo.h>
#include <engine/renderer/window.h>
#include <engine/world/world.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

using namespace glm;

#define VBO_POSITION_INDEX 0
#define VBO_NORMAL_INDEX 1
#define VBO_UV_INDEX 2

#define NEAR_PLANE (0.1f)
#define FAR_PLANE (50000.0f)
#define SKYBOX_SIZE (FAR_PLANE / 2)
#define FOV (70.0f)

typedef struct {
  glm::mat4 modelMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  glm::mat4 modelViewProjectionMatrix;

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
} matrix_stack_entry_t;

class Scene {
  /** The model, view and projection matrices. */
  std::vector<matrix_stack_entry_t> matrixStack;
  matrix_stack_entry_t currentMatrix;
  Transform observer;

  std::chrono::microseconds lastTime;
  std::chrono::microseconds currentTime;

  double timePassed = 0;

  Shader skyboxShader;
  Shader sceneShader;
  Frustum frustum;
  VBO skyboxVbo;

public:
  double deltaTime;

  Scene(Transform observer);

  void draw(Window &window, World &world);

  mat4 getModelMatrix() { return currentMatrix.modelMatrix; }
  mat4 getViewMatrix() { return currentMatrix.viewMatrix; }
  mat4 getProjectionMatrix() { return currentMatrix.projectionMatrix; }
  mat4 getModelViewProjectionMatrix() {
    return currentMatrix.modelViewProjectionMatrix;
  }

  /**
   * Function that prepares all the matrices for further use.
   * These matrices can then be pushed to the provided shader by calling
   * `pushMatrices(programId)`.
   */
  void computeMatrices(float fov, float zNear, float zFar, float width,
                       float height);

  /**
   * Function that prepares the object for rendering.
   */
  void pushMatrices(GLuint programId);

  /**
   * Updates the provided frustum with the previously calculated matrices.
   */
  void updateFrustum(Frustum *frustum);

  /**
   * Function that translates the object.
   * @param pos The pos to translate to.
   */
  void translate(vec3 pos);

  /**
   * Function that translates the object.
   * @param x The x position to translate to.
   * @param y The y position to translate to.
   * @param z The z position to translate to.
   */
  void translate(float x, float y, float z);

  /**
   * Function that translates the object on the x axis.
   * @param x The x position to translate to.
   */
  void translateX(float x);

  /**
   * Function that translates the object on the y axis.
   * @param y The y position to translate to.
   */
  void translateY(float y);

  /**
   * Function that translates the object on the z axis.
   * @param z The z position to translate to.
   */
  void translateZ(float z);

  /**
   * Function that rotates the object.
   * @param rotation The rotation to rotate to.
   */
  void rotate(vec4 rotation);

  /**
   * Function that rotates the object.
   * @param x The x rotation to rotate to.
   * @param y The y rotation to rotate to.
   * @param z The z rotation to rotate to.
   */
  void rotate(float x, float y, float z);

  /**
   * Function that rotates the object on the x axis.
   * @param radians The x rotation to rotate to.
   */
  void rotateX(float radians);

  /**
   * Function that rotates the object on the y axis.
   * @param radians The y rotation to rotate to.
   */
  void rotateY(float radians);

  /**
   * Function that rotates the object on the z axis.
   * @param radians The z rotation to rotate to.
   */
  void rotateZ(float radians);

  /**
   * Function that scales the object.
   *
   * @param scalingFactor The scalingFactor to scalingFactor to.
   */
  void scale(vec3 scalingFactor);

  /**
   * Function that scales the object.
   *
   * @param x The x scale to scale to.
   * @param y The y scale to scale to.
   * @param z The z scale to scale to.
   */
  void scale(float x, float y, float z);

  /**
   * Function that scales the object.
   *
   * @param scalar The scalar to scale to.
   */
  void scale(float scalar);

  /**
   * Function that scales the object on the x axis.
   * @param x The x scale to scale to.
   */
  void scaleX(float x);

  /**
   * Function that scales the object on the y axis.
   * @param y The y scale to scale to.
   */
  void scaleY(float y);

  /**
   * Function that scales the object on the z axis.
   * @param z The z scale to scale to.
   */
  void scaleZ(float z);

  /**
   * Function that sets the model matrix.
   * @param model The model matrix to set.
   */
  void setModelMatrix(mat4 model);

  /**
   * Function that sets the view matrix.
   * @param view The view matrix to set.
   */
  void setViewMatrix(mat4 view);

  /**
   * Function that sets the projection matrix.
   * @param projection The projection matrix to set.
   */
  void setProjectionMatrix(mat4 projection);

  /**
   * Function that sets the model, view and projection matrices.
   * @param model The model matrix to set.
   * @param view The view matrix to set.
   * @param projection The projection matrix to set.
   */
  void setModelViewProjectionMatrix(mat4 model, mat4 view, mat4 projection);

  /**
   * Function that resets the matrices.
   */
  void resetMatrices();
};

#endif // GAME_RENDERER_SCENE_H
