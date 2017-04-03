//////////////////////////////////////////////////////////////////////
//
//  Renderer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/11/2015.
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

#include "Renderer.h"
#include "ResourceManager.h"
#include "RenderContext.h"

GreBeginNamespace

typedef std::chrono::high_resolution_clock Clock;

Renderer::Renderer (const std::string& name, const RendererOptions& options)
: Gre::Resource( name ) , iInstalled(false)
{
    iEnabled = true ;
}

Renderer::~Renderer() noexcept ( false )
{

}

void Renderer::render() const
{
    GreAutolock ;

    if ( iInstalled )
    {
        //////////////////////////////////////////////////////////////////////
        // The renderer is attached to one render context. It is responsible
        // for rendering it , and also for swapping buffers (flush) operations.
        // Passes that are drawed using the same render context should not swap
        // buffers between them. But , drawing to different framebuffers is done
        // in the technique object. 
        
        if ( iContext.isInvalid() )
        return ;
        
        iContext -> bind() ;
        
        for ( const RenderPassHolder & pass : iPasses )
        if ( !pass.isInvalid() ) pass -> render ( this ) ;
        
        iContext -> unbind() ;
        iContext -> flush() ;
    }
}

RenderPassHolder Renderer::addPass(const std::string &name)
{
    GreAutolock ;

    if ( iInstalled )
    {
        RenderPassHolder pass ( new RenderPass(name) ) ;

        if ( pass.isInvalid() )
        {
            GreDebug("[WARN] RenderPass '") << name << "' couldn't be allocated.'" << gendl ;
            return RenderPassHolder ( nullptr ) ;
        }

        GreDebug("[INFO] RenderPass '") << name << "' allocated.'" << gendl ;

        iPasses.push_back(pass) ;
        return pass ;
    }

    return RenderPassHolder ( nullptr ) ;
}

RenderPassHolder Renderer::copyPass(const RenderPassHolder &pass)
{
    GreAutolock ;

    if ( iInstalled && !pass.isInvalid() )
    {
        RenderPassHolder cpy ( new RenderPass(pass->getName()) ) ;

        if ( cpy.isInvalid() )
        {
            GreDebug("[WARN] RenderPass '") << pass->getName() << "' couldn't be copied.'" << gendl ;
            return RenderPassHolder ( nullptr ) ;
        }

        cpy -> setRenderTarget( pass->getRenderTarget() ) ;
        cpy -> setViewport ( pass->getViewport() ) ;
        cpy -> setClearColor ( pass->getClearColor() ) ;
        cpy -> setClearDepth ( pass->getClearDepth() ) ;
        cpy -> setClearBuffers ( pass->getClearBuffers() ) ;
        cpy -> setScene ( pass->getScene() ) ;
        cpy -> setTechnique ( pass->getTechnique() ) ;
        cpy -> setCamera ( pass->getCamera() ) ;
        cpy -> setClearViewport ( pass->isClearViewport() ) ;

        GreDebug("[INFO] RenderPass '") << cpy->getName() << "' copied.'" << gendl ;

        iPasses.push_back(cpy) ;
        return cpy ;
    }

    return RenderPassHolder ( nullptr ) ;
}

void Renderer::clearPasses ()
{
    GreAutolock ; iPasses.clear() ;
}

void Renderer::draw(const RenderNodeHolder &node) const
{
    GreAutolock ;

    if ( !node.isInvalid() )
    {
        drawMesh ( node->getMesh() ) ;
    }
}

bool Renderer::installManagers ()
{
    GreAutolock ;

    iInstalled = true ;

    MeshManagerHolder meshmanager = iCreateMeshManager () ;

    if ( meshmanager.isInvalid() )
    {
        iInstalled = false ;
#ifdef GreIsDebugMode
        GreDebugPretty () << "No Mesh Manager can be installed." << Gre::gendl ;
#endif
    }
    else
    {
        if ( ResourceManager::Get()->getMeshManager().isInvalid() )
        {
            ResourceManager::Get()->setMeshManager ( meshmanager ) ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Mesh Manager installed." << Gre::gendl ;
#endif
        }
        else
        {
            iInstalled = false ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Mesh Manager already present and cannot be installed." << Gre::gendl ;
#endif
        }
    }

    TextureInternalCreator* texturemanager = iCreateTextureCreator () ;

    if ( !texturemanager )
    {
        iInstalled = false ;
#ifdef GreIsDebugMode
        GreDebugPretty () << "No Texture Manager can be installed." << Gre::gendl ;
#endif
    }
    else
    {
        if ( !ResourceManager::Get()->getTextureManager().isInvalid() )
        {
            ResourceManager::Get()->getTextureManager()->setInternalCreator(texturemanager) ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Texture Creator installed." << Gre::gendl ;
#endif
        }
        else
        {
            iInstalled = false ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Texture Manager not present and cannot be installed." << Gre::gendl ;
#endif
        }
    }

    RenderFramebufferInternalCreator* framebuffercreator = iCreateFramebufferCreator() ;

    if ( !framebuffercreator )
    {
        iInstalled = false ;
#ifdef GreIsDebugMode
        GreDebug("[WARN] Framebuffer Creator failed to be created.") << gendl ;
#endif
    }
    else
    {
        if ( !ResourceManager::Get()->getFramebufferManager().isInvalid() )
        {
            ResourceManager::Get()->getFramebufferManager()->setInternalCreator ( framebuffercreator ) ;
#ifdef GreIsDebugMode
            GreDebug("[INFO] Framebuffer Creator installed.") << gendl ;
#endif
        }
        else
        {
            iInstalled = false ;
#ifdef GreIsDebugMode
            GreDebug("[WARN] RenderFramebufferManager invalid.") << gendl ;
#endif
            delete framebuffercreator ;
        }
    }

    HardwareProgramManagerInternalCreator* pmanager = iCreateProgramManagerCreator () ;

    if ( !pmanager )
    {
        iInstalled = false ;
#ifdef GreIsDebugMode
        GreDebugPretty () << "[WARN] No HardwareProgramManager Creator can be installed." << Gre::gendl ;
#endif
    }
    else
    {
        if ( !ResourceManager::Get()->getHardwareProgramManager().isInvalid() )
        {
            ResourceManager::Get()->getHardwareProgramManager () -> setInternalCreator(pmanager) ;

#ifdef GreIsDebugMode
            GreDebugPretty () << "[INFO] HardwareProgramManager Creator installed." << Gre::gendl ;
#endif
        }
        else
        {
            iInstalled = false ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "[WARN] HardwareProgramManager invalid." << Gre::gendl ;
#endif
        }
    }

	if ( iInstalled && !ResourceManager::Get()->getRenderSceneManager().isInvalid() )
	{
		// Remember we must also initialize the RenderSceneManager.
		ResourceManager::Get()->getRenderSceneManager()->initialize() ;
	}

    return iInstalled ;
}

