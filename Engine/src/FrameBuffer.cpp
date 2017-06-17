//////////////////////////////////////////////////////////////////////
//
//  FrameBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 25/01/2016.
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

#include "FrameBuffer.h"

GreBeginNamespace

// ---------------------------------------------------------------------------------------------------

RenderColorBuffer RenderColorBufferFromString ( const std::string & str )
{
    if ( str == "FrontLeft" ) return RenderColorBuffer::FrontLeft ;
    if ( str == "FrontRight" ) return RenderColorBuffer::FrontRight ;
    if ( str == "BackLeft" ) return RenderColorBuffer::BackLeft ;
    if ( str == "BackRight" ) return RenderColorBuffer::BackRight ;
    if ( str == "Front" ) return RenderColorBuffer::Front ;
    if ( str == "Back" ) return RenderColorBuffer::Back ;
    if ( str == "Left" ) return RenderColorBuffer::Left ;
    if ( str == "Right" ) return RenderColorBuffer::Right ;
    if ( str == "FrontAndBack" ) return RenderColorBuffer::FrontAndBack ;
    if ( str == "Color0" ) return RenderColorBuffer::Color0 ;
    if ( str == "Color1" ) return RenderColorBuffer::Color1 ;
    if ( str == "Color2" ) return RenderColorBuffer::Color2 ;
    if ( str == "Color3" ) return RenderColorBuffer::Color3 ;
    if ( str == "Color4" ) return RenderColorBuffer::Color4 ;
    if ( str == "Color5" ) return RenderColorBuffer::Color5 ;
    if ( str == "Color6" ) return RenderColorBuffer::Color6 ;
    if ( str == "Color7" ) return RenderColorBuffer::Color7 ;
    if ( str == "Color8" ) return RenderColorBuffer::Color8 ;
    return RenderColorBuffer::None ;
}

RenderFramebufferAttachement RenderFramebufferAttachementFromString ( const std::string & str )
{
    if ( str == "Color0" ) return RenderFramebufferAttachement::Color0 ;
    if ( str == "Color1" ) return RenderFramebufferAttachement::Color1 ;
    if ( str == "Color2" ) return RenderFramebufferAttachement::Color2 ;
    if ( str == "Color3" ) return RenderFramebufferAttachement::Color3 ;
    if ( str == "Color4" ) return RenderFramebufferAttachement::Color4 ;
    if ( str == "Color5" ) return RenderFramebufferAttachement::Color5 ;
    if ( str == "Color6" ) return RenderFramebufferAttachement::Color6 ;
    if ( str == "Color7" ) return RenderFramebufferAttachement::Color7 ;
    if ( str == "Color8" ) return RenderFramebufferAttachement::Color8 ;

    if ( str == "Depth" ) return RenderFramebufferAttachement::Depth ;
    if ( str == "Stencil" ) return RenderFramebufferAttachement::Stencil ;
    if ( str == "DepthStencil" ) return RenderFramebufferAttachement::DepthStencil ;

    return RenderFramebufferAttachement::Null ;
}

std::string RenderFramebufferAttachementToString ( const RenderFramebufferAttachement & layer )
{
    if ( layer == RenderFramebufferAttachement::Color0 ) return "Color0" ;
    if ( layer == RenderFramebufferAttachement::Color1 ) return "Color1" ;
    if ( layer == RenderFramebufferAttachement::Color2 ) return "Color2" ;
    if ( layer == RenderFramebufferAttachement::Color3 ) return "Color3" ;
    if ( layer == RenderFramebufferAttachement::Color4 ) return "Color4" ;
    if ( layer == RenderFramebufferAttachement::Color5 ) return "Color5" ;
    if ( layer == RenderFramebufferAttachement::Color6 ) return "Color6" ;
    if ( layer == RenderFramebufferAttachement::Color7 ) return "Color7" ;
    if ( layer == RenderFramebufferAttachement::Color8 ) return "Color8" ;

    if ( layer == RenderFramebufferAttachement::Depth ) return "Depth" ;
    if ( layer == RenderFramebufferAttachement::Stencil ) return "Stencil" ;
    if ( layer == RenderFramebufferAttachement::DepthStencil ) return "DepthStencil" ;

    return "Null" ;
}

// ---------------------------------------------------------------------------------------------------

FramebufferAttachment::FramebufferAttachment()
{
    layer = RenderFramebufferAttachement::Null;
    type = RenderFramebufferAttachementType::Texture;
    texture = TextureHolder(nullptr);
    renderbuffer.size = { 0 , 0 } ;
    renderbuffer.format = InternalPixelFormat::None ;
}

void FramebufferAttachment::reset()
{
    layer = RenderFramebufferAttachement::Null;
    type = RenderFramebufferAttachementType::Texture;
    texture = TextureHolder(nullptr);
    renderbuffer.size = { 0 , 0 } ;
    renderbuffer.format = InternalPixelFormat::None ;
}

// ---------------------------------------------------------------------------------------------------

RenderFramebuffer::RenderFramebuffer(const std::string& name) : Resource ( name )
{
    iWriteBuffer = RenderColorBuffer::None ;
    iReadBuffer = RenderColorBuffer::None ;
    iDefaultSize.first = 0 ;
    iDefaultSize.second = 0 ;
    iAttachments [RenderFramebufferAttachement::Null] = FramebufferAttachment () ;
}

RenderFramebuffer::~RenderFramebuffer()
{

}

FramebufferAttachment & RenderFramebuffer::getAttachment ( const RenderFramebufferAttachement & value )
{
    GreAutolock ;

    auto it = iAttachments.find ( value ) ;

    if ( it != iAttachments.end() )
    return it->second ;

    return iAttachments.at ( RenderFramebufferAttachement::Null ) ;
}

const FramebufferAttachment & RenderFramebuffer::getAttachment ( const RenderFramebufferAttachement & value ) const
{
    GreAutolock ;

    auto it = iAttachments.find ( value ) ;

    if ( it != iAttachments.end() )
    return it->second ;

    return iAttachments.at ( RenderFramebufferAttachement::Null ) ;
}

void RenderFramebuffer::setAttachment ( const RenderFramebufferAttachement & value , const TextureHolder & texture ) const
{
    GreAutolock ;

    FramebufferAttachment attachment ;
    attachment.layer = value ;
    attachment.type = RenderFramebufferAttachementType::Texture ;
    attachment.texture = texture ;

    //////////////////////////////////////////////////////////////////////
    // Tries to attach the texture to the framebuffer using the implementation
    // function. On success , sets the attachment property.

    bool success = bindAttachment ( attachment ) ;

    if ( !success )
    GreDebug ( "[WARN] Attachment '" ) << RenderFramebufferAttachementToString(value) << "' failed for Framebuffer '" << getName() << "'." << gendl ;

    else
    iAttachments [value] = attachment ;
}

void RenderFramebuffer::setAttachmentBuffer (const RenderFramebufferAttachement & value ,
                                             const std::pair < int , int > & size ,
                                             const InternalPixelFormat & format ) const
{
    GreAutolock ;

    FramebufferAttachment attachment ;
    attachment.layer = value ;
    attachment.type = RenderFramebufferAttachementType::Renderbuffer ;
    attachment.renderbuffer.size = size ;
    attachment.renderbuffer.format = format ;

    //////////////////////////////////////////////////////////////////////
    // Tries to attach the renderbuffer to the framebuffer using the implementation
    // function. On success , sets the attachment property.

    bool success = bindAttachment ( attachment ) ;

    if ( !success )
    GreDebug ( "[WARN] Attachment '" ) << RenderFramebufferAttachementToString(value) << "' failed for Framebuffer '" << getName() << "'." << gendl ;

    else
    iAttachments [value] = attachment ;
}

void RenderFramebuffer::setWriteBuffer(const Gre::RenderColorBuffer &buffer)
{
    GreAutolock ; iWriteBuffer = buffer ;
}

void RenderFramebuffer::setReadBuffer(const Gre::RenderColorBuffer &buffer)
{
    GreAutolock ; iReadBuffer = buffer ;
}

