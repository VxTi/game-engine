//
// Created by Luca Warmenhoven on 20/05/2024.
//

#ifndef GRAPHICS_TEST_PLAYER_H
#define GRAPHICS_TEST_PLAYER_H

#include "Entity.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Player : public Entity
{
public:

    Player(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, glm::vec3 rotation, float mass,
           float frictionConstant) : Entity(position, velocity, acceleration, rotation,
                                            mass, frictionConstant)
    {}

    Player() : Entity()
    {}

    void update(float deltaTime);

    const float movementSpeedForce = 10000.0f;

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

    /**
     * Handle input for the player.
     * This will update the acceleration, velocity and position
     * accordingly.
     *
     * @param deltaTime
     */
    void handleInput(GLFWwindow *window, float deltaTime);
};

#endif //GRAPHICS_TEST_PLAYER_H
