//
// Created by Luca Warmenhoven on 24/05/2024.
//

#ifndef GRAPHICS_TEST_TEXTURE_H
#define GRAPHICS_TEST_TEXTURE_H


#include "GLFW/glfw3.h"

class Texture
{
public:
    GLuint textureId;
    GLuint width;
    GLuint height;

    static Texture loadFromResource(const char *resourceRelativePath);

    void bind();

    void unbind();


};


#endif //GRAPHICS_TEST_TEXTURE_H
