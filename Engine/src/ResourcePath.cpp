//////////////////////////////////////////////////////////////////////
//
//  ResourcePath.cpp
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

#include "ResourcePath.h"

GreBeginNamespace

std::string ResourceDirectoryTypeToString(const ResourceDirectoryType& rdirtype)
{
    if ( rdirtype == ResourceDirectoryType::Custom )
    {
        return "Custom";
    }
    
    if ( rdirtype == ResourceDirectoryType::Material )
    {
        return "Material";
    }
    
    if ( rdirtype == ResourceDirectoryType::Mesh )
    {
        return "Mesh";
    }
    
    if ( rdirtype == ResourceDirectoryType::Window )
    {
        return "Window";
    }
    
    if ( rdirtype == ResourceDirectoryType::RenderScene )
    {
        return "RenderScene";
    }
    
    if ( rdirtype == ResourceDirectoryType::Renderer )
    {
        return "Renderer";
    }
    
    return "Unknown";
}

// ---------------------------------------------------------------------------------------------------

ResourcePath::ResourcePath(const std::string& path)
: iDirectoryType(ResourceDirectoryType::Unknown)
{
    if ( !path.empty() )
    {
        std::vector<std::string> splitted = split(path, '/');
        
        if ( splitted.size() > 2 )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "'path' given ('" << path << "') is invalid because it contains more than 2 elements." << std::endl;
#endif
        }
        
        else if ( splitted.size() == 1 )
        {
            iName = splitted.front();
        }
        
        else
        {
            std::string dirtype = splitted.front();
            iName = splitted.back();
            
            if ( dirtype == "Material" || dirtype == "Materials" )
            {
                iDirectoryType = ResourceDirectoryType::Material;
            }
            
            else if ( dirtype == "Mesh" || dirtype == "Meshes" )
            {
                iDirectoryType = ResourceDirectoryType::Mesh;
            }
            
            else if ( dirtype == "Custom" || dirtype == "Customs" )
            {
                iDirectoryType = ResourceDirectoryType::Custom;
            }
            
            else if ( dirtype == "Window" || dirtype == "Windows" )
            {
                iDirectoryType = ResourceDirectoryType::Window;
            }
            
            else if ( dirtype == "RenderScene" || dirtype == "RenderScenes" )
            {
                iDirectoryType = ResourceDirectoryType::RenderScene;
            }
            
            else if ( dirtype == "Renderer" || dirtype == "Renderers" )
            {
                iDirectoryType = ResourceDirectoryType::Renderer;
            }
            
            else
            {
                iDirectoryType = ResourceDirectoryType::Unknown;
            }
        }
    }
}

ResourcePath::ResourcePath(const ResourceDirectoryType& rdirtype, const std::string& name)
: iDirectoryType(rdirtype)
, iName(name)
{
    
}

ResourcePath::ResourcePath(const ResourcePath& rhs)
: iDirectoryType(rhs.getDirectoryType())
, iName(rhs.getName())
{
    
}

ResourcePath::~ResourcePath()
{
    
}

const ResourceDirectoryType& ResourcePath::getDirectoryType() const
{
    return iDirectoryType;
}

std::string ResourcePath::getStringType() const
{
    return ResourceDirectoryTypeToString(iDirectoryType);
}

const std::string& ResourcePath::getName() const
{
    return iName;
}

bool ResourcePath::empty() const
{
    return iName.empty();
}

std::string ResourcePath::toString() const
{
    return ResourceDirectoryTypeToString(iDirectoryType) + "/" + iName;
}

GreEndNamespace
