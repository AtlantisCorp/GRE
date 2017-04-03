//////////////////////////////////////////////////////////////////////
//
//  TechniqueFilePreprocessor.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 01/04/2017.
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

#ifndef GRE_TECHNIQUEFILEPREPROCESSOR_H
#define GRE_TECHNIQUEFILEPREPROCESSOR_H

#include "Platform.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Basic object that only purpose is to preprocess the given
/// technique's source file.
///
/// A preprocessor valid lines always begins by a '%' . As the line is
/// splitted by space , a space is recommended between the '%' and the
/// preprocessor function.
///
/// % version : Indicate the version used for the technique's parser.
///
/// % include : Indicate the preprocessor to add the given file's source
/// to the current source. Notes the include path should be relative to
/// the file's path.
///
/// % message : Indicate the preprocessor to emit a debugging message.
/// This message will be seen only if 'GreIsDebugMode' is active.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TechniqueFilePreprocessor
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TechniqueFilePreprocessor () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TechniqueFilePreprocessor () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Processes the given source file. Its location is given by
    /// 'path'. Notes all includes preprocessives will be from this path.
    /// Also returns the file's version.
    //////////////////////////////////////////////////////////////////////
    virtual int process ( const std::string & path , const std::string & source ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iSourceResult'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string & getResult () const ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Processes one line of the source's file , as given by 'process'.
    //////////////////////////////////////////////////////////////////////
    virtual void processLine ( const std::string & line ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Process a 'version' word.
    //////////////////////////////////////////////////////////////////////
    virtual void processWordVersion ( const std::vector < std::string > & words ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Process a 'include' word.
    //////////////////////////////////////////////////////////////////////
    virtual void processWordInclude ( const std::vector < std::string > & words ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Process a 'message' word.
    //////////////////////////////////////////////////////////////////////
    virtual void processWordMessage ( const std::vector < std::string > & words ) ;

protected:

    /// @brief Version defined with 'process' .
    int iSourceVersion ;

    /// @brief Source's path.
    std::string iSourcePath ;

    /// @brief Source's original.
    std::string iSource ;

    /// @brief Resulting source , after preprocessing the sources.
    std::string iSourceResult ;
};

GreEndNamespace

#endif // GRE_TECHNIQUEFILEPREPROCESSOR_H
