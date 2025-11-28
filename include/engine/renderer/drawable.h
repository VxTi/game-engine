#include <engine/math/transform.h>

using namespace glm;
/**
 * Class representing a drawable object.
 * The draw method must be overridden when extending this class.
 */
class Drawable : public Transform {
public:
  Drawable(vec3 position, vec3 scale, vec3 rotation);

  virtual void draw(float deltaTime) = 0;

  /**
   * Function that checks whether the object is within the provided frustum.
   * @param frustum The frustum to check against
   * @return Whether the object is within the frustum
   */
  /* TODO: Implement */ /*virtual bool isWithinFrustum(Frustum frustum);*/
};
