//
// Created by Luca Warmenhoven on 30/05/2024.
//

#ifndef GRAPHICS_TEST_QUADTREE_H
#define GRAPHICS_TEST_QUADTREE_H

#include <vector>

template <typename T>
class QuadTree
{
private:
    float x, y, width, height;
    int level, maxLevel;
    std::vector<T *> objects;
    QuadTree *nodes[4];

    int getIndex(float x, float y);
    void split();

public:
    QuadTree(float x, float y, float width, float height, int level, int maxLevel);
    ~QuadTree();

    void insert(T *object);
    void remove(T *object);
    void clear();
    std::vector<T *> query(float x, float y, float width, float height);
};


#endif //GRAPHICS_TEST_QUADTREE_H
