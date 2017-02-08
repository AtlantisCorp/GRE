//////////////////////////////////////////////////////////////////////
//
//  RenderingQuery.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 14/11/2016.
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

#ifndef RenderingQuery_h
#define RenderingQuery_h

#include "Pools.h"
#include "Scene.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A Query sent by the RenderScene to the Renderer to prepare
/// the rendering informations.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderingQuery : public Lockable
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderingQuery ( ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~RenderingQuery ( ) noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iViewport' field.
    //////////////////////////////////////////////////////////////////////
    void setViewport ( const Viewport & v ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iViewport' field.
    //////////////////////////////////////////////////////////////////////
    const Viewport & getViewport ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iRenderScene' field.
    //////////////////////////////////////////////////////////////////////
    void setRenderScene ( const RenderSceneUser & scene ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iRenderScene' field.
    //////////////////////////////////////////////////////////////////////
    const RenderSceneUser & getRenderScene ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iRenderPass'.
    //////////////////////////////////////////////////////////////////////
    void setRenderPass ( const RenderPassUser& pass ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iCamera'.
    //////////////////////////////////////////////////////////////////////
    void setCamera ( const CameraUser & camera ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iProgram'.
    //////////////////////////////////////////////////////////////////////
    void setHardwareProgram ( const HardwareProgramUser& program ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iElapsed'.
    //////////////////////////////////////////////////////////////////////
    void setElapsedTime ( const Duration& elapsed ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iRenderedNodes'.
    //////////////////////////////////////////////////////////////////////
    void setRenderedNodes ( const std::vector < RenderNodeHolder > & nodes ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iProgram'.
    //////////////////////////////////////////////////////////////////////
    const HardwareProgramUser& getHardwareProgram () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iCamera'.
    //////////////////////////////////////////////////////////////////////
    const CameraUser& getCamera () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iRenderedNodes'.
    //////////////////////////////////////////////////////////////////////
    const std::vector < RenderNodeHolder > & getRenderedNodes () const ;

protected:
    
    /// @brief The Viewport the Renderer has to draw in the RenderContext.
    Viewport iViewport ;
    
    /// @brief The RenderScene the Renderer has to draw in the Viewport.
    RenderSceneUser iRenderScene ;
    
    /// @brief RenderPass to draw.
    RenderPassUser iRenderPass ;
    
    /// @brief
    CameraUser iCamera ;
    
    /// @brief
    HardwareProgramUser iProgram ;
    
    /// @brief
    Duration iElapsed ;
    
    /// @brief
    std::vector < RenderNodeHolder > iRenderedNodes ;
};

GreEndNamespace

#endif /* RenderingQuery_h */
