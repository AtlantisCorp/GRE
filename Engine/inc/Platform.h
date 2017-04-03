//////////////////////////////////////////////////////////////////////
//
//  Platform.h
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

#ifndef Platform_h
#define Platform_h

#include "Version.h"

#include <uuid/uuid.h>

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct Uuid {
    uuid_t uuid;

    Uuid() { }
    Uuid(const Uuid &other) { uuid_copy(uuid, other.uuid); }
    Uuid(const uuid_t other_uuid) { uuid_copy(uuid, other_uuid); }
    void generateInplace() { uuid_generate(uuid); }
    static Uuid generate() { Uuid wrapped; uuid_generate(wrapped.uuid); return wrapped; }
    bool operator<(const Uuid &other) { return uuid_compare(uuid, other.uuid) < 0; }
    bool operator==(const Uuid &other) { return uuid_compare(uuid, other.uuid) == 0; }
};

//////////////////////////////////////////////////////////////////////
/// @brief Represents a set of static function that aims to be
/// platform-specific.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Platform
{
public:

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a list of files with the given extension in the given
    /// directory.
    //////////////////////////////////////////////////////////////////////
    static std::vector<std::string> FindFilesWithExtension ( const std::string & dir , const std::string & ext ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the standard path separator between directories.
    //////////////////////////////////////////////////////////////////////
    static char GetSeparator () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a file's name in the given directory.
    //////////////////////////////////////////////////////////////////////
    static bool FindFileInDirectory ( const std::string & dir , const std::string & file ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the whole file data as plain text.
    //////////////////////////////////////////////////////////////////////
    static std::string GetFileSource ( const std::string & filepath ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a non-recursive file's list for the given directory.
    //////////////////////////////////////////////////////////////////////
    static std::vector < std::string > GetFilesList ( const std::string & dirpath ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls 'GetFilesList' and appends the directory to the path.
    /// Notes the directory path is cleaned as the last character is the path
    /// separator.
    //////////////////////////////////////////////////////////////////////
    static std::vector < std::string > GetFilesListWithDirectory ( const std::string & dirpath ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the directory of given file path.
    //////////////////////////////////////////////////////////////////////
    static std::string GetFileDirectory ( const std::string & path ) ;
};

GreEndNamespace

#endif /* Platform_h */
