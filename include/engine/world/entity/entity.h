//
// Created by Luca Warmenhoven on 19/05/2024.
//

#ifndef GRAPHICS_TEST_ENTITY_H
#define GRAPHICS_TEST_ENTITY_H

#include <engine/math/transform.h>
#include <glm/glm.hpp>

using namespace glm;

class Entity : public Transform {
public:
  vec3 velocity;
  vec3 acceleration;
  vec3 position;

  float mass;
  float frictionConstant;

  /**
   * Constructor for creating a new entity
   *
   * @param position The position of the entity
   * @param velocity The velocity of the entity
   * @param acceleration The acceleration of the entity
   * @param rotation The rotation of the entity
   * @param mass The mass of the entity
   * @param frictionConstant The friction constant of the entity
   */
  Entity(vec3 position, vec3 velocity, vec3 acceleration, vec3 rotation,
         float mass, float frictionConstant);

  /**
   * Default constructor for creating a new entity
   */
  Entity();

  /**
   * Update the entity
   *
   * @param deltaTime The time since the last update
   */
  virtual void update(float deltaTime);

  /**
   * Apply a force to the entity
   *
   * @param force The force to apply, given in Newtons
   */
  void applyForce(vec3 force, float deltaTime);

  /**
   * Method for rendering this entity in the world.
   */
  void draw(float deltaTime);
};

#endif // GRAPHICS_TEST_ENTITY_H