void RenderFramebuffer::setViewport ( const Viewport & viewport )
{
    GreAutolock ; iViewport = viewport ;
}

const Viewport & RenderFramebuffer::getViewport() const
{
    GreAutolock ; return iViewport ;
}

Viewport & RenderFramebuffer::getViewport ()
{
    GreAutolock ; return iViewport ;
}

const std::pair < int , int > & RenderFramebuffer::getDefaultSize () const
{
    GreAutolock ; return iDefaultSize ;
}

void RenderFramebuffer::clear ( const RenderFramebufferAttachement & attachment )
{
    GreAutolock ;

    auto it = iAttachments.find ( attachment ) ;

    if ( it == iAttachments.end() )
    return ;

    unbindAttachment ( it->second ) ;
    iAttachments.erase ( it ) ;
}

void RenderFramebuffer::clear ()
{
    GreAutolock ;

    while ( !iAttachments.empty() )
    {
        auto it = iAttachments.begin() ;
        unbindAttachment ( it->second ) ;
        iAttachments.erase ( it ) ;
    }

    iDefaultSize = { 0 , 0 } ;
    iWriteBuffer = RenderColorBuffer::None ;
    iReadBuffer = RenderColorBuffer::None ;
    iViewport = { 0 , 0 , 0 , 0 } ;
}

const Projection & RenderFramebuffer::getProjection () const
{
    GreAutolock ; return iViewport.getProjection () ;
}

// ---------------------------------------------------------------------------------------------------

RenderFramebufferInternalCreator::RenderFramebufferInternalCreator ()
{

}

RenderFramebufferInternalCreator::~RenderFramebufferInternalCreator()
{

}

// ---------------------------------------------------------------------------------------------------

RenderFramebufferManager::RenderFramebufferManager ( const std::string & name )
: ResourceManagerBase<Gre::RenderFramebuffer>(name)
, iCreator ( nullptr )
{

}

RenderFramebufferManager::~RenderFramebufferManager() noexcept ( false )
{
    if ( iCreator ) {
        delete iCreator ;
    }
}

RenderFramebufferHolder RenderFramebufferManager::getNull ()
{
    GreAutolock ; return findFirstHolder ( "framebuffers.default" ) ;
}

RenderFramebufferHolder RenderFramebufferManager::loadBlank(const std::string &name)
{
    return load ( name , ResourceLoaderOptions() ) ;
}

RenderFramebufferHolder RenderFramebufferManager::load(const std::string &name, const ResourceLoaderOptions &options)
{
    GreAutolock ;

    if ( iCreator )
    {
        RenderFramebufferHolder framebuffer ( iCreator->load(name, options) ) ;
        if ( framebuffer.isInvalid() ) {
            GreDebug("[WARN] RenderFramebufferInternalCreator failed creating RenderFramebuffer '") << name << "'." << gendl ;
            return RenderFramebufferHolder ( nullptr ) ;
        }

        GreDebug("[INFO] Loaded new RenderFramebuffer '") << name << "'." << gendl ;

        iHolders.add(framebuffer) ;
        return framebuffer ;
    }

    else
    {
        GreDebug("[WARN] No RenderFramebufferInternalCreator registered.") << Gre::gendl ;
        return RenderFramebufferHolder ( nullptr ) ;
    }
}

void RenderFramebufferManager::setInternalCreator(Gre::RenderFramebufferInternalCreator *creator)
{
    GreAutolock ;

    if ( iCreator )
    return ;

    iCreator = creator ;

    RenderFramebufferHolder null = iCreator -> loadNull() ;
    null -> setName( "framebuffers.default" ) ;

    iHolders.add(null) ;
}

const RenderFramebufferInternalCreator* RenderFramebufferManager::getInternalCreator() const
{
    GreAutolock ; return iCreator ;
}

RenderFramebufferHolder RenderFramebufferManager::get(const std::string &name)
{
    GreAutolock ; return findFirstHolder ( name ) ;
}

const RenderFramebufferHolder RenderFramebufferManager::get(const std::string &name) const
{
    GreAutolock ; return findFirstHolder ( name ) ;
}

GreEndNamespace
