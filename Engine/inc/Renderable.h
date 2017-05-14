//////////////////////////////////////////////////////////////////////
//
//  Renderable.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 18/04/2017.
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

#ifndef GRE_RENDERABLE_H
#define GRE_RENDERABLE_H

#include "TechniqueParamBinder.h"
#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Renderable is an object mixing the Resource and the TechniqueParamBinder
/// class , which means it is an object able to use a Technique.
///
/// A Renderable object can also holds preprocessing and postprocessing
/// techniques. Those are responsible for custom per-object pre/post processing
/// using custom techniques. When a Renderpass use this object , it will use the
/// preprocessing techniques in the order specified by the user and then
/// the postprocessing techniques (after using the main technique). A Renderable
/// even can set that no technique should be used as main technique if the
/// flag 'iNoMainTechnique' is set to true.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Renderable : public Resource , public TechniqueParamBinder
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Renderable ( const std::string & name = std::string() ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Renderable () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    bool hasNoMainTechnique () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void setNoMainTechnique ( bool value ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void addPreProcessTechnique ( const TechniqueHolder & technique ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const TechniqueHolderList & getPreProcessTechniques () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void clearPreProcessTechniques () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void addPostProcessTechnique ( const TechniqueHolder & technique ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    const TechniqueHolderList & getPostProcessTechniques () const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    void clearPostProcessTechniques () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

protected:

    /// @brief True if the main renderpass technique that render this object
    /// should not be used to draw this object. Notes the first postprocessing
    /// or the last preprocessing can be considered as the 'main' technique of
    /// this object if the renderpass's main technique is discarded.
    bool iNoMainTechnique ;

    /// @brief Retains preprocessing techniques used for this renderable. Notes
    /// those will be called after the renderpass's preprocessing techniques.
    TechniqueHolderList iPreProcessTechniques ;

    /// @brief Retains postprocessing techniques used for this renderable. Notes
    /// those will be called before the renderpass's postprocessing techniques.
    TechniqueHolderList iPostProcessTechniques ;
};

/// @brief
typedef Holder < Renderable > RenderableHolder ;

/// @Brief
typedef SpecializedResourceHolderList < Renderable > RenderableHolderList ;

GreEndNamespace

#endif // GRE_RENDERABLE_H
