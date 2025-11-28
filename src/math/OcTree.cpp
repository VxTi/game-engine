#include <engine/math/octree.h>

template <typename T>
bool OcTree<T>::isWithinBounds(double x, double y, double z) {
  return (x >= -0.5 && x <= 0.5) && (y >= -0.5 && y <= 0.5) &&
         (z >= -0.5 && z <= 0.5);
}

template <typename T>
Result OcTree<T>::insert(T element, double x, double y, double z) {
  if (!this->isWithinBounds(x, y, z)) {
    return Result::ERROR;
  }

  

  return Result::OK;
}
