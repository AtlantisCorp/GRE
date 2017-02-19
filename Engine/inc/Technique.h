//////////////////////////////////////////////////////////////////////
//
//  Technique.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/02/2017.
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

#ifndef Technique_h
#define Technique_h

#include "Pools.h"
#include "Resource.h"
#include "Pass.h"
#include "Camera.h"
#include "SceneNode.h"
#include "Viewport.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Holds generics informations about how to draw a RenderScene.
//////////////////////////////////////////////////////////////////////
class Technique : public Resource
{
public:
    
    POOLED ( Pools::Render )
    
    /// @brief
    typedef SpecializedCountedObjectHolder < Technique > TechniqueHolder ;
    
    /// @brief
    typedef SpecializedResourceHolderList < Technique > TechniqueHolderList ;
    
    /// @brief
    typedef SpecializedCountedObjectUser < Technique > TechniqueUser ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Technique ( const std::string & name = "DefaultTechnique" ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Technique () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the RenderPasses this technique holds.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < RenderPassHolder > & getPasses () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a RenderPass to this technique.
    //////////////////////////////////////////////////////////////////////
    virtual void addPass ( const RenderPassHolder& pass ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Camera use for this Technique.
    //////////////////////////////////////////////////////////////////////
    virtual const CameraHolder & getCamera () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iCamera'.
    //////////////////////////////////////////////////////////////////////
    virtual void setCamera ( const CameraUser& camera ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iViewport'.
    //////////////////////////////////////////////////////////////////////
    virtual const Viewport & getViewport () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iViewport'.
    //////////////////////////////////////////////////////////////////////
    virtual void setViewport ( const Viewport& viewport ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iExclusive'.
    //////////////////////////////////////////////////////////////////////
    virtual bool isExclusive () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iExclusive'.
    //////////////////////////////////////////////////////////////////////
    virtual void setExclusive ( bool value ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iRenderedNodes'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < RenderNodeHolder > & getNodes () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief True if has sub techniques.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasSubtechniques () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iSubtechniques'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < TechniqueHolder > & getSubtechniques () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iActivated'.
    //////////////////////////////////////////////////////////////////////
    virtual bool isActivated () const ;
    
protected:
    
    /// @brief
    std::vector < RenderPassHolder > iPasses ;
    
    /// @brief
    CameraHolder iCamera ;
    
    /// @brief
    Viewport iViewport ;
    
    /// @brief A Technique is 'exclusive' if only some special RenderNodes are affected.
    bool iExclusive ;
    
    /// @brief
    std::vector < RenderNodeHolder > iRenderedNodes ;
    
    /// @brief
    std::vector < TechniqueHolder > iSubtechniques ;
    
    /// @brief True if this Technique can be used to draw the RenderScene.
    bool iActivated ;
};

/// @brief
typedef SpecializedCountedObjectHolder < Technique > TechniqueHolder ;

/// @brief
typedef SpecializedResourceHolderList < Technique > TechniqueHolderList ;

/// @brief
typedef SpecializedCountedObjectUser < Technique > TechniqueUser ;

GreEndNamespace

#endif /* Technique_h */
