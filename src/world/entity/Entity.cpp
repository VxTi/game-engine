//
// Created by Luca Warmenhoven on 19/05/2024.
//

#include "Entity.h"

Entity::Entity(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, glm::vec3 rotation, float mass,
               float frictionConstant) : Drawable(position, vec3(1), rotation)
{
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->mass = mass;
    this->frictionConstant = frictionConstant;
}

Entity::Entity() : Entity(
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        1.0f,
        1.0f
)
{}

void Entity::update(float deltaTime)
{
    // s = ut + 0.5at^2
    this->position = this->position + this->velocity * deltaTime + 0.5f * this->acceleration * deltaTime * deltaTime;
    // v = u + at
    this->velocity = this->velocity + this->acceleration * deltaTime;
    // Apply friction
    this->velocity = this->velocity * ( 1.0f - this->frictionConstant * deltaTime );
    this->acceleration = glm::vec3(0.0f);
}

/**
 * Applies a force to this entity.
 */
void Entity::applyForce(glm::vec3 force, float deltaTime)
{
    this->acceleration = this->acceleration + force / this->mass;
}

void Entity::draw(float deltaTime)
{
    // TODO: Implement.
}
