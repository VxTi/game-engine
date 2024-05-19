//
// Created by Luca Warmenhoven on 19/05/2024.
//

#include "Entity.h"

AbstractBody::AbstractBody(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, glm::vec3 rotation, float mass,
                           float frictionConstant, float yaw, float pitch)
{
    this->position = position;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->rotation = rotation;
    this->mass = mass;
    this->frictionConstant = frictionConstant;
    this->yaw = yaw;
    this->pitch = pitch;
}

AbstractBody::AbstractBody() {
    this->position = glm::vec3(0.0f);
    this->velocity = glm::vec3(0.0f);
    this->acceleration = glm::vec3(0.0f);
    this->rotation = glm::vec3(0.0f);
    this->mass = 1.0f;
    this->frictionConstant = 2.0f;
    this->yaw = 0.0f;
    this->pitch = 0.0f;
}

void AbstractBody::update(float deltaTime) {
    this->velocity += this->acceleration * deltaTime;
    this->position += this->velocity * deltaTime;
    this->acceleration = glm::vec3(0.0f);
    this->velocity *= (1.0 / this->frictionConstant) / deltaTime;
}

void AbstractBody::applyForce(glm::vec3 force) {
    this->acceleration += force / this->mass;
}