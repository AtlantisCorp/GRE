//////////////////////////////////////////////////////////////////////
//
//  RenderingApiDescriptor.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 21/08/2016.
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

#ifndef RenderingApiDescriptor_h
#define RenderingApiDescriptor_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Summarize all informations that can be passed to a
/// RendererLoader or a RendererManager in order to provide the desired
/// Renderer.
///
/// This Object can be filled by a user and given to RendererLoader using
/// 'RendererLoader::load()' function to certifiate the Loader will
/// produce a Renderer with every desired options.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC RenderingApiDescriptor
{
public:
    
    POOLED(Pools::Default)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    RenderingApiDescriptor();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~RenderingApiDescriptor();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Name the Api should have.
    //////////////////////////////////////////////////////////////////////
    const std::string& getApiName() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iApiName' property.
    //////////////////////////////////////////////////////////////////////
    void setApiName ( const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iMinVersion' property.
    //////////////////////////////////////////////////////////////////////
    const std::string& getMinVersion() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iMinVersion' property.
    //////////////////////////////////////////////////////////////////////
    void setMinVersion ( const std::string& minversion );
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the 'iMaxVersion' property.
    //////////////////////////////////////////////////////////////////////
    const std::string& getMaxVersion() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the 'iMaxVersion' property.
    //////////////////////////////////////////////////////////////////////
    void setMaxVersion ( const std::string& maxversion );
    
private:
    
    /// @brief Api name.
    std::string iApiName;
    
    /// @brief Api minimum Version.
    std::string iMinVersion;
    
    /// @brief Api maximum Version.
    std::string iMaxVersion;
};

GreEndNamespace

#endif /* RenderingApiDescriptor_h */