void Renderer::unload ( )
{
    GreAutolock ;

    iDefaultMaterial.clear() ;
    iContext.clear() ;

    if ( iInstalled )
    {
        ResourceManager::Get() ->setMeshManager ( MeshManagerHolder(nullptr) ) ;
        ResourceManager::Get() ->setTextureManager ( TextureManagerHolder(nullptr) ) ;
        ResourceManager::Get() ->setHardwareProgramManager ( HardwareProgramManagerHolder(nullptr) ) ;
        iInstalled = false ;
    }
}

bool Renderer::isInstalled () const
{
    GreAutolock ; return iInstalled ;
}

bool Renderer::isEnabled () const
{
    GreAutolock ; return iEnabled ;
}

void Renderer::setEnabled ( bool b )
{
    GreAutolock ; iEnabled = b ;
}

void Renderer::setRenderContext(const RenderContextUser &context)
{
    GreAutolock ; iContext = context.lock() ;
}

const RenderContextHolder& Renderer::getRenderContext() const
{
    GreAutolock ; return iContext ;
}

// ---------------------------------------------------------------------------------------------------

RendererLoader::RendererLoader()
{

}

RendererLoader::~RendererLoader()
{

}

// ---------------------------------------------------------------------------------------------------

RendererManager::RendererManager( const std::string& name )
: SpecializedResourceManager<Gre::Renderer, Gre::RendererLoader>(name)
{

}

RendererManager::~RendererManager() noexcept ( false )
{

}

RendererUser RendererManager::load(const std::string &name, const Gre::RendererOptions &options)
{
    GreAutolock ;

    if ( !name.empty() )
    {
        RendererUser tmp = findFirst(name);

        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Renderer Resource '" << name << "' already loaded." << Gre::gendl;
#endif
            return RendererUser ( nullptr );
        }

        for ( auto it = iLoaders.getLoaders().begin(); it != iLoaders.getLoaders().end(); it++ )
        {
            auto loader = it->second;

            if ( loader )
            {
                if ( loader->isCompatible(options) )
                {
                    RendererHolder rholder = loader->load(name, options);

                    if ( rholder.isInvalid() )
                    {
#ifdef GreIsDebugMode
                        GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by loader '" << it->first << "'." << Gre::gendl;
#endif
                    }

                    else
                    {
                        //////////////////////////////////////////////////////////////////////
                        // Parse Common options.
                        
                        auto op = options.find ( "Enabled" ) ;
                        if ( op != options.end() ) rholder->setEnabled ( op->second.toBool() ) ;
                        
                        //////////////////////////////////////////////////////////////////////
                        // Adds the renderer only for update events , and registers it to the
                        // manager.
                        
                        addFilteredListener( EventProceederUser(rholder) , { EventType::Update } ) ;
                        iHolders.push_back(rholder);
                        
                        return RendererUser ( rholder );
                    }
                }
            }
        }

#ifdef GreIsDebugMode
        GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by any installed Loader." << Gre::gendl;
#endif
        return RendererUser ( nullptr );
    }

    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return RendererUser ( nullptr );
    }
}

void RendererManager::render() const
{
    GreAutolock ;
    
    for ( auto renderer : iHolders )
    {
        //////////////////////////////////////////////////////////////////////
        // Calls the renderer's RenderPass'es only if this one is valid AND
        // enabled.
        
        if ( !renderer.isInvalid() )
        {
            if ( renderer->isEnabled() )
            {
                renderer -> render() ;
            }
        }
    }
}

GreEndNamespace
