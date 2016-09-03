//////////////////////////////////////////////////////////////////////
//
//  ResourcePath.h
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

#ifndef ResourcePath_h
#define ResourcePath_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Enumerates the ResourceDirectory a Resource can be.
///
/// When using ResourceDirectoryType::Custom , the ResourceManager
/// will use the callback function 'iCustomFindFunction'. You can
/// change this function by using ResourceManager::setCustomFindCallback().
///
/// Most of the objects that can be loaded by a Manager normally have
/// a ResourceDirectoryType.
///
//////////////////////////////////////////////////////////////////////
enum class ResourceDirectoryType
{
    /// @brief Unknown ResourceDirectory. If this ResourceDirectoryType is used,
    /// no manager will be used by the ResourceManager to find the Resource.
    Unknown,
    
    /// @brief Custom ResourceDirectory.
    Custom,
    
    /// @brief Used by Material's objects.
    Material,
    
    /// @brief Used by Mesh's objects.
    Mesh,
    
    /// @brief Windows objects.
    Window,
    
    /// @brief RenderScene objects.
    RenderScene,
    
    /// @brief Renderer objects.
    Renderer
};

/// @brief Returns the String value of each ResourceDirectoryType.
std::string ResourceDirectoryTypeToString ( const ResourceDirectoryType& rdirtype );

//////////////////////////////////////////////////////////////////////
/// @brief A virtual path localizing a Resource in the Engine.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourcePath
{
public:
    
    POOLED(Pools::Default)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourcePath ( const std::string& path );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourcePath ( const ResourceDirectoryType& rdirtype , const std::string& name );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourcePath ( const ResourcePath& rhs );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ResourcePath();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the ResourceDirectoryType found for this ResourcePath.
    //////////////////////////////////////////////////////////////////////
    virtual const ResourceDirectoryType& getDirectoryType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the String value for the ResourceDirectoryType.
    //////////////////////////////////////////////////////////////////////
    virtual std::string getStringType() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Resource Name found for this ResourcePath.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string& getName() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the ResourcePath has an empty name.
    //////////////////////////////////////////////////////////////////////
    virtual bool empty() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the String ResourcePath.
    //////////////////////////////////////////////////////////////////////
    virtual std::string toString() const;
    
private:
    
    /// @brief ResourceDirectoryType.
    ResourceDirectoryType iDirectoryType;
    
    /// @brief Resource's Name.
    std::string iName;
};

GreEndNamespace

#endif /* ResourcePath_h */
