//////////////////////////////////////////////////////////////////////
//
//  ResourceBundle.h
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

#ifndef GRE_ResourceBundle_h
#define GRE_ResourceBundle_h

#include "Resource.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines Resource's type. Each loader can load a path using
/// one of the resource's type , depending on what is loaded.
enum class ResourceType
{
    Plugin ,
    Program ,
    Texture ,
    Mesh ,

    /// @brief A Definition File is any file within the DefinitionFile directories
    /// in the given bundle. Generally it should have the extension '.gdef' or '.gre'.
    DefinitionFile
};

//////////////////////////////////////////////////////////////////////
/// @brief Defines a set of path usable to load a resource , grouping
/// pathes by resource type.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceBundle : public Resource
{
public:

    POOLED ( Pools::Bundle )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourceBundle ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ResourceBundle () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Directory with given path to load the given resource's
    /// type. If this directory does not exists , it should be ignored.
    //////////////////////////////////////////////////////////////////////
    void addDirectory ( const ResourceType & type , const std::string & path ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every directories for given resource's type.
    //////////////////////////////////////////////////////////////////////
    const std::vector<std::string> & getDirectories ( const ResourceType & type ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of files in the directories for given
    /// ResourceType. Notes this list is non-recursive , and that the directory
    /// path will be added at the beginning of the file path , in order to be
    /// able to use the file path directly after this function.
    //////////////////////////////////////////////////////////////////////
    std::vector < std::string > getFilesList ( const ResourceType & type ) const ;

protected:

    /// @brief Holds the directories grouped by resource's type. Each Resource that
    /// can be loaded should be of one of Resource's type.
    std::map < ResourceType , std::vector <std::string> > iDirectories ;
};

GRE_MAKE_HOLDER( ResourceBundle ) ;

GreEndNamespace

#endif // GRE_ResourceBundle_h
