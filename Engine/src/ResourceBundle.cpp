//////////////////////////////////////////////////////////////////////
//
//  ResourceBundle.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 21/03/2017.
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

#include "ResourceBundle.h"
#include "Platform.h"

GreBeginNamespace

ResourceBundle::ResourceBundle ( const std::string & name ) : Gre::Resource ( name )
{
    iDirectories[ResourceType::Plugin] = { } ;
    iDirectories[ResourceType::Program] = { } ;
    iDirectories[ResourceType::Effect] = { } ;
    iDirectories[ResourceType::Mesh] = { } ;
    iDirectories[ResourceType::Texture] = { } ;
}

ResourceBundle::~ResourceBundle() noexcept ( false )
{

}

void ResourceBundle::addDirectory(const Gre::ResourceType &type, const std::string &path)
{
    GreAutolock ; iDirectories[type].push_back(path) ;
}

const std::vector < std::string > & ResourceBundle::getDirectories(const Gre::ResourceType &type) const
{
    GreAutolock ; return iDirectories.at(type) ;
}

std::vector < std::string > ResourceBundle::getFilesList ( const ResourceType & type ) const
{
    GreAutolock ;

    auto dirs = iDirectories.at(type) ;

    if ( dirs.empty() )
        return std::vector < std::string > () ;

    std::vector < std::string > files ;

    for ( auto dir : dirs ) {
        
        auto tmp = Platform::GetFilesListWithDirectory (dir) ;
        files.insert ( files.end() , tmp.begin() , tmp.end() ) ;
    }

    return files ;
}

GreEndNamespace
