//////////////////////////////////////////////////////////////////////
//
//  X11OpenGl.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 17/04/2017.
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

#ifndef GRE_X11OPENGL_H
#define GRE_X11OPENGL_H

//////////////////////////////////////////////////////////////////////
// Includes Gre::ResourceManager , OpenGl , and GLEW extensions manager.
// Notes the resource manager may includes every other GRE files needed.
// X11 is also included.

#include <ResourceManager.h>

#include <GL/glew.h>
#include <GL/glx.h>
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>

//////////////////////////////////////////////////////////////////////
// Disable compilation on other platforms than linux.

#ifndef GrePlatformUnix
# 	error "This Plugin must be compiled only on unix-like platform."
#endif // GrePlatformLinux

#endif // GRE_X11OPENGL_H
