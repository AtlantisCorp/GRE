//////////////////////////////////////////////////////////////////////
//
//  Platform.cpp
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

#include "Platform.h"

GreBeginNamespace

std::vector<std::string> Platform::FindFilesWithExtension(const std::string &dir, const std::string &cmp)
{
    DIR * dptr ; struct dirent * ent ;
    std::vector < std::string > ret ;

    dptr = opendir(dir.c_str()) ;

    if ( dptr == NULL ) {
        GreDebug("[WARN] Directory '") << dir << "' can't be opened." << gendl ;
        return ret ;
    }

    ent = readdir(dptr) ;

    while ( ent != NULL )
    {
        std::string path = std::string(ent -> d_name) ;

        size_t ext = path.find_last_of( std::string(".") );
        std::string extension = path.substr( ext + 1 , path.size() );

        if ( extension == cmp )
            ret.push_back(dir + Platform::GetSeparator() + path) ;

        ent = readdir(dptr) ;
    }

    closedir(dptr) ;
    return ret ;
}

char Platform::GetSeparator()
{
#ifndef GrePlatformWindows
    return '/' ;
#else
    return '\\' ;
#endif
}

bool Platform::FindFileInDirectory(const std::string &dir, const std::string &file)
{
    DIR * dptr ; struct dirent * ent ;
    dptr = opendir(dir.c_str()) ;

    if ( dptr == NULL ) {
        GreDebug("[WARN] Directory '") << dir << "' can't be opened." << gendl ;
        return false ;
    }

    //////////////////////////////////////////////////////////////////////
    // As we need to also find files in subdirectories , we may first divide
    // the filepath by the directory's separator , and then look for each file
    // in the deeper step.

    std::vector < std::string > subdirs = split(file, Platform::GetSeparator()) ;

    if ( subdirs.empty() )
    {
        closedir(dptr) ;
        return false ;
    }

    ent = readdir(dptr) ;

    while ( ent != NULL )
    {
        std::string path = std::string(ent -> d_name) ;

        if ( path == subdirs.at(0) )
        {
            if ( subdirs.size() > 1 )
            {
                //////////////////////////////////////////////////////////////////////
                // Constructs a temporary path to find the next subdir. Notes the substitued
                // string is at position of first sub directory size + 1 , because we
                // must count the separator.

                std::string subdir = dir + GetSeparator() + subdirs.at(0) ;
                std::string subfile = file.substr(subdirs.at(0).size()+1) ;

                closedir(dptr) ;
                return FindFileInDirectory(subdir, subfile) ;
            }

            else
            {
                //////////////////////////////////////////////////////////////////////
                // As they is nothing behind us in the path , we can return true.

                closedir(dptr) ;
                return true ;
            }
        }

        ent = readdir(dptr) ;
    }

    closedir(dptr) ;
    return false ;
}

std::string Platform::GetFileSource(const std::string &filepath)
{
    //////////////////////////////////////////////////////////////////////
    // Loading the source from file.
    // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring

    std::ifstream srcstream ( filepath );

    if ( !srcstream ) {
        return std::string () ;
    }

    std::string src;

    srcstream.seekg(0, std::ios::end);
    src.reserve(srcstream.tellg());
    srcstream.seekg(0, std::ios::beg);

    src.assign( (std::istreambuf_iterator<char>(srcstream)), std::istreambuf_iterator<char>() );
    return src ;
}

std::vector < std::string > Platform::GetFilesList ( const std::string & dirpath )
{
    //////////////////////////////////////////////////////////////////////
    // Creates the dir structure to iterate over it.

    DIR * dptr ; struct dirent * ent ;
    dptr = opendir ( dirpath.c_str() ) ;

    if ( dptr == NULL ) {
        GreDebug("[WARN] Directory '") << dirpath << "' can't be opened." << gendl ;
        return std::vector < std::string > () ;
    }

    //////////////////////////////////////////////////////////////////////
    // Iterates over the directory entries to get files name.

    std::vector < std::string > files ;

    ent = readdir ( dptr ) ;

    while ( ent != NULL )
    {
        std::string tmp (ent->d_name) ;

        if ( tmp != "." && tmp != ".." && tmp != ".DS_Store" )
            files.push_back ( tmp ) ;

        ent = readdir ( dptr ) ;
    }

    closedir ( dptr ) ;
    return files ;
}

std::vector < std::string > Platform::GetFilesListWithDirectory(const std::string &dirpath)
{
    std::vector < std::string > files = Platform::GetFilesList(dirpath) ;
    
    if ( files.empty() )
    return files ;
    
    //////////////////////////////////////////////////////////////////////
    // Checks if the directory path ends with a separator or not. If not ,
    // we just add it.
    
    std::string cleanedpath = dirpath ;
    
    if ( cleanedpath.empty() )
    return files ;
    
    if ( cleanedpath.at(cleanedpath.size()-1) != Platform::GetSeparator() )
    cleanedpath.push_back( Platform::GetSeparator() ) ;
    
    //////////////////////////////////////////////////////////////////////
    // Appends the directory path to every pathes.
    
    for ( auto & path : files )
    path = cleanedpath + path ;
    
    return files ;
}

std::string Platform::GetFileDirectory ( const std::string & path )
{
    return path.substr ( 0 , path.find_last_of(GetSeparator()) ) ;
}

GreEndNamespace
