//////////////////////////////////////////////////////////////////////
//
//  TextureWorker.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/07/2017.
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

#include "TextureWorker.h"

#include <ResourceManager.h>
#include <Texture.h>

// -----------------------------------------------------------------------------
// TextureWorker implementation.

TextureWorker::TextureWorker ( const std::string & name )
: Gre::DefinitionWorker( name )
{
    iUUID = Uuid::fromStr( "4ed887c4-2e8a-4ad6-a0e5-f77751e6fd20" );
}

TextureWorker::~TextureWorker() noexcept( false )
{

}

bool TextureWorker::handles( const std::string & definition ) const
{
    return ( definition == "Texture" ) || ( definition == "GRE:Texture" ) ;
}

bool TextureWorker::isOverwritten( const Uuid & uuid ) const
{
    return false ;
}

bool TextureWorker::process( const DefinitionFileNode * node ,
                      DefinitionContext * ctxt ,
                      const DefinitionWorkerHandlingMap & defs ,
                      const DefinitionParser* parser ) const
{
    if ( !node || !ctxt )
    return false ;

    auto tm = ResourceManager::Get() -> getTextureManager() ;

    if ( tm.isInvalid() )
    {
        ctxt -> pushError({
            123 , "Texture Manager not available." ,
            std::string() , 0 , false
        });

        return false ;
    }

    //////////////////////////////////////////////////////////////////////
    // Computes name and type.

    std::string stype = node -> getDefinitionWord( 1 );
    std::string name = node -> getDefinitionWord( 2 );

    if ( name.empty() )
    {
        ctxt -> pushError({
            123 , "Texture has no name." ,
            std::string() , 0 , false
        });

        return false ;
    }

    if ( stype.empty() )
    {
        ctxt -> pushError({
            123 , name + " : invalid type." ,
            std::string() , 0 , false
        });

        return false ;
    }

    TextureType type = TextureTypeFromString( stype );
    PixelFormat pf = PixelFormat::RGBA ;
    InternalPixelFormat ipf = InternalPixelFormat::RGBA ;
    PixelType pt = PixelType::Float ;
    std::pair<int,int> size = std::make_pair(0,0);
    int depth = 0 ;

    for ( auto child : node -> getChildren() )
    {
        if ( !child )
        continue ;

        if ( child -> getName() == "File" )
        {
            //////////////////////////////////////////////////////////////////////
            // In this case , we abort the children iteration and we directly
            // creates the texture from given bundled file.
            std::string path = child -> getDefinitionWord( 1 );
            TextureHolder texture = tm -> loadBundledFile( name , path , type , ResourceLoaderOptions() );
            return !texture.isInvalid() ;
        }

        else if ( child -> getName() == "Size"
                && child -> countWords() >= 3 )
        {
            int width  = static_cast<int>(strtol(child -> getDefinitionWord(1).c_str(), nullptr, 10)) ;
            int height = static_cast<int>(strtol(child -> getDefinitionWord(2).c_str(), nullptr, 10)) ;
            size = std::make_pair( width , height );
        }

        else if ( child -> getName() == "PixelFormat"
                && child -> countWords() >= 2 )
        {
            pf = PixelFormatFromString( child->getDefinitionWord(1) );
        }

        else if ( child -> getName() == "InternalPixelFormat"
                && child -> countWords() >= 2 )
        {
            ipf = InternalPixelFormatFromString( child->getDefinitionWord(1) );
        }

        else if ( child -> getName() == "PixelType"
                && child -> countWords() >= 2 )
        {
            pt = PixelTypeFromString( child->getDefinitionWord(1) );
        }
    }

    size_t psize = PixelFormatGetCount( pf ) * PixelTypeGetSize( pt ) ;
    if ( psize == 0 )
    {
        ctxt -> pushError({
            123 , name + " : Resulting pixel size is null." ,
            std::string() , 0 , false
        });

        return false ;
    }

    TextureHolder texture = tm -> loadFromNewPixelBuffer( name ,
        size.first , size.second , depth ,
        pf , ipf , pt ,
        type , psize
    );

    return !texture.isInvalid() ;
}

const std::vector < std::string > TextureWorker::getDependentDefinitions() const
{
    return
    {

    };
}

const std::vector < std::string > TextureWorker::definitions() const
{
    return
    {
        "Texture" , "GRE:Texture"
    };
}
