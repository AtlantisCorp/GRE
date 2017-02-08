//////////////////////////////////////////////////////////////////////
//
//  FrameBuffer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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
  
RenderFramebuffer::Attachement::Attachement()
  : texture(nullptr)
{
  
}

void RenderFramebuffer::Attachement::reset()
{
    attachement = RenderFramebufferAttachement::Null;
    attachType = RenderFramebufferAttachementType::Texture;
    surface = Surface ({0, 0, 0, 0});
    texture = TextureHolder(nullptr);
}

bool RenderFramebuffer::Attachement::isValid() const
{
    if( attachType == RenderFramebufferAttachementType::Texture )
    {
        return ! texture.isInvalid();
    }
  
    return false;
}
  
// ---------------------------------------------------------------------------------------------------

RenderFramebuffer::RenderFramebuffer(const std::string& name)
: Resource(ResourceIdentifier::New() , name)
{
    
}

RenderFramebuffer::~RenderFramebuffer()
{
    
}

void RenderFramebuffer::bind() const
{
    
}

void RenderFramebuffer::unbind() const
{
    
}

TextureUser RenderFramebuffer::getTextureAttachement(const RenderFramebufferAttachement& attachement)
{
    auto it = iAttachements.find(attachement);
    if(it != iAttachements.end())
    {
#ifdef GreIsDebugMode
        if(it->second.attachType != RenderFramebufferAttachementType::Texture)
        {
            GreDebugPretty() << "Looking for texture attachement '" << (int) attachement << "' but is not Texture." << Gre::gendl;
        }
#endif
        return TextureUser(it->second.texture);
    }
  
#ifdef GreIsDebugMode
    GreDebugPretty() << "Can't found attachement '" << (int) attachement << "'." << Gre::gendl;
#endif
    return nullptr;
}

const TextureUser RenderFramebuffer::getTextureAttachement(const RenderFramebufferAttachement& attachement) const
{
    auto it = iAttachements.find(attachement);
    if(it != iAttachements.end())
    {
#ifdef GreIsDebugMode
        if(it->second.attachType != RenderFramebufferAttachementType::Texture)
        {
            GreDebugPretty() << "Looking for texture attachement '" << (int) attachement << "' but is not Texture." << Gre::gendl;
        }
#endif
        return TextureUser(it->second.texture);
    }
  
#ifdef GreIsDebugMode
    GreDebugPretty() << "Can't found attachement '" << (int) attachement << "'." << Gre::gendl;
#endif
    return nullptr;
}

Surface RenderFramebuffer::getAttachementSurface(const RenderFramebufferAttachement& attachement) const
{
    auto it = iAttachements.find(attachement);
    if(it != iAttachements.end())
	 {
        return it->second.surface;
    }
  
#ifdef GreIsDebugMode
    GreDebugPretty() << "Can't found attachement '" << (int) attachement << "'." << Gre::gendl;
#endif
  return { 0, 0, 0, 0 };
}

void RenderFramebuffer::setAttachement(const RenderFramebufferAttachement& attachement, TextureHolder& holder)
{
    auto attach = iAttachements[attachement];
  
    if(attach.isValid())
    {
        // We should reset the Attachement before to set it.
        attach.reset();
      
#ifdef GreIsDebugMode
        GreDebugPretty() << "Reseting Attachement '" << (int) attachement << "'." << Gre::gendl;
#endif
    }
  
    attach.attachement = attachement;
    attach.attachType = RenderFramebufferAttachementType::Texture;
    attach.texture = holder;
    attach.surface = holder->getSurface();
  
    iAttachements[attachement] = attach;
}

int RenderFramebuffer::getMaximumWidth() const
{
    return 0; 
}

int RenderFramebuffer::getMaximumHeight() const
{
    return 0;
}

int RenderFramebuffer::getMaximumColorAttachementCount() const
{
    return 0;
}

// ---------------------------------------------------------------------------------------------------

RenderFramebufferLoader::RenderFramebufferLoader()
{
    
}

RenderFramebufferLoader::~RenderFramebufferLoader()
{
    
}

GreEndNamespace
