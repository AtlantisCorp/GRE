//////////////////////////////////////////////////////////////////////
//
//  Pass.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/01/2016.
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

#ifndef GRE_Pass_h
#define GRE_Pass_h

#include "Pools.h"
#include "Resource.h"
#include "HardwareProgram.h"

GreBeginNamespace

/// @brief Defines a unique identifier. This identifier is use to know
/// the order the RenderPass should have when rendering. This is very important , as
/// for example transparent objects should be rendered after opaque objects.
typedef uint32_t RenderPassIdentifier ;

//////////////////////////////////////////////////////////////////////
/// @brief Defines a RenderPass.
///
/// A RenderPass is a step in the rendering process to render a
/// RenderScene. The RenderScene is rendered using multiple RenderPass, as
/// layers. Those RenderPass can render every RenderNodes of the Scene , or
/// just particular nodes as transparents , shadows , etc ...
///
/// By creating new RenderPass with different actions , a user can effectively
/// change the rendering process and customize it.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderPass : public Resource
{
public:
    
    POOLED ( Pools::Referenced )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderPass ( const RenderPassIdentifier & identifier ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderPass ( const std::string & name , const RenderPassIdentifier & identifier ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderPass () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iIdentifier'.
    //////////////////////////////////////////////////////////////////////
    const RenderPassIdentifier & getPassIdentifier () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iActivated'.
    //////////////////////////////////////////////////////////////////////
    void setActivated ( bool value ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iActivated'.
    //////////////////////////////////////////////////////////////////////
    bool isActivated ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iProgram'.
    //////////////////////////////////////////////////////////////////////
    const HardwareProgramUser& getHardwareProgram () const ;
    
protected:
    
    /// @brief The RenderPass Identifier.
    /// This number identifies the order this RenderPass should be processed by the
    /// Renderer. The RenderPasses are processed from 0 to the last one.
    RenderPassIdentifier iIdentifier ;
    
    /// @brief True if this RenderPass is activated for rendering.
    bool iActivated ;
    
    /// @brief
    HardwareProgramUser iProgram ;
    
};

/// @brief SpecializedCountedObjectHolder for RenderPass.
typedef SpecializedCountedObjectHolder<RenderPass> RenderPassHolder ;

/// @brief SpecializedResourceHolderList for RenderPass.
typedef SpecializedResourceHolderList<RenderPass> RenderPassHolderList ;

/// @brief SpecializedCountedObjectUser for RenderPass.
typedef SpecializedCountedObjectUser<RenderPass> RenderPassUser ;

GreEndNamespace

#endif
