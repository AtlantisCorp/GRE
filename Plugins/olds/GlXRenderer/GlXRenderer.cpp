//////////////////////////////////////////////////////////////////////
//
//  GlXRenderer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 23/10/2016.
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

#include "GlXRenderer.h"
#include "GlXContext.h"

extern "C" bool isExtensionSupported(const char *extList, const char *extension)
{
    const char *start;
    const char *where, *terminator;
    
    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;
    
    /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
    for (start=extList;;) {
        where = strstr(start, extension);
        
        if (!where)
            break;
        
        terminator = where + strlen(extension);
        
        if ( where == start || *(where - 1) == ' ' )
            if ( *terminator == ' ' || *terminator == '\0' )
                return true;
        
        start = terminator;
    }
    
    return false;
}

GlXRenderer::GlXRenderer(const std::string& name)
: Gre::RendererPrivate(name)
, iGlobalContext ( nullptr )
{
    GreResourceAutolock ;
    
    // We must create the Global Context in order to share other Contexts.
    iGlobalContext = Gre::RenderContextHolder ( new GlXContext ( "GlobalContext" , 3 , 0 , 0 ) ) ;
    
    if ( iGlobalContext.isInvalid() )
    {
        GreDebugPretty() << "Can't create Global OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Can't create Global OpenGl Context." ) ;
    }
    
    GreDebugPretty() << "Created Global OpenGl Context." << Gre::gendl;
}

GlXRenderer::~GlXRenderer() noexcept ( false )
{
    GreResourceAutolock ;
    
    if ( iGlobalContext.isInvalid() )
    {
        GreDebugPretty() << "Can't access to Global OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Can't access to Global OpenGl Context." ) ;
    }
}

void GlXRenderer::drawVertexBufferPrivate(const Gre::HardwareVertexBuffer& vbuf, const Gre::HardwareProgramHolder& program)
{
    //  TODO
}

void GlXRenderer::drawIndexBufferPrivate(const Gre::HardwareIndexBuffer& ibuf, const Gre::HardwareVertexBuffer& vbuf, const Gre::HardwareProgramHolder& program)
{
    // TODO
}

void GlXRenderer::setClearColor ( const Gre::Color& color )
{
    GreResourceAutolock ;
    glClearColor ( color.getRed() , color.getGreen() , color.getBlue() , color.getAlpha() ) ;
}

void GlXRenderer::setClearDepth ( float depth )
{
    GreResourceAutolock ;
    glClearDepth(depth);
}

Gre::HardwareVertexBufferHolder GlXRenderer::iCreateVertexBuffer ( const std::string& name , size_t sz ) const
{
    GreResourceAutolock ;
    return Gre::HardwareVertexBufferHolder ( nullptr ) ;
}

Gre::TextureHolder GlXRenderer::iCreateTexturePrivate ( const std::string& name ) const
{
    GreResourceAutolock ;
    return Gre::TextureHolder ( nullptr ) ;
}

Gre::RenderContextHolder GlXRenderer::iCreateRenderContext ( const std::string& name , const Gre::RenderContextInfo& info ) const
{
    GreResourceAutolock ;
    
    if ( iGlobalContext.isInvalid() )
    {
        GreDebugPretty() << "Invalid Global OpenGl Context." << Gre::gendl;
        throw Gre::GreExceptionWithText ( "Invalid Global OpenGl Context." ) ;
    }
        
    GLint glmaj = info.hasEntry("Version") ? info.getValue("Version").toVersion().major : 3 ;
    GLint glmin = info.hasEntry("Version") ? info.getValue("Version").toVersion().minor : 0 ;
    GLXContext glshared = * ( (const GLXContext*) iGlobalContext->getProperty("GLXContext") ) ;
    
    return Gre::RenderContextHolder ( new GlXContext ( name , glmaj , glmin , glshared ) ) ;
}

// ---------------------------------------------------------------------------------------------------

GlXRendererLoader::GlXRendererLoader()
{
    
}

GlXRendererLoader::~GlXRendererLoader()
{
    
}

bool GlXRendererLoader::isCompatible(const Gre::RenderingApiDescriptor &apidescriptor) const
{
    if ( apidescriptor.getApiName() == "OpenGl" )
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

Gre::RendererHolder GlXRendererLoader::load(const std::string &name) const
{
    return Gre::RendererHolder ( new GlXRenderer(name) );
}

Gre::ResourceLoader* GlXRendererLoader::clone() const
{
    return new GlXRendererLoader();
}

bool GlXRendererLoader::isLoadable(const std::string &filepath) const
{
    return false;
}

