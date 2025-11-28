//
// Created by Luca Warmenhoven on 17/05/2024.
//

#include <engine/renderer/scene.h>

vertex_t skyboxVertices[] = {
    {-1, -1, -1, .5, .5, .5}, {1, -1, -1, -.5, .5, .5},
    {1, -1, 1, -.5, .5, -.5}, {-1, -1, 1, .5, .5, -.5},
    {-1, 1, -1, .5, -.5, .5}, {1, 1, -1, -.5, -.5, .5},
    {1, 1, 1, -.5, -.5, -.5}, {-1, 1, 1, .5, -.5, -.5},
};
unsigned int skyboxIndices[] = {0, 3, 1, 1, 3, 2, 4, 5, 6, 6, 7, 4,
                                3, 0, 4, 4, 7, 3, 1, 2, 5, 5, 2, 6,
                                0, 1, 4, 1, 5, 4, 2, 3, 7, 7, 6, 2};

glm::vec3 sunPosition = glm::normalize(glm::vec3(5.0f, 5.0f, 3.0f));

void Scene::computeMatrices(float fov, float zNear, float zFar, float width,
                            float height) {
  this->currentMatrix.modelMatrix = glm::mat4(1.0f);

  this->currentMatrix.modelMatrix =
      glm::scale(this->currentMatrix.modelMatrix, this->currentMatrix.scale);
  this->currentMatrix.modelMatrix =
      glm::rotate(this->currentMatrix.modelMatrix,
                  this->currentMatrix.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  this->currentMatrix.modelMatrix =
      glm::rotate(this->currentMatrix.modelMatrix,
                  this->currentMatrix.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  this->currentMatrix.modelMatrix =
      glm::rotate(this->currentMatrix.modelMatrix,
                  this->currentMatrix.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  this->currentMatrix.modelMatrix =
      glm::translate(this->currentMatrix.modelMatrix,
                     glm::vec3(-this->currentMatrix.position));
  this->currentMatrix.projectionMatrix =
      glm::perspective(glm::radians(fov), width / height, zNear, zFar);
  this->currentMatrix.viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));
}

void Scene::updateFrustum(Frustum *frustum) {
  frustum->updateViewProjectionMatrix(this->currentMatrix.viewMatrix,
                                      this->currentMatrix.projectionMatrix);
}

void Scene::pushMatrices(GLuint shaderProgramId) {
  glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "u_ModelMatrix"), 1,
                     GL_FALSE, glm::value_ptr(this->currentMatrix.modelMatrix));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "u_ViewMatrix"), 1,
                     GL_FALSE, glm::value_ptr(this->currentMatrix.viewMatrix));
  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgramId, "u_ProjectionMatrix"), 1, GL_FALSE,
      glm::value_ptr(this->currentMatrix.projectionMatrix));

  this->currentMatrix.modelViewProjectionMatrix =
      this->currentMatrix.projectionMatrix * this->currentMatrix.viewMatrix *
      this->currentMatrix.modelMatrix;
  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgramId, "u_ModelViewProjectionMatrix"), 1,
      GL_FALSE, glm::value_ptr(this->currentMatrix.modelViewProjectionMatrix));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void Scene::translate(vec3 pos) { this->currentMatrix.position = pos; }

void Scene::translate(float x, float y, float z) {
  Scene::translate(glm::vec3(x, y, z));
}

void Scene::translateX(float x) { this->currentMatrix.position.x = x; }

void Scene::translateY(float y) { this->currentMatrix.position.y = y; }

void Scene::translateZ(float z) { this->currentMatrix.position.z = z; }

void Scene::rotate(glm::vec4 rotation) {
  this->currentMatrix.rotation = glm::vec3(rotation);
}

void Scene::rotate(float x, float y, float z) {
  Scene::rotate(glm::vec4(x, y, z, 0.0f));
}

void Scene::rotateX(float radians) { this->currentMatrix.rotation.x = radians; }

void Scene::rotateY(float radians) { this->currentMatrix.rotation.y = radians; }

void Scene::rotateZ(float radians) { this->currentMatrix.rotation.z = radians; }

void Scene::scale(glm::vec3 scalingFactor) {
  Scene::scale(scalingFactor.x, scalingFactor.y, scalingFactor.z);
}

void Scene::scale(float scalingFactor) {
  Scene::scale(scalingFactor, scalingFactor, scalingFactor);
}

void Scene::scale(float x, float y, float z) {
  this->currentMatrix.scale = glm::vec3(x, y, z);
}

void Scene::scaleX(float x) { this->currentMatrix.scale.x = x; }

void Scene::scaleY(float y) { this->currentMatrix.scale.y = y; }

void Scene::scaleZ(float z) { this->currentMatrix.scale.z = z; }

