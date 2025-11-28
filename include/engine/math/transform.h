//
// Created by Luca Warmenhoven on 31/05/2024.
//

#ifndef GAME_ENGINE_TRANSFORMATION_H
#define GAME_ENGINE_TRANSFORMATION_H

#include <glm/vec3.hpp>

class Transform {
public:
  glm::vec3 position;

  union {
    glm::vec3 rotation;
    struct {
      float pitch;
      float yaw;
      float roll;
    };
  };
};

#endif // GAME_ENGINE_TRANSFORMATION_H
