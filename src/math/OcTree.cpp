//
// Created by Luca Warmenhoven on 30/05/2024.
//

#include <cstdlib>
#include "OcTree.h"

template<typename T>
OcTree<T>::OcTree(unsigned int size, unsigned int depth, unsigned long capacity)
{
    this->size = size;
    this->depth = depth;
    this->data = (T *) malloc(sizeof(T) * capacity);
    this->capacity = capacity;
    this->dataCursor = 0;
    this->root = new octree_node_t<T>();
    this->root->nodeData = new octree_data_t<T>();
    this->root->nodeData->data = nullptr;
    this->root->nodeData->dataLength = 0;
}

/**
 * Whether the given coordinate is within the bounds of the octree.
 */
template<typename T>
bool OcTree<T>::coordinatesWithin(double x, double y, double z)
{
    return x >= 0 && x < this->size && y >= 0 && y < this->size && z >= 0 && z < this->size;
}

/**
 * Get the octant of the given coordinate.
 */
template<typename T>
octree_octant_t OcTree<T>::getOctant(double x, double y, double z)
{
    if ( !coordinatesWithin(x, y, z) )
        return OCTREE_OCTET_FRONT_BOTTOM_LEFT;

    double half = this->size / 2;

    if ( x < half ) {
        if ( y < half ) {
            if ( z < half ) {
                return OCTREE_OCTET_FRONT_BOTTOM_LEFT;
            } else {
                return OCTREE_OCTET_BACK_BOTTOM_LEFT;
            }
        } else {
            if ( z < half ) {
                return OCTREE_OCTET_FRONT_TOP_LEFT;
            } else {
                return OCTREE_OCTET_BACK_TOP_LEFT;
            }
        }
    } else {
        if ( y < half ) {
            if ( z < half ) {
                return OCTREE_OCTET_FRONT_BOTTOM_RIGHT;
            } else {
                return OCTREE_OCTET_BACK_BOTTOM_RIGHT;
            }
        } else {
            if ( z < half ) {
                return OCTREE_OCTET_FRONT_TOP_RIGHT;
            } else {
                return OCTREE_OCTET_BACK_TOP_RIGHT;
            }
        }
    }
}

template<typename T>
void OcTree<T>::createBranch(octree_node_t<T> node, double x, double y, double z)
{
    if ( !node || !coordinatesWithin(x, y, z))
        return;
    // If the node doesn't have children yet, create them.
    if ( node->children == nullptr)
    {
        node->children = new octree_node_t<T> *[8];

        // Fill up the child nodes with empty nodeData.
        for ( int i = 0; i < 8; i++ ) {
            node->children[i] = new octree_node_t<T>();
            node->children[i]->nodeData = new octree_data_t<T>();
            node->children[i]->nodeData->nodeData = nullptr;
            node->children[i]->nodeData->dataLength = 0;
        }
    }
}

/**
 * Whether the octree has a node at the given coordinates.
 */
template<typename T>
bool OcTree<T>::hasNode(double x, double y, double z)
{
    if ( !coordinatesWithin(x, y, z) || this->root == nullptr)
        return false;

    int currentDepth = this->depth;
    octree_octant_t octant = getOctant(x, y, z);
    octree_node_t<T> *node = this->root;

    for ( ; currentDepth >= 0; currentDepth-- ) {
        if ( node->children == nullptr )
            return false;

        if ( currentDepth <= 0 )
            return node->nodeData->data != nullptr;

        node = node->children[octant];

        // Subdivide coordinates for next iteration
        x = (((int) x) / 2) * 2;
        y = (((int) y) / 2) * 2;
        z = (((int) z) / 2) * 2;

        octant = getOctant(x, y, z);
    }
}

/*
 * Inserts referenceData into the octree.
 * The referenceData is stored in the octree's 'referenceData' field.
 * The retrieved referenceData is an object that contains a pointer to the location of
 * the referenceData in the octree and the length of the referenceData.
 */
template<typename T>
void OcTree<T>::insert(double x, double y, double z, T *referenceData, size_t dataLength)
{
    // Check if the coordinates are within the octree
    if ( !coordinatesWithin(x, y, z) )
        return;

    // Prevent overflow
    if ( this->dataCursor >= this->capacity )
    {
        // TODO: Add error handling
        return;
    }

    octree_octant_t octant = getOctant(x, y, z);
    octree_node_t<T> *node = this->root;
    int currentDepth = this->depth;

    for (; currentDepth >= 0; currentDepth-- ) {
        createBranch(node, x, y, z); // Ensure branch existence
        if ( currentDepth <= 0 ) {
            node->nodeData->data = referenceData;
            node->nodeData->dataLength = dataLength;
            node->nodeData->arrayIndex = this->dataCursor;
            break;
        }
        node = node->children[octant];

        // Subdivide coordinates for next iteration
        x = (((int) x) / 2) * 2;
        y = (((int) y) / 2) * 2;
        z = (((int) z) / 2) * 2;
        octant = getOctant(x, y, z);
    }

    // Insert into the octree
    memcpy(this->data[this->dataCursor], referenceData, dataLength);
    this->dataCursor += dataLength;
}

template<typename T>
octree_node_t<T> *OcTree<T>::getNode(double x, double y, double z)
{
    if ( !coordinatesWithin(x, y, z) || this->root == nullptr)
        return nullptr;

    int currentDepth = this->depth;
    octree_octant_t octant = getOctant(x, y, z);
    octree_node_t<T> *node = this->root;

    for ( ; currentDepth >= 0; currentDepth-- ) {
        if ( node->children == nullptr )
            return nullptr;

        if ( currentDepth <= 0 )
            return node;

        node = node->children[octant];
        octant = getOctant(x, y, z);
    }
}

/**
 * Get the data at the given coordinates and store it in dstData.
 */
template<typename T>
void OcTree<T>::get(double x, double y, double z, octree_data_t<T> *dstData)
{
    octree_node_t<T> *node = getNode(x, y, z);
    if ( node == nullptr )
        return;

    *dstData->data = *node->nodeData->data;
}

template <typename T>
void OcTree<T>::remove(octree_node_t<T> *node)
{
    if ( node == nullptr )
        return;

    // Remove the nodeData from the node
    node->nodeData->data = nullptr;
    node->nodeData->dataLength = 0;

    // Remove the children
    for ( int i = 0; i < 8; i++ ) {
        remove(node->children[i]);
    }

    // Remove the node
    free(node->nodeData);
    free(node);

    // Remove node from the nodeData array
    if ( this->data[node->arrayIndex] != nullptr )
    {
        this->data[node->arrayIndex] = nullptr;
        this->dataCursor--;
    }
}

template<typename T>
OcTree<T>::~OcTree()
{
    free(this->data);
    delete this->root;
}