//
// Created by Luca Warmenhoven on 19/05/2024.
//

#ifndef GRAPHICS_TEST_ENTITY_H
#define GRAPHICS_TEST_ENTITY_H

#include <glm/glm.hpp>

class AbstractBody
{
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    float mass;
    float frictionConstant;
    float yaw, pitch;

    /**
     * Constructor for creating a new entity
     *
     * @param position The position of the entity
     * @param velocity The velocity of the entity
     * @param acceleration The acceleration of the entity
     * @param rotation The rotation of the entity
     * @param mass The mass of the entity
     * @param frictionConstant The friction constant of the entity
     * @param yaw The yaw of the entity
     * @param pitch The pitch of the entity
     */
    AbstractBody(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, glm::vec3 rotation, float mass,
                 float frictionConstant, float yaw, float pitch);

    /**
     * Default constructor for creating a new entity
     */
    AbstractBody();

    /**
     * Update the entity
     *
     * @param deltaTime The time since the last update
     */
    void update(float deltaTime);

    /**
     * Apply a force to the entity
     *
     * @param force The force to apply
     */
    void applyForce(glm::vec3 force);

};

class Player : public AbstractBody
{
public:

    Player(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, glm::vec3 rotation, float mass,
           float frictionConstant, float yaw, float pitch) : AbstractBody(position, velocity, acceleration, rotation,
                                                                          mass, frictionConstant, yaw, pitch)
    {}

    Player() : AbstractBody() {}

    /**
     * Move the player by the given amount.
     * This will update the acceleration, velocity and position
     * accordingly.
     *
     * @param x
     * @param y
     * @param z
     */
    void move(float x, float y, float z);
};


#endif //GRAPHICS_TEST_ENTITY_H
