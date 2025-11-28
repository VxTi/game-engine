//
// Created by Luca Warmenhoven on 17/05/2024.
//

#include <engine/renderer/renderer.h>

std::vector<matrix_stack_entry_t> Renderer::matrixStack = {};
matrix_stack_entry_t Renderer::currentMatrix = {
    glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f),
    glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f),
};

unsigned char Renderer::renderMode = RENDER_MODE_3D;

void Renderer::computeMatrices(float fov, float zNear, float zFar, float width,
                               float height) {
  if (Renderer::renderMode == RENDER_MODE_3D) {
    Renderer::currentMatrix.modelMatrix = glm::mat4(1.0f);

    Renderer::currentMatrix.modelMatrix = glm::scale(
        Renderer::currentMatrix.modelMatrix, Renderer::currentMatrix.scale);
    Renderer::currentMatrix.modelMatrix = glm::rotate(
        Renderer::currentMatrix.modelMatrix, Renderer::currentMatrix.rotation.z,
        glm::vec3(0.0f, 0.0f, 1.0f));
    Renderer::currentMatrix.modelMatrix = glm::rotate(
        Renderer::currentMatrix.modelMatrix, Renderer::currentMatrix.rotation.x,
        glm::vec3(1.0f, 0.0f, 0.0f));
    Renderer::currentMatrix.modelMatrix = glm::rotate(
        Renderer::currentMatrix.modelMatrix, Renderer::currentMatrix.rotation.y,
        glm::vec3(0.0f, 1.0f, 0.0f));
    Renderer::currentMatrix.modelMatrix =
        glm::translate(Renderer::currentMatrix.modelMatrix,
                       glm::vec3(-Renderer::currentMatrix.position));
    Renderer::currentMatrix.projectionMatrix =
        glm::perspective(glm::radians(fov), width / height, zNear, zFar);
    Renderer::currentMatrix.viewMatrix =
        glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  } else {
    Renderer::currentMatrix.modelMatrix = glm::translate(
        glm::mat4(1.0f), glm::vec3(Renderer::currentMatrix.position.x,
                                   Renderer::currentMatrix.position.y, 0.0f));
    Renderer::currentMatrix.modelMatrix = glm::rotate(
        Renderer::currentMatrix.modelMatrix, Renderer::currentMatrix.rotation.z,
        glm::vec3(0.0f, 0.0f, 1.0f));
    Renderer::currentMatrix.projectionMatrix =
        glm::ortho(0.0f, width, height, 0.0f, zNear, zFar);
  }
}

void Renderer::updateFrustum(Frustum *frustum) {
  frustum->updateViewProjectionMatrix(Renderer::currentMatrix.viewMatrix,
                                      Renderer::currentMatrix.projectionMatrix);
}

void Renderer::pushMatrices(GLuint shaderProgramId) {
  glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "u_ModelMatrix"), 1,
                     GL_FALSE,
                     glm::value_ptr(Renderer::currentMatrix.modelMatrix));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, "u_ViewMatrix"), 1,
                     GL_FALSE,
                     glm::value_ptr(Renderer::currentMatrix.viewMatrix));
  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgramId, "u_ProjectionMatrix"), 1, GL_FALSE,
      glm::value_ptr(Renderer::currentMatrix.projectionMatrix));

  Renderer::currentMatrix.modelViewProjectionMatrix =
      Renderer::currentMatrix.projectionMatrix *
      Renderer::currentMatrix.viewMatrix * Renderer::currentMatrix.modelMatrix;
  glUniformMatrix4fv(
      glGetUniformLocation(shaderProgramId, "u_ModelViewProjectionMatrix"), 1,
      GL_FALSE,
      glm::value_ptr(Renderer::currentMatrix.modelViewProjectionMatrix));

  if (Renderer::renderMode == RENDER_MODE_3D) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void Renderer::translate(vec3 pos) { Renderer::currentMatrix.position = pos; }

void Renderer::translate(float x, float y, float z) {
  Renderer::translate(glm::vec3(x, y, z));
}

void Renderer::translateX(float x) { Renderer::currentMatrix.position.x = x; }

void Renderer::translateY(float y) { Renderer::currentMatrix.position.y = y; }

void Renderer::translateZ(float z) { Renderer::currentMatrix.position.z = z; }

void Renderer::rotate(glm::vec4 rotation) {
  Renderer::currentMatrix.rotation = glm::vec3(rotation);
}

void Renderer::rotate(float x, float y, float z) {
  Renderer::rotate(glm::vec4(x, y, z, 0.0f));
}

void Renderer::rotateX(float radians) {
  Renderer::currentMatrix.rotation.x = radians;
}

void Renderer::rotateY(float radians) {
  Renderer::currentMatrix.rotation.y = radians;
}

void Renderer::rotateZ(float radians) {
  Renderer::currentMatrix.rotation.z = radians;
}

void Renderer::scale(glm::vec3 scalingFactor) {
  Renderer::scale(scalingFactor.x, scalingFactor.y, scalingFactor.z);
}

void Renderer::scale(float scalingFactor) {
  Renderer::scale(scalingFactor, scalingFactor, scalingFactor);
}

void Renderer::scale(float x, float y, float z) {
  Renderer::currentMatrix.scale = glm::vec3(x, y, z);
}

void Renderer::scaleX(float x) { Renderer::currentMatrix.scale.x = x; }

void Renderer::scaleY(float y) { Renderer::currentMatrix.scale.y = y; }

void Renderer::scaleZ(float z) { Renderer::currentMatrix.scale.z = z; }

void Renderer::setModelMatrix(glm::mat4 model) {
  Renderer::currentMatrix.modelMatrix = model;
  Renderer::currentMatrix.scale = glm::vec3(1.0f);
  Renderer::currentMatrix.rotation = glm::vec3(0.0f);
  Renderer::currentMatrix.position = glm::vec3(0.0f);
}

void Renderer::setViewMatrix(glm::mat4 view) {
  Renderer::currentMatrix.viewMatrix = view;
}

void Renderer::setProjectionMatrix(glm::mat4 projection) {
  Renderer::currentMatrix.projectionMatrix = projection;
}

void Renderer::setModelViewProjectionMatrix(glm::mat4 model, glm::mat4 view,
                                            glm::mat4 projection) {
  Renderer::currentMatrix.modelMatrix = model;
  Renderer::currentMatrix.viewMatrix = view;
  Renderer::currentMatrix.projectionMatrix = projection;
  Renderer::currentMatrix.modelViewProjectionMatrix = projection * view * model;
}

void Renderer::setRenderMode(unsigned char mode) {
  Renderer::renderMode = mode;
}

void Renderer::resetMatrices() {
  Renderer::currentMatrix.modelMatrix = glm::mat4(1.0f);
  Renderer::currentMatrix.viewMatrix = glm::mat4(1.0f);
  Renderer::currentMatrix.projectionMatrix = glm::mat4(1.0f);
  Renderer::currentMatrix.modelViewProjectionMatrix = glm::mat4(1.0f);
  Renderer::currentMatrix.scale = glm::vec3(1.0f);
  Renderer::currentMatrix.rotation = glm::vec3(0.0f);
  Renderer::currentMatrix.position = glm::vec3(0.0f);
}

void Renderer::pushMatrix() {
  Renderer::matrixStack.push_back(Renderer::currentMatrix);
}

void Renderer::popMatrix() {
  if (Renderer::matrixStack.empty())
    return;

  Renderer::currentMatrix = Renderer::matrixStack.back();
  Renderer::matrixStack.pop_back();
}