void Scene::setModelMatrix(glm::mat4 model) {
  this->currentMatrix.modelMatrix = model;
  this->currentMatrix.scale = glm::vec3(1.0f);
  this->currentMatrix.rotation = glm::vec3(0.0f);
  this->currentMatrix.position = glm::vec3(0.0f);
}

void Scene::setViewMatrix(glm::mat4 view) {
  this->currentMatrix.viewMatrix = view;
}

void Scene::setProjectionMatrix(glm::mat4 projection) {
  this->currentMatrix.projectionMatrix = projection;
}

void Scene::setModelViewProjectionMatrix(glm::mat4 model, glm::mat4 view,
                                         glm::mat4 projection) {
  this->currentMatrix.modelMatrix = model;
  this->currentMatrix.viewMatrix = view;
  this->currentMatrix.projectionMatrix = projection;
  this->currentMatrix.modelViewProjectionMatrix = projection * view * model;
}
void Scene::resetMatrices() {
  this->currentMatrix.modelMatrix = glm::mat4(1.0f);
  this->currentMatrix.viewMatrix = glm::mat4(1.0f);
  this->currentMatrix.projectionMatrix = glm::mat4(1.0f);
  this->currentMatrix.modelViewProjectionMatrix = glm::mat4(1.0f);
  this->currentMatrix.scale = glm::vec3(1.0f);
  this->currentMatrix.rotation = glm::vec3(0.0f);
  this->currentMatrix.position = glm::vec3(0.0f);
}

Scene::Scene(Transform observer)
    : observer(std::move(observer)),
      frustum(Frustum(glm::mat4(1.0f), glm::mat4(1.0f))) {
  this->currentMatrix = {
      glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f),
      glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f),
  };

  this->skyboxShader = Shader();
  skyboxShader.addFragmentShader("skybox_frag.glsl");
  skyboxShader.addVertexShader("skybox_vert.glsl");
  skyboxShader.compile();

  this->sceneShader = Shader();
  sceneShader.addFragmentShader("world_rendering_frag.glsl");
  sceneShader.addVertexShader("world_rendering_vert.glsl");
  sceneShader.compile();

  this->skyboxVbo = VBO();
  skyboxVbo.addVertices(skyboxVertices, 8);
  skyboxVbo.addIndices(skyboxIndices, 36);
  skyboxVbo.build();
}

void applyUniforms(Shader &shader, World &world, Window &window,
                   Transform &observer, double timePassed) {
  shader.uniformFloat("u_time", timePassed);
  shader.uniformFloat("u_SunIntensity", world.sunIntensity);
  shader.uniformFloat("u_SunAmbient", world.sunAmbient);
  shader.uniformVec3("u_SunPosition", world.sunPosition);
  shader.uniformVec4("u_SunColor", world.sunColor);
  shader.uniformFloat("u_FogDensity", world.fogDensity);
  shader.uniformFloat("u_SunSize", world.sunSize);
  shader.uniformVec2("u_Resolution", (float)window.width, (float)window.height);
  shader.uniformVec3("u_CameraPosition", observer.position.x,
                     observer.position.y, observer.position.z);
}

/**
 * Function that draws the scene.
 */
void Scene::draw(Window &window, World &world) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  /** Skybox rendering */
  skyboxShader.bind();
  Scene::resetMatrices();
  Scene::rotateX(glm::radians(observer.pitch));
  Scene::rotateY(glm::radians(observer.yaw));
  Scene::scale(SKYBOX_SIZE);
  Scene::computeMatrices(FOV, NEAR_PLANE, FAR_PLANE, (float)window.width,
                         (float)window.height);
  Scene::updateFrustum(&this->frustum);

  // Send the uniforms to the shader

  applyUniforms(skyboxShader, world, window, observer, timePassed);

  Scene::pushMatrices(skyboxShader.getProgramId());
  this->skyboxVbo.draw(deltaTime);

  Scene::resetMatrices();
  Scene::translate(glm::vec4(observer.position, 1.0f));
  Scene::rotate(glm::vec4(glm::radians(observer.pitch),
                          glm::radians(observer.yaw), 0.0, 0.0));

  /** World rendering section */
  this->sceneShader.bind();
  Scene::computeMatrices(FOV, NEAR_PLANE, FAR_PLANE, (float)window.width,
                         (float)window.height);
  Scene::pushMatrices(this->sceneShader.getProgramId());

  applyUniforms(sceneShader, world, window, observer, timePassed);

  world.render(deltaTime, &this->frustum);
  world.update(deltaTime);

  glfwSwapBuffers(window.glfwWindow);
  glfwPollEvents();

  // Update delta time
  lastTime = currentTime;
  currentTime = std::chrono::system_clock::now().time_since_epoch();
  deltaTime =
      (float)duration_cast<std::chrono::microseconds>(currentTime - lastTime)
          .count() /
      1000000.0f;

  this->timePassed += deltaTime;
}
