//
//  Color.h
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#ifndef GRE_Color_h
#define GRE_Color_h

#include "Version.h"

GRE_BEGIN_NAMESPACE

/// @brief Defines a color in the Engine.
struct DLL_PUBLIC Color
{
    float red;
    float green;
    float blue;
    float alpha;
    
    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
};

typedef struct Color Color;

GRE_END_NAMESPACE
#endif
