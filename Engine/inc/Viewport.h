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

#ifndef GRE_Viewport_h
#define GRE_Viewport_h

#include "Pools.h"
#include "Scene.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A simple Viewport object.
///
/// The Viewport is defined using ratio. This means that for example,
/// if you set a width ratio of 0.5f, when RenderContext calls
/// onBordersChanged() with the RenderContext new width of 400 px, the
/// width given by getSurface().width will be 400*0.5f = 200.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Viewport
{
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a new Viewport structure.
    //////////////////////////////////////////////////////////////////////
    Viewport(const std::string& name, float top = 0.0f, float left = 0.0f, float width = 1.0f, float height = 1.0f, bool activated = true);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destructs the Viewport.
    //////////////////////////////////////////////////////////////////////
    virtual ~Viewport();
    
    Viewport& operator = (const Viewport& rhs);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Viewport. This function should be called by the
    /// RenderContext object.
    //////////////////////////////////////////////////////////////////////
    virtual void onBordersChanged(const Surface& parentSurface);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Name of this Viewport.
    //////////////////////////////////////////////////////////////////////
    const std::string& getName() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Viepwort should be rendered.
    //////////////////////////////////////////////////////////////////////
    bool isActivated() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Surface covered by this Viewport.
    //////////////////////////////////////////////////////////////////////
    const Surface& getSurface() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Selects a SceneManager for this Viewport.
    //////////////////////////////////////////////////////////////////////
    void selectScene(const SceneManager& scene);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'true' if this Viewport has a Scene object linked to.
    //////////////////////////////////////////////////////////////////////
    bool hasScene() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Scene object linked to this Viewport if it has
    /// one, or SceneManager::Null if not.
    //////////////////////////////////////////////////////////////////////
    const SceneManager& getScene() const;
    
    /// @brief An empty const vector of Viewport.
    static const std::list<Viewport> EmptyList;
    
protected:
    
    /// @brief Name of this Viewport.
    std::string _mName;
    
    /// @brief Is this Viewport activated ?
    /// Default value is 'true'.
    bool _mActivated;
    
    /// @brief Width border ratio.
    float _mBorderWidth;
    
    /// @brief Height border ratio.
    float _mBorderHeight;
    
    /// @brief Top margin ratio.
    float _mBorderTop;
    
    /// @brief Left margin ratio.
    float _mBorderLeft;
    
    /// @brief Surface object updated with Viewport::onBordersChanged().
    Surface _mSurface;
    
    /// @brief Holds the Scene that might have been selected by the User to draw this
    /// Viewport object.
    SceneManager _mScene;
};

/// @brief std::list<> for Viewport.
typedef std::list<Viewport> ViewportList;

GreEndNamespace

#endif
