//
// Created by Luca Warmenhoven on 30/05/2024.
//

#include "QuadTree.h"

template<typename T>
QuadTree<T>::QuadTree(float x, float y, float width, float height, int level, int maxLevel)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->level = level;
    this->maxLevel = maxLevel;
    this->objects = std::vector<T *>();
    this->nodes = {nullptr, nullptr, nullptr, nullptr};
}

template<typename T>
QuadTree<T>::~QuadTree()
{
    this->clear();
}

template<typename T>
int QuadTree<T>::getIndex(float x, float y)
{
    int index = -1;
    float verticalMidpoint = this->x + (this->width / 2);
    float horizontalMidpoint = this->y + (this->height / 2);

    bool topQuadrant = y < horizontalMidpoint && y + height < horizontalMidpoint;
    bool bottomQuadrant = y > horizontalMidpoint;

    if ( x < verticalMidpoint && x + width < verticalMidpoint ) {
        if ( topQuadrant ) {
            index = 1;
        } else if ( bottomQuadrant ) {
            index = 2;
        }
    } else if ( x > verticalMidpoint ) {
        if ( topQuadrant ) {
            index = 0;
        } else if ( bottomQuadrant ) {
            index = 3;
        }
    }

    return index;
}

template<typename T>
void QuadTree<T>::split()
{
    float subWidth = this->width / 2;
    float subHeight = this->height / 2;
    float x = this->x;
    float y = this->y;

    this->nodes[0] = new QuadTree(x + subWidth, y, subWidth, subHeight, this->level + 1, this->maxLevel);
    this->nodes[1] = new QuadTree(x, y, subWidth, subHeight, this->level + 1, this->maxLevel);
    this->nodes[2] = new QuadTree(x, y + subHeight, subWidth, subHeight, this->level + 1, this->maxLevel);
    this->nodes[3] = new QuadTree(x + subWidth, y + subHeight, subWidth, subHeight, this->level + 1, this->maxLevel);
}

template<typename T>
void QuadTree<T>::insert(T *object)
{
    if ( this->nodes[0] != nullptr ) {
        int index = this->getIndex(object->x, object->y);

        if ( index != -1 ) {
            this->nodes[index]->insert(object);
            return;
        }
    }

    this->objects.push_back(object);

    if ( this->objects.size() > this->maxLevel && this->level < this->maxLevel ) {
        if ( this->nodes[0] == nullptr ) {
            this->split();
        }

        int i = 0;
        while ( i < this->objects.size() ) {
            int index = this->getIndex(this->objects[i]->x, this->objects[i]->y);
            if ( index != -1 ) {
                this->nodes[index]->insert(this->objects[i]);
                this->objects.erase(this->objects.begin() + i);
            } else {
                i++;
            }
        }
    }
}

template<typename T>
void QuadTree<T>::remove(T *object)
{
    if ( this->nodes[0] != nullptr ) {
        int index = this->getIndex(object->x, object->y);

        if ( index != -1 ) {
            this->nodes[index]->remove(object);
            return;
        }
    }

    for ( int i = 0; i < this->objects.size(); i++ ) {
        if ( this->objects[i] == object ) {
            this->objects.erase(this->objects.begin() + i);
            return;
        }
    }
}

template<typename T>
std::vector<T *> QuadTree<T>::query(float x, float y, float width, float height)
{
    std::vector<T *> result = std::vector<T *>();
    int index = this->getIndex(x, y);

    if ( index != -1 && this->nodes[0] != nullptr ) {
        result = this->nodes[index]->query(x, y, width, height);
    }

    for ( T *object: this->objects ) {
        result.push_back(object);
    }

    return result;
}

template<typename T>
void QuadTree<T>::clear()
{

}
