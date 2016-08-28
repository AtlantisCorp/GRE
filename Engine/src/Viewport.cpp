//////////////////////////////////////////////////////////////////////
//
//  Viewport.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 09/03/2016.
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

#include "Viewport.h"

GreBeginNamespace

Viewport::Viewport(const std::string& name, float top, float left, float width, float height, bool activated)
: _mName(name), _mBorderTop(top), _mBorderLeft(left), _mBorderWidth(width), _mBorderHeight(height), _mActivated(activated), _mScene(RenderScene::Null)
{
    _mSurface.height = 0;
    _mSurface.left = 0;
    _mSurface.top = 0;
    _mSurface.width = 0;
}

Viewport::~Viewport()
{
    
}

Viewport& Viewport::operator= (const Viewport& rhs)
{
    _mName = rhs._mName;
    _mActivated = rhs._mActivated;
    _mBorderWidth = rhs._mBorderWidth;
    _mBorderHeight = rhs._mBorderHeight;
    _mBorderLeft = rhs._mBorderLeft;
    _mBorderTop = rhs._mBorderTop;
    _mSurface = rhs._mSurface;
    _mScene = rhs._mScene;
    return *this;
}

void Viewport::onBordersChanged(const Surface &parentSurface)
{
    _mSurface.height = (typeof(_mSurface.height)) (_mBorderHeight * parentSurface.height);
    _mSurface.width = (typeof(_mSurface.width)) (_mBorderWidth * parentSurface.width);
    _mSurface.top = (typeof(_mSurface.top)) (_mBorderTop * parentSurface.top);
    _mSurface.left = (typeof(_mSurface.left)) (_mBorderLeft * parentSurface.left);
}

const std::string& Viewport::getName() const
{
    return _mName;
}

bool Viewport::isActivated() const
{
    return _mActivated;
}

const Surface& Viewport::getSurface() const
{
    return _mSurface;
}

void Viewport::selectScene(const Gre::RenderScene &scene)
{
    _mScene = scene;
}

bool Viewport::hasScene() const
{
    return !_mScene.isExpired();
}

const RenderScene& Viewport::getScene() const
{
    return _mScene;
}

const std::list<Viewport> Viewport::EmptyList = std::list<Viewport>();

GreEndNamespace
