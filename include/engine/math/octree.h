#ifndef ENGINE_MATH_OCTREE_H
#define ENGINE_MATH_OCTREE_H

#include <engine/util/result.h>
#include <vector>

// This arbitrary depth is set to prevent causing
// performance issues with too deep octrees.
#define MAX_PARTITION_DEPTH (8)
#define DEFAULT_PARTITION_DEPTH (4)

template <typename T> class OcTreeElement {
  T value;
  double x;
  double y;
  double z;
};

template <typename T> class OcTreeNode {
private:
  OcTreeNode<T> children[8];
  std::vector<T> elements;

public:
  bool isLeaf();

  bool isRoot();

  OcTreeNode<T>()
      : children{nullptr, nullptr, nullptr, nullptr,
                 nullptr, nullptr, nullptr, nullptr} {}
};

template <typename T> class OcTree : public OcTreeNode<T> {
private:
  /**
   * Determines the max partitioning depth of this octree.
   */
  const unsigned char partitionDepth;

  bool isWithinBounds(double x, double y, double z);

public:
  /**
   * Constructor for the octree.
   * @param depth The max partitioning depth of the octree.
   */
  OcTree(unsigned char depth = DEFAULT_PARTITION_DEPTH)
      : partitionDepth(depth), OcTreeNode<T>() {}

  /**
   * Checks whether the provided point intersects with this octree.
   * @param x The x coordinate of the point, range [-0.5, 0.5]
   * @param y The y coordinate of the point, range [-0.5, 0.5]
   * @param z The z coordinate of the point, range [-0.5, 0.5]
   * @param maxDistance The maximum distance from the point to consider an
   * intersection.
   *
   * @return True if the point intersects with this octree, false otherwise.
   */
  bool intersects(double x, double y, double z, double maxDistance);

  /**
   * Inserts an element into the octree at the specified coordinates.
   * @param element The element to insert.
   * @param x The x coordinate of the element, range [-0.5, 0.5]
   * @param y The y coordinate of the element, range [-0.5, 0.5]
   * @param z The z coordinate of the element, range [-0.5, 0.5]
   *
   * @note If the coordinates are out of bounds, the element will not be
   * inserted.
   */
  Result insert(T element, double x, double y, double z);

  /**
   * Deletes an element from the octree.
   * @param element The element to delete.
   *
   * @return Result indicating success or failure of the deletion.
   */
  Result deleteElement(T element);
};

#endif // ENGINE_MATH_OCTREE_H
