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

GreBeginNamespace

/// @brief An enumeration of every keys in the Engine available to the
/// user.
/// @note The key value is platform-dependent. This value is tested on each
/// platform and reported here. Those values can be errored.
enum class Key
{
#ifdef GrePlatformDarwin
    A         = 0,
    S         = 1,
    D         = 2,
    V         = 9,
    W         = 13,
    T         = 17,
    Esc       = 53,
    ArrowUp   = 126,
    ArrowDown = 125
#endif
};

/// @brief Translate given Key to char.
char KeyToChar ( const Key& key );

GreEndNamespace
#endif
