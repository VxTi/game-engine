//
// Created by Luca Warmenhoven on 19/05/2024.
//

#include "Player.h"
#include "glm/ext/matrix_transform.hpp"

void Player::update(float deltaTime)
{
    Entity::update(deltaTime);
    this->handleInput(glfwGetCurrentContext(), deltaTime);
}

void Player::handleInput(GLFWwindow *window, float deltaTime)
{
    auto direction = glm::vec3(
            (glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S)),
            (glfwGetKey(window, GLFW_KEY_SPACE) - glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)),
            (glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A))
    );

    // Get the angle at which to move to
    float angle = -this->yaw - 90.f + (direction.z != 0 ? (direction.x != 0 ? direction.z * 45.f : 0) : direction.z * 90.f);

    // Get the rotation matrix
    glm::mat4 rotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(angle),
            glm::vec3(0, 1.0, 0));
    // Rotate the direction vector
    direction = glm::vec3(rotation * glm::vec4(direction, 0.0f));
    // Apply appropriate force
    this->applyForce(direction * movementSpeedForce, deltaTime);
}

void Player::move(float x, float y, float z)
{
    this->acceleration = glm::vec3(x, y, z);
}