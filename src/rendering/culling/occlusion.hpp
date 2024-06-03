//
// Created by Luca Warmenhoven on 02/06/2024.
//

#ifndef GRAPHICS_TEST_OCCLUSION_HPP
#define GRAPHICS_TEST_OCCLUSION_HPP


#include "../renderer.h"
#include "frustum.h"

namespace culling::occlusion
{
    extern bool isOccluded(Drawable* drawable, Frustum frustum)
    {
        if ( !frustum.isWithin(drawable->position) )
        {
            return true;
        }
    }
}


#endif //GRAPHICS_TEST_OCCLUSION_HPP
