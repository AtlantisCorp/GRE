//////////////////////////////////////////////////////////////////////
//
//  RenderPipeline.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 18/06/2017.
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

#ifndef GRE_RENDERPIPELINE_H
#define GRE_RENDERPIPELINE_H

#include "Resource.h"
#include "RenderPass.h"
#include "SpecializedResourceManager.h"

GreBeginNamespace

class Renderer ;

//////////////////////////////////////////////////////////////////////
/// @brief Describes an object used to order and render RenderPass
/// for a Renderer. The RenderPass are ordered with an index in a classic
/// std::map , and can be rendered using a normal or a reverse iterator.
///
/// For storage purpose , and as the number of rendering pass should never
/// exceed a too high number , the indexing is made with an uint8_t index ,
/// so rendering pass are 0 to 511.
///
//////////////////////////////////////////////////////////////////////
class RenderPipeline : public Resource
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderPipeline ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderPipeline () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the given pass for the given index.
    //////////////////////////////////////////////////////////////////////
    virtual void setPass ( uint8_t idx , const RenderPassHolder & pass ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a pass with given technique and set it to the given
    /// index.
    //////////////////////////////////////////////////////////////////////
    virtual void setPass ( uint8_t idx , const std::string & name , const std::string & techname ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a pass with given technique.
    //////////////////////////////////////////////////////////////////////
    virtual void setPass ( uint8_t idx , const std::string & name , const TechniqueHolder & technique ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the pass at given index , or an invalid holder.
    //////////////////////////////////////////////////////////////////////
    virtual RenderPassHolder getPass ( uint8_t idx ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the pass at given index , or an invalid holder.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderPassHolder getPass ( uint8_t idx ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Reset a given index.
    //////////////////////////////////////////////////////////////////////
    virtual void reset ( uint8_t idx ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Iterates over the different activated passes and render
    /// them with given renderer.
    //////////////////////////////////////////////////////////////////////
    virtual void render ( const Renderer * renderer ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Reverse - iterates over the different activated passes and
    /// render them with given renderer.
    //////////////////////////////////////////////////////////////////////
    virtual void renderReversed ( const Renderer * renderer ) const ;

protected:

    /// @brief Ordered map of render passes.
    std::map < uint8_t , RenderPassHolder > iPasses ;
};

GRE_MAKE_HOLDER( RenderPipeline ) ;

//////////////////////////////////////////////////////////////////////
/// @brief Manages RenderPipeline resources.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderPipelineManager : public ResourceManagerBase < RenderPipeline >
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderPipelineManager( const std::string & name = "pipelines.manager.default" );

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~RenderPipelineManager() noexcept( false );

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a RenderPipeline and stores it in this manager.
    //////////////////////////////////////////////////////////////////////
    virtual RenderPipelineHolder loadBlank( const std::string & pipelinename );
};

GRE_MAKE_HOLDER( RenderPipelineManager );

GreEndNamespace

#endif
