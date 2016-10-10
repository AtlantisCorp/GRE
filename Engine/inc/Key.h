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
/// user. This list is based on the ASCII representation of characters.

enum class Key
{
    Null = 0 ,
    SOH = 1 ,
    SOT = 2 ,
    EOT = 3 ,
    EOTransmission = 4 ,
    Backspace = 8 ,
    HorizontalTab = 9 ,
    CarriageReturn = 13 ,
    ShiftOut = 14 ,
    ShiftIn = 15 ,
    Escape = 27 ,
    Space = 32 ,
    
    NumberBegin = 48 ,
    Num0 = 48 ,
    Num1 = 49 ,
    Num2 = 50 ,
    Num3 = 51 ,
    Num4 = 52 ,
    Num5 = 53 ,
    Num6 = 54 ,
    Num7 = 55 ,
    Num8 = 56 ,
    Num9 = 57 ,
    NumberEnd = 57 ,
    
    CapitalsBegin = 65 ,
    CapitalsEnd = 90 ,
    
    MinusculsBegin = 97 ,
    MinusculsEnd = 122
};

/// @brief Translate given Key to char.
extern "C" DLL_PUBLIC char KeyToChar ( const Key& key );

/// @brief Translate an ASCII char character to Key .
extern "C" DLL_PUBLIC Key KeyFromAscii ( char key ) ;

GreEndNamespace
#endif
