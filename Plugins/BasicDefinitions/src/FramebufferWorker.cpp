//////////////////////////////////////////////////////////////////////
//
//  FramebufferWorker.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 01/07/2017.
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

#include "FramebufferWorker.h"
#include <ResourceManager.h>
#include <Projection.h>

FramebufferWorker::FramebufferWorker( const std::string & name )
: Gre::DefinitionWorker( name )
{
    iUUID = Uuid::fromStr( "7a344b53-3bdb-4139-aa48-b3151b3adfa6" );
}

FramebufferWorker::~FramebufferWorker() noexcept( false )
{

}

bool FramebufferWorker::handles( const std::string & definition ) const
{
    return ( definition == "Framebuffer" ) || ( definition == "GRE:Framebuffer" ) ;
}

bool FramebufferWorker::isOverwritten( const Uuid & uuid ) const
{
    return false ;
}

bool FramebufferWorker::process( const DefinitionFileNode * node ,
                      DefinitionContext * ctxt ,
                      const DefinitionWorkerHandlingMap & defs ,
                      const DefinitionParser* parser ) const
{
    if ( !node || !ctxt )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Wait for dependents workers here.

    // waitDependentDefinitions( parser , defs );

    auto tm  = ResourceManager::Get() -> getTextureManager() ;
    auto fbm = ResourceManager::Get() -> getFramebufferManager() ;
    if ( fbm.isInvalid() )
    {
        ctxt -> pushError({
            123 , "Framebuffer Manager is invalid." ,
            std::string() , 0 , false
        });

        return false ;
    }

    //////////////////////////////////////////////////////////////////////
    // Computes name.

    std::string name = node -> getDefinitionWord( 1 );
    RenderColorBuffer readbuf = RenderColorBuffer::None ;
    RenderColorBuffer writebuf = RenderColorBuffer::None ;

    if ( name.empty() )
    {
        ctxt -> pushError({
            123 , "Definition Block Framebuffer has no name." ,
            std::string() , 0 , false
        });

        return false ;
    }

    //////////////////////////////////////////////////////////////////////
    // Creates a blank framebuffer.

    RenderFramebufferHolder fb = fbm -> loadBlank( name );

    if ( fb.isInvalid() )
    {
        ctxt -> pushError({
            123 , "Framebuffer can't be created." ,
            std::string() , 0 , false
        });

        return false ;
    }

    fb -> bind() ;

    //////////////////////////////////////////////////////////////////////
    // Parse children.

    for ( auto child : node -> getChildren() )
    {
        if ( !child )
        continue ;

        if ( child -> getName() == "Texture" &&
             child -> countWords() >= 3 )
        {
            std::string attachement = child -> getDefinitionWord( 1 );
            std::string texture = child -> getDefinitionWord( 2 );

            RenderFramebufferAttachement rfa = RenderFramebufferAttachementFromString( attachement );
            TextureHolder th = tm.isInvalid() ? nullptr : tm -> findFirstHolder( texture );

            if ( !th.isInvalid() )
            fb -> setAttachment( rfa , th );
        }

        else if ( child -> getName() == "Drawbuffer" &&
                  child -> countWords() >= 2 )
        {
            std::string buffer = child -> getDefinitionWord( 1 );
            writebuf = RenderColorBufferFromString( buffer );
        }

        else if ( child -> getName() == "Readbuffer" &&
                  child -> countWords() >= 2 )
        {
            std::string buffer = child -> getDefinitionWord( 1 );
            readbuf = RenderColorBufferFromString( buffer );
        }

        else if ( child -> getName() == "Viewport" &&
                  child -> countWords() >= 5 )
        {
            //////////////////////////////////////////////////////////////////////
            // Scans viewport left, top, width, height.

            Viewport viewport = Viewport ( (Surface) {
                static_cast<int>(strtol(child -> getDefinitionWord(1).c_str(), nullptr, 10)),
                static_cast<int>(strtol(child -> getDefinitionWord(2).c_str(), nullptr, 10)),
                static_cast<int>(strtol(child -> getDefinitionWord(3).c_str(), nullptr, 10)),
                static_cast<int>(strtol(child -> getDefinitionWord(4).c_str(), nullptr, 10))
            });

            //////////////////////////////////////////////////////////////////////
            // Scans viewport's projection. The projection should have been loaded
            // previously by any projection worker and registered to the context
            // local variables table.

            if ( child -> countWords() >= 6 )
            {
                std::string projname = child -> getDefinitionWord( 5 );

                if ( !projname.empty() )
                {
                    if ( ctxt -> hasLocalVariableTpl<Projection>( projname ) )
                    {
                        Projection projection = ctxt -> getLocalVariableTpl<Projection>( projname );
                        viewport.setProjection( projection );
                    }

                    else
                    {
                        ctxt -> pushError({
                            123 , std::string( "Context does not hold projection '" ) + projname + "'." ,
                            std::string() , 0 , false
                        });
                    }
                }
            }

            fb -> setViewport( viewport );
        }
    }

    fb -> setReadBuffer( readbuf );
    fb -> setWriteBuffer( writebuf );
    fb -> unbind() ;

    return fb -> isComplete() ;
}

const std::vector < std::string > FramebufferWorker::getDependentDefinitions() const
{
    return
    {
        "Projection" , "GRE:Projection" ,
        "Texture" , "GRE:Texture"
    };
}

const std::vector < std::string > FramebufferWorker::definitions() const
{
    return
    {
        "Framebuffer" , "GRE:Framebuffer"
    };
}
