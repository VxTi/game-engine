//
// Created by Luca Warmenhoven on 17/05/2024.
//

#include "Renderer.h"

const glm::mat4 Renderer::IDENTITY = glm::mat4(1.0f);

void Renderer::computeMatrices(float fov, float zNear, float zFar, float width, float height)
{
    // Model matrix = Vec4(World) = Mat4(Translate) * Mat4(Rotate) * Mat4(Scale) * Vec4(Vertex)
    // View matrix = Vec4(Camera) = Mat4(Camera) * Vec4(World)

    if ( this->renderMode == RENDER_MODE_3D ) {
        this->modelMatrix = glm::mat4(1.0f);
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->modelMatrix = glm::translate(this->modelMatrix,glm::vec3(-this->position));
        this->projectionMatrix = glm::perspective(glm::radians(fov),
                                                  width / height,
                                                  zNear, zFar);
        this->viewMatrix = glm::lookAt(
                glm::vec3(0.0f, 0.0f, -10.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
        );

    } else {
        this->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->position.x, this->position.y, 0.0f));
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->projectionMatrix = glm::ortho(0.0f, width, height, 0.0f, zNear, zFar);
    }
}

void Renderer::pushMatrices(GLuint shaderProgramId)
{
    glm::mat4 mvp = this->projectionMatrix * this->viewMatrix * this->modelMatrix;
    glUniformMatrix4fv(
            glGetUniformLocation(shaderProgramId, "u_ModelViewProjectionMatrix"),
            1, GL_FALSE, glm::value_ptr(mvp)
            );
    if ( this->renderMode == RENDER_MODE_3D ) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::translate(glm::vec4 pos)
{
    this->position = pos;
}

void Renderer::translate(float x, float y, float z)
{
    this->position = glm::vec4(x, y, z, 1.0f);
}

void Renderer::translateX(float x)
{
    this->position.x = x;
}

void Renderer::translateY(float y)
{
    this->position.y = y;
}

void Renderer::translateZ(float z)
{
    this->position.z = z;
}

void Renderer::rotate(glm::vec4 rotation)
{
    this->rotation = rotation;
}

void Renderer::rotate(float x, float y, float z)
{
    this->rotation = glm::vec4(x, y, z, 1.0f);
}

void Renderer::rotateX(float radians)
{
    this->rotation.x = radians;
}

void Renderer::rotateY(float radians)
{
    this->rotation.y = radians;
}

void Renderer::rotateZ(float radians)
{
    this->rotation.z = radians;
}

void Renderer::setModelMatrix(glm::mat4 model)
{
    this->modelMatrix = model;
}

void Renderer::setViewMatrix(glm::mat4 view)
{
    this->viewMatrix = view;
}

void Renderer::setProjectionMatrix(glm::mat4 projection)
{
    this->projectionMatrix = projection;
}

void Renderer::setModelViewProjectionMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    this->modelMatrix = model;
    this->viewMatrix = view;
    this->projectionMatrix = projection;
}

void Renderer::setRenderMode(unsigned char mode)
{
    this->renderMode = mode;
}

void Renderer::resetMatrices()
{
    this->modelMatrix = IDENTITY;
    this->viewMatrix = IDENTITY;
    this->projectionMatrix = IDENTITY;
    this->position = glm::vec4(0.0f);
    this->rotation = glm::vec4(0.0f);
}

Renderer::Renderer() = default;