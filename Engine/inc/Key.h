//
//  Key.h
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#ifndef GRE_Key_h
#define GRE_Key_h

#include "Version.h"

GRE_BEGIN_NAMESPACE

/// @brief An enumeration of every keys in the Engine available to the
/// user.
/// @note The key value is platform-dependent. This value is tested on each
/// platform and reported here. Those values can be errored.
enum class Key
{
#ifdef GRE_OSX
    A         = 0,
    V         = 9,
    T         = 17,
    ArrowUp   = 126,
    ArrowDown = 125
#endif
};

GRE_END_NAMESPACE
#endif
