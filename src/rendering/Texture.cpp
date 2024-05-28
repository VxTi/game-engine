//
// Created by Luca Warmenhoven on 24/05/2024.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../include/stb/stb_image.h"
#include "Rendering.h"

Texture Texture::loadFromResource(const char *resourceRelativePath)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);
    stbi_uc *image_data = stbi_load(resourceRelativePath, &width, &height, &channels, 0);

    if ( !image_data )
        throw std::runtime_error(stbi_failure_reason());

    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.textureId);
    glBindTexture(GL_TEXTURE_2D, texture.textureId);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int format = channels == 3 ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image_data);

    return texture;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, this->textureId);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}