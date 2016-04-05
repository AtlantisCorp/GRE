//
//  Viewport.cpp
//  GRE
//
//  Created by Jacques Tronconi on 09/03/2016.
//
//

#include "Viewport.h"

GreBeginNamespace

Viewport::Viewport(const std::string& name, float top, float left, float width, float height, bool activated)
: _mName(name), _mBorderTop(top), _mBorderLeft(left), _mBorderWidth(width), _mBorderHeight(height), _mActivated(activated), _mScene(Scene::Null)
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

void Viewport::selectScene(const Gre::Scene &scene)
{
    _mScene = scene;
}

bool Viewport::hasScene() const
{
    return !_mScene.expired();
}

const Scene& Viewport::getScene() const
{
    return _mScene;
}

const std::vector<Viewport> Viewport::EmptyVector = std::vector<Viewport>();

GreEndNamespace
