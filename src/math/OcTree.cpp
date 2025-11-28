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

  auto leaf = this->traverseToLeaf(x, y, z, true);
  if (leaf == nullptr) {
    return Result::ERROR;
  }

  leaf.elements.push_back(element);

  return Result::OK;
}

template <typename T>
OcTreeNode<T> &OcTree<T>::traverseToLeaf(double x, double y, double z,
                                         bool createIfMissing) {
  OcTreeNode<T> *currentNode = this;
  double minX = -0.5, maxX = 0.5;
  double minY = -0.5, maxY = 0.5;
  double minZ = -0.5, maxZ = 0.5;

  for (unsigned char depth = 0; depth < this->partitionDepth; ++depth) {
    double midX = (minX + maxX) / 2.0;
    double midY = (minY + maxY) / 2.0;
    double midZ = (minZ + maxZ) / 2.0;

    unsigned char index = 0;
    if (x >= midX) {
      index |= 4;
      minX = midX;
    } else {
      maxX = midX;
    }
    if (y >= midY) {
      index |= 2;
      minY = midY;
    } else {
      maxY = midY;
    }
    if (z >= midZ) {
      index |= 1;
      minZ = midZ;
    } else {
      maxZ = midZ;
    }

    if (currentNode->children[index] == nullptr) {
      if (createIfMissing) {
        currentNode->children[index] = new OcTreeNode<T>();
      } else {
        return nullptr;
      }
    }
    currentNode = currentNode->children[index];
  }

  return *currentNode;
}

template <typename T>
bool OcTree<T>::intersects(double x, double y, double z, double maxDistance) {
  if (!this->isWithinBounds(x, y, z)) {
    return false;
  }

  auto node = this->traverseToLeaf(x, y, z, false);

  if (node == nullptr)
    return false;

  for (const auto &element : node->elements) {
    double dx = element.x - x;
    double dy = element.y - y;
    double dz = element.z - z;
    double dSq = dx * dx + dy * dy + dz * dz;
 
    if (dSq <= maxDistance * maxDistance) {
      return true;
    }
  }
  return false;
}
