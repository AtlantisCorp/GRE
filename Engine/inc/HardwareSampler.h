//////////////////////////////////////////////////////////////////////
//
//  HardwareSampler.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 08/08/2016.
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

#ifndef HardwareSampler_h
#define HardwareSampler_h

#include "Pools.h"
#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Describes a Sampler that can be used to change the
/// configurations of a Texture to a HardwareProgram.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareSampler : public Resource
{
public:
    
    POOLED(Pools::HdwBuffer)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareSampler(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareSampler() noexcept(false);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the HardwareSampler.
    //////////////////////////////////////////////////////////////////////
    virtual void bind(int textureunit) const = 0;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the HardwareSampler from given Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind(int textureunit) const = 0;
};

/// @brief SpecializedCountedObjectHolder for HardwareSamplerPrivate.
typedef SpecializedCountedObjectHolder<HardwareSampler> HardwareSamplerHolder;

/// @brief SpecializedResourceHolderList for HardwareSamplerPrivate.
typedef SpecializedResourceHolderList<HardwareSampler> HardwareSamplerHolderList;

/// @brief SpecializedResourceUser for HardwareSampler.
typedef SpecializedCountedObjectUser<HardwareSampler> HardwareSamplerUser ;

GreEndNamespace

#endif /* HardwareSampler_h */
