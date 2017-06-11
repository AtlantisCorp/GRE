//////////////////////////////////////////////////////////////////////
//
//  X11Keycodes.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 31/05/2017.
//
//////////////////////////////////////////////////////////////////////
/*
 -----------------------------------------------------------------------------
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
*/

#include "X11Keycodes.h"

// -----------------------------------------------------------------------------
// Internal X11 keycodes variables.

// X11 keysym to Gre::Key.
std::map < int , Gre::Key > keysyms ;

// -----------------------------------------------------------------------------
// Internal X11 keycodes translation functions.

void X11InternalInitKeycodes () 
{
	//////////////////////////////////////////////////////////////////////
	// Populates the keysym array.
	
	keysyms[NoSymbol] 	= Gre::Key::Null ;
	keysyms[XK_0] 		= Gre::Key::K0 ;
	keysyms[XK_1] 		= Gre::Key::K1 ;
	keysyms[XK_2] 		= Gre::Key::K2 ;
	keysyms[XK_3] 		= Gre::Key::K3 ;
	keysyms[XK_4] 		= Gre::Key::K4 ;
	keysyms[XK_5] 		= Gre::Key::K5 ;
	keysyms[XK_6] 		= Gre::Key::K6 ;
	keysyms[XK_7] 		= Gre::Key::K7 ;
	keysyms[XK_8] 		= Gre::Key::K8 ;
	keysyms[XK_9] 		= Gre::Key::K9 ;
	keysyms[XK_A]		= Gre::Key::A ;
	keysyms[XK_B]		= Gre::Key::B ;
	keysyms[XK_C]		= Gre::Key::C ;
	keysyms[XK_D]		= Gre::Key::D ;
	keysyms[XK_E]		= Gre::Key::E ;
	keysyms[XK_F]		= Gre::Key::F ;
	keysyms[XK_G]		= Gre::Key::G ;
	keysyms[XK_H]		= Gre::Key::H ;
	keysyms[XK_I]		= Gre::Key::I ;
	keysyms[XK_J]		= Gre::Key::J ;
	keysyms[XK_K]		= Gre::Key::K ;
	keysyms[XK_L]		= Gre::Key::L ;
	keysyms[XK_M]		= Gre::Key::M ;
	keysyms[XK_N]		= Gre::Key::N ;
	keysyms[XK_O]		= Gre::Key::O ;
	keysyms[XK_P]		= Gre::Key::P ;
	keysyms[XK_Q]		= Gre::Key::Q ;
	keysyms[XK_R]		= Gre::Key::R ;
	keysyms[XK_S]		= Gre::Key::S ;
	keysyms[XK_T]		= Gre::Key::T ;
	keysyms[XK_U]		= Gre::Key::U ;
	keysyms[XK_V]		= Gre::Key::V ;
	keysyms[XK_W]		= Gre::Key::W ;
	keysyms[XK_X]		= Gre::Key::X ;
	keysyms[XK_Y]		= Gre::Key::Y ;
	keysyms[XK_Z]		= Gre::Key::Z ;
	keysyms[XK_apostrophe] = Gre::Key::Apostrophe ;
	keysyms[XK_backslash] = Gre::Key::Backslash ;
	keysyms[XK_comma] 	= Gre::Key::Comma ;
	keysyms[XK_equal] 	= Gre::Key::Equal ;
	keysyms[XK_grave]	= Gre::Key::GraveAccent ;
	keysyms[XK_bracketleft] = Gre::Key::LeftBracket ;
	keysyms[XK_minus] 	= Gre::Key::Minus ;
	keysyms[XK_period]	= Gre::Key::Period ;
	keysyms[XK_bracketright] = Gre::Key::RightBracket ;
	keysyms[XK_semicolon] = Gre::Key::Semicolon ;
	keysyms[XK_slash]	= Gre::Key::Slash ;
	keysyms[XK_BackSpace] = Gre::Key::Backspace ;
	keysyms[XK_Caps_Lock] = Gre::Key::CapsLock ;
	keysyms[XK_Delete] 	= Gre::Key::Delete ;
	keysyms[XK_Down] 	= Gre::Key::Down ;
	keysyms[XK_End] 	= Gre::Key::End ;
	keysyms[XK_Return] 	= Gre::Key::Enter ;
	keysyms[XK_Escape] 	= Gre::Key::Escape ;
	keysyms[XK_F1]		= Gre::Key::F1 ;
	keysyms[XK_F2]		= Gre::Key::F2 ;
	keysyms[XK_F3]		= Gre::Key::F3 ;
	keysyms[XK_F4]		= Gre::Key::F4 ;
	keysyms[XK_F5]		= Gre::Key::F5 ;
	keysyms[XK_F6]		= Gre::Key::F6 ;
	keysyms[XK_F7]		= Gre::Key::F7 ;
	keysyms[XK_F8]		= Gre::Key::F8 ;
	keysyms[XK_F9]		= Gre::Key::F9 ;
	keysyms[XK_F10]		= Gre::Key::F10 ;
	keysyms[XK_F11]		= Gre::Key::F11 ;
	keysyms[XK_F12]		= Gre::Key::F12 ;
	keysyms[XK_F13]		= Gre::Key::F13 ;
	keysyms[XK_F14]		= Gre::Key::F14 ;
	keysyms[XK_F15]		= Gre::Key::F15 ;
	keysyms[XK_F16]		= Gre::Key::F16 ;
	keysyms[XK_F17]		= Gre::Key::F17 ;
	keysyms[XK_F18]		= Gre::Key::F18 ;
	keysyms[XK_F19]		= Gre::Key::F19 ;
	keysyms[XK_F20]		= Gre::Key::F20 ;
	keysyms[XK_Home]	= Gre::Key::Home ;
	keysyms[XK_Insert]	= Gre::Key::Insert ;
	keysyms[XK_Left] 	= Gre::Key::Left ;
	keysyms[XK_Alt_L]	= Gre::Key::LeftAlt ;
	keysyms[XK_Control_L] = Gre::Key::LeftControl ;
	keysyms[XK_Shift_L] = Gre::Key::LeftShift ;
	keysyms[XK_Super_L] = Gre::Key::LeftSuper ;
    keysyms[XK_Home]    = Gre::Key::Menu ;
    keysyms[XK_Num_Lock] = Gre::Key::NumLock;
    keysyms[XK_Page_Up] = Gre::Key::PageUp ;
    keysyms[XK_Page_Down] = Gre::Key::PageDown ;
    keysyms[XK_Right]   = Gre::Key::Right ;
    keysyms[XK_Alt_R]   = Gre::Key::RightAlt ;
    keysyms[XK_Control_R] = Gre::Key::RightControl ;
    keysyms[XK_Shift_R] = Gre::Key::RightShift ;
    keysyms[XK_Super_R] = Gre::Key::RightSuper ;
    keysyms[XK_space]   = Gre::Key::Space ;
    keysyms[XK_Tab]     = Gre::Key::Tab ;
    keysyms[XK_Up]      = Gre::Key::Up ;
    keysyms[XK_KP_0]    = Gre::Key::KP0 ;
    keysyms[XK_KP_1]    = Gre::Key::KP1 ;
    keysyms[XK_KP_2]    = Gre::Key::KP2 ;
    keysyms[XK_KP_3]    = Gre::Key::KP3 ;
    keysyms[XK_KP_4]    = Gre::Key::KP4 ;
    keysyms[XK_KP_5]    = Gre::Key::KP5 ;
    keysyms[XK_KP_6]    = Gre::Key::KP6 ;
    keysyms[XK_KP_7]    = Gre::Key::KP7 ;
    keysyms[XK_KP_8]    = Gre::Key::KP8 ;
    keysyms[XK_KP_9]    = Gre::Key::KP9 ;
    keysyms[XK_KP_Add]  = Gre::Key::KPAdd ;
    keysyms[XK_KP_Decimal] = Gre::Key::KPDecimal ;
    keysyms[XK_KP_Divide] = Gre::Key::KPDivide ;
    keysyms[XK_KP_Enter] = Gre::Key::KPEnter ;
    keysyms[XK_KP_Equal] = Gre::Key::KPEqual ;
    keysyms[XK_KP_Multiply] = Gre::Key::KPMultiply ;
    keysyms[XK_KP_Subtract] = Gre::Key::KPSubstract ;
}

// -----------------------------------------------------------------------------
// X11 keycodes translation functions.

Gre::Key X11TranslateKeyCode( int keycode ) 
{
	auto it = keysyms.find(keycode) ;
	
	if ( it == keysyms.end() )
	return Gre::Key::Null ;
	
	return it->second ;
}

Gre::KeyModifier X11TranslateKeyModifiers( int state )
{
    if ( state == ShiftMask ) return Gre::KeyModifier::Shift ;
    else if ( state == ControlMask ) return Gre::KeyModifier::Control ;
    else if ( state == LockMask ) return Gre::KeyModifier::Alt ;
    
    return (Gre::KeyModifier) 0 ;
}
