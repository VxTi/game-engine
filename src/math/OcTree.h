//
// Created by Luca Warmenhoven on 30/05/2024.
//

#ifndef GRAPHICS_TEST_OCTREE_H
#define GRAPHICS_TEST_OCTREE_H


#include <cstddef>

enum octree_octant_t {
    OCTREE_OCTET_FRONT_BOTTOM_LEFT = 0,
    OCTREE_OCTET_FRONT_BOTTOM_RIGHT = 1,
    OCTREE_OCTET_FRONT_TOP_LEFT = 2,
    OCTREE_OCTET_FRONT_TOP_RIGHT = 3,
    OCTREE_OCTET_BACK_BOTTOM_LEFT = 4,
    OCTREE_OCTET_BACK_BOTTOM_RIGHT = 5,
    OCTREE_OCTET_BACK_TOP_LEFT = 6,
    OCTREE_OCTET_BACK_TOP_RIGHT = 7
};

template <typename T>
struct octree_data_t {
    T *data;
    size_t dataLength;
};

template <typename T>
struct octree_node_t {
    octree_node_t *children[8];
    octree_data_t<T> *nodeData;
    unsigned int depth;
    unsigned int arrayIndex; // The index of the node in the 'nodeData' array.
};

template <typename T>
class OcTree
{
private:
    unsigned int dataCursor;
    T *data;

    unsigned long capacity;
    unsigned int depth;
    unsigned int size;

    octree_node_t<T> *root;

    void createBranches(double x, double y, double z);
    void createBranch(octree_node_t<T> node, double x, double y, double z);

public:
    OcTree<T>(unsigned int size, unsigned int depth, unsigned long capacity);

    /**
     * Whether the given coordinate is within the bounds of the octree.
     */
    bool coordinatesWithin(double x, double y, double z);

    /**
     * Get the octant of the given coordinate.
     */
    octree_octant_t getOctant(double x, double y, double z);

    /**
     * Insert nodeData into the octree.
     */
    void insert(double x, double y, double z, T *referenceData, size_t referenceDataLength);

    void get(double x, double y, double z, octree_data_t<T> *data);

    octree_node_t<T> *getNode(double x, double y, double z);

    bool hasNode(double x, double y, double z);

    void remove(octree_node_t<T> *node);

    ~OcTree();
};


#endif //GRAPHICS_TEST_OCTREE_H
