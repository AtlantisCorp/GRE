//
//  Color.cpp
//  GRE
//
//  Created by Jacques Tronconi on 25/11/2015.
//
//

#include "Color.h"

GRE_BEGIN_NAMESPACE

bool Color::operator==(const Color &rhs) const
{
    return red   == rhs.red    &&
           green == rhs.green  &&
           blue  == rhs.blue   &&
           alpha == rhs.alpha;
}

bool Color::operator!=(const Color &rhs) const
{
    return !(*this == rhs);
}

Color Color::White = {1.0f, 1.0f, 1.0f, 1.0f};
Color Color::Black = {0.0f, 0.0f, 0.0f, 1.0f};
Color Color::Red   = {1.0f, 0.0f, 0.0f, 1.0f};
Color Color::Green = {0.0f, 1.0f, 0.0f, 1.0f};
Color Color::Blue  = {0.0f, 0.0f, 1.0f, 1.0f};

GRE_END_NAMESPACE
