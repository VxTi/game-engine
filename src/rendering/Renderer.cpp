//
// Created by Luca Warmenhoven on 17/05/2024.
//

#include "Rendering.h"

void Renderer::computeMatrices(float fov, float zNear, float zFar, float width, float height)
{
    // Model matrix = Vec4(World) = Mat4(Translate) * Mat4(Rotate) * Mat4(Scale) * Vec4(Vertex)
    // View matrix = Vec4(Camera) = Mat4(Camera) * Vec4(World)

    if ( this->renderMode == RENDER_MODE_3D ) {
        this->modelMatrix = glm::mat4(1.0f);
        this->modelMatrix = glm::scale(this->modelMatrix, Transformation::scale);
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(-this->position));
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
    glUniformMatrix4fv(
            glGetUniformLocation(shaderProgramId, "u_ModelMatrix"),
            1, GL_FALSE, glm::value_ptr(this->modelMatrix)
    );
    glUniformMatrix4fv(
            glGetUniformLocation(shaderProgramId, "u_ViewMatrix"),
            1, GL_FALSE, glm::value_ptr(this->viewMatrix)
    );
    glUniformMatrix4fv(
            glGetUniformLocation(shaderProgramId, "u_ProjectionMatrix"),
            1, GL_FALSE, glm::value_ptr(this->projectionMatrix));

    this->modelViewProjectionMatrix = this->projectionMatrix * this->viewMatrix * this->modelMatrix;
    glUniformMatrix4fv(
            glGetUniformLocation(shaderProgramId, "u_ModelViewProjectionMatrix"),
            1, GL_FALSE, glm::value_ptr(this->modelViewProjectionMatrix)
    );

    if ( this->renderMode == RENDER_MODE_3D ) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::translate(vec3 pos)
{
    Transformation::position = pos;
}

void Renderer::translate(float x, float y, float z)
{
    Transformation::position = glm::vec4(x, y, z, 1.0f);
}

void Renderer::translateX(float x)
{
    Transformation::position.x = x;
}

void Renderer::translateY(float y)
{
    Transformation::position.y = y;
}

void Renderer::translateZ(float z)
{
    Transformation::position.z = z;
}

void Renderer::rotate(glm::vec4 rotation)
{
    Transformation::rotation = rotation;
}

void Renderer::rotate(float x, float y, float z)
{
    Transformation::rotation = glm::vec4(x, y, z, 1.0f);
}

void Renderer::rotateX(float radians)
{
    Transformation::rotation.x = radians;
}

void Renderer::rotateY(float radians)
{
    Transformation::rotation.y = radians;
}

void Renderer::rotateZ(float radians)
{
    Transformation::rotation.z = radians;
}

void Renderer::scale(glm::vec3 scalingFactor)
{
    Transformation::scale = scalingFactor;
}

void Renderer::scale(float scalingFactor)
{
    Transformation::scale = glm::vec3(scalingFactor);
}

void Renderer::scale(float x, float y, float z)
{
    Transformation::scale = glm::vec3(x, y, z);
}

void Renderer::scaleX(float x)
{
    Transformation::scale.x = x;
}

void Renderer::scaleY(float y)
{
    Transformation::scale.y = y;
}

void Renderer::scaleZ(float z)
{
    Transformation::scale.z = z;
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
    this->modelMatrix = mat4(1.0f);
    this->viewMatrix = mat4(1.0f);
    this->projectionMatrix = mat4(1.0f);
    Transformation::position = glm::vec4(0.0f);
    Transformation::rotation = glm::vec4(0.0f);
    Transformation::scale = glm::vec3(1.0f);
}

Renderer::Renderer() : Transformation() {}

/*** Drawable implementation **/
Drawable::Drawable(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
    Transformation::position = position;
    Transformation::scale = scale;
    Transformation::rotation = rotation;
}