//////////////////////////////////////////////////////////////////////
//
//  TechniqueFilePreprocessor.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 02/04/2017.
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

#include "TechniqueFilePreprocessor.h"
#include "TechniqueFileParser.h"

GreBeginNamespace

TechniqueFilePreprocessor::TechniqueFilePreprocessor ()
{
    iSourceVersion = GRE_TECHNIQUE_PARSER_VERSION ;
}

TechniqueFilePreprocessor::~TechniqueFilePreprocessor ()
{

}

int TechniqueFilePreprocessor::process ( const std::string & path , const std::string & source )
{
    if ( source.empty() ) {
#ifdef GreIsDebugMode
        GreDebug ( "[WARN] No source input." ) << gendl ;
#endif
        return 0 ;
    }

    iSourcePath = path ;
    iSource = source ;

    //////////////////////////////////////////////////////////////////////
    // Iterates over the source line by line.

    std::stringstream lines ( source ) ;

    while ( !lines.eof() )
    {
        std::string line ; std::getline ( lines , line ) ;
        processLine ( line ) ;
    }

    //////////////////////////////////////////////////////////////////////
    // Returns the 'iSourceVersion' value.

    return iSourceVersion ;
}

const std::string & TechniqueFilePreprocessor::getResult () const
{
    return iSourceResult ;
}

void TechniqueFilePreprocessor::processLine ( const std::string & line )
{
    if ( !line.empty() )
    {
        //////////////////////////////////////////////////////////////////////
        // Splits the line in preprocessor's words.

        std::vector < std::string > words = split ( line , ' ' ) ;
        if ( words.size() < 2 ) {
            iSourceResult += line ;
            return ;
        }

        if ( words.at(0) == "%" )
        {
            //////////////////////////////////////////////////////////////////////
            // Check the second word to see what function we have to do.

            if ( words.at(1) == "version" )
                processWordVersion ( words ) ;

            else if ( words.at(1) == "include" )
                processWordInclude ( words ) ;

            else if ( words.at(1) == "message" )
                processWordMessage ( words ) ;

            else
                iSourceResult += line ;
        }
        
        else
        {
            iSourceResult += line ;
        }
    }
}

void TechniqueFilePreprocessor::processWordVersion ( const std::vector < std::string > & words )
{
    if ( words.size() >= 3 )
    {
        std::string versionstr = words.at(2) ;
        iSourceVersion = strtol ( versionstr.c_str() , NULL , 10 ) ;
    }
}

void TechniqueFilePreprocessor::processWordInclude ( const std::vector < std::string > & words )
{
    if ( words.size() >= 3 )
    {
        //////////////////////////////////////////////////////////////////////
        // Computes the file's path using this file path and the include directive.

        std::string includepath = words.at(2) ;
        includepath = iSourcePath + Platform::GetSeparator() + includepath ;

        //////////////////////////////////////////////////////////////////////
        // Gets the file's source.

        std::string src = Platform::GetFileSource ( includepath ) ;
        if ( src.empty() )
        {
#ifdef GreIsDebugMode
            GreDebug ( "[WARN] Includes file '" ) << includepath << "' not found." << gendl ;
#endif
            return ;
        }

        //////////////////////////////////////////////////////////////////////
        // Tries to launch another preprocessor. Ignores the version attribute ,
        // and launch it using the Platform::GetFileDirectory() function to get
        // the relative path.

        TechniqueFilePreprocessor preprocessor ;
        preprocessor.process ( Platform::GetFileDirectory(includepath) , src ) ;

        //////////////////////////////////////////////////////////////////////
        // On sucess , adds the resulting source to this file result.

        iSourceResult = iSource + preprocessor.getResult () ;
    }
}

void TechniqueFilePreprocessor::processWordMessage ( const std::vector < std::string > & words )
{
#ifdef GreIsDebugMode

    //////////////////////////////////////////////////////////////////////
    // Recreates the message inserting space between words.

    std::string msg ;

    for ( int i = 2 ; i < words.size() ; ++i ) {
        msg = msg + words.at(i) + " " ;
    }

    GreDebug ( "[INFO] 'message' : " ) << msg << gendl ;

#endif
}

GreEndNamespace
