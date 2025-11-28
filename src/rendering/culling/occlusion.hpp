//
// Created by Luca Warmenhoven on 02/06/2024.
//

#ifndef GRAPHICS_TEST_OCCLUSION_HPP
#define GRAPHICS_TEST_OCCLUSION_HPP

#include <engine/renderer/culling/frustum.h>
#include <engine/renderer/scene.h>

namespace culling::occlusion {
extern bool isOccluded(Drawable *drawable, Frustum frustum) {
  if (!frustum.isWithin(drawable->position)) {
    return true;
  }
  return false;
}
} // namespace culling::occlusion

#endif // GRAPHICS_TEST_OCCLUSION_HPP
