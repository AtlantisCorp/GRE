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

//////////////////////////////////////////////////////////////////////
/// @brief States a key can be.
enum class KeyState : int
{
    Released = 0 ,
    Pressed  = 1
};

/// @brief An enumeration of every keys in the Engine available to the
/// user.
enum class Key : int
{
    Null = 0,
    
    K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    Apostrophe,
    Backslash,
    Comma,
    Equal,
    GraveAccent,
    LeftBracket,
    Minus,
    Period,
    RightBracket,
    Semicolon,
    Slash,
    World1,
    
    Backspace,
    CapsLock,
    Delete,
    Down,
    End,
    Enter,
    Escape,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18,
    F19, F20,
    Home,
    Insert,
    Left,
    LeftAlt,
    LeftControl,
    LeftShift,
    LeftSuper,
    Menu,
    NumLock,
    PageDown,
    PageUp,
    Right,
    RightAlt,
    RightControl,
    RightShift,
    RightSuper,
    Space,
    Tab,
    Up,
    
    KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,
    KPAdd, KPDecimal, KPDivide, KPEnter, KPEqual, KPMultiply, KPSubstract
};

/// @brief Defines some modifiers that can be handled in the same time of a key.
enum class KeyModifier : int
{
    Shift   = 0x0001,
    Control = 0x0002,
    Alt     = 0x0004,
    Super   = 0x0008
};

GreEndNamespace
#endif
