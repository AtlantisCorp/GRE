//////////////////////////////////////////////////////////////////////
//
//  ResourceManager.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 01/11/2015.
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

#include "ResourceManager.h"

GreBeginNamespace

ResourceManagerHolder ResourceManager::iManagerHolder = ResourceManagerHolder () ;

// ---------------------------------------------------------------------------------------------------

ResourceManagerHolder ResourceManager::Create ( bool setdefault )
{
    ResourceManagerHolder ret ( new ResourceManager() ) ;

    if ( !ret.isInvalid() && setdefault ) {
        iManagerHolder = ret ;
    }

    return ret ;
}

void ResourceManager::Destroy ()
{
    if ( !iManagerHolder.isInvalid() )
    {
        iManagerHolder -> unload() ;
        iManagerHolder.clear();
    }
}

ResourceManagerHolder ResourceManager::Get()
{
    return iManagerHolder;
}

// ---------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager() : Resource ( )
, iRendererManager( nullptr ) , iWindowManager( nullptr ) , iRenderSceneManager( nullptr )
, iMaterialManager ( nullptr ) , iPluginManager ( nullptr ) , iMeshManager ( nullptr )
, iTextureManager( nullptr )
, iRenderContextManager( nullptr ) , iProgramManager( nullptr )
, iApplicationFactory ()
{
    iInitialized = false ;
}

void ResourceManager::initialize ()
{
    // Initialization of internal managers here. Notes that some managers has to be initialized by a plugin.
    // For example, MeshManager, TextureManager and RenderContextManager should be created by the active
    // Renderer. WindowManager should be specific from the Window Plugin.

	iPluginManager = new PluginManager () ;

	if ( iPluginManager.isInvalid() ) {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] Can't create PluginManager.") << gendl;
#endif
		iInitialized = false ;
		return ;
	}

#ifdef GreIsDebugMode
	GreDebug("[INFO] Created PluginManager.") << gendl;
#endif

    iRendererManager = new RendererManager () ;

	if ( iRendererManager.isInvalid() ) {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] Can't create RendererManager.") << gendl;
#endif
		iInitialized = false ;
		return ;
	}

#ifdef GreIsDebugMode
	GreDebug("[INFO] Created RendererManager.") << gendl;
#endif

    iRenderSceneManager = new RenderSceneManager () ;

	if ( iRenderSceneManager.isInvalid() ) {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] Can't create RenderSceneManager.") << gendl;
#endif
		iInitialized = false ;
		return ;
	}

#ifdef GreIsDebugMode
	GreDebug("[INFO] Created RenderSceneManager.") << gendl;
#endif

    iMaterialManager = new MaterialManager () ;

	if ( iMaterialManager.isInvalid() ) {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] Can't create MaterialManager.") << gendl;
#endif
		iInitialized = false ;
		return ;
	}

#ifdef GreIsDebugMode
	GreDebug("[INFO] Created MaterialManager.") << gendl;
#endif

    iTextureManager = new TextureManager () ;

    if ( iTextureManager.isInvalid() ) {
#ifdef GreIsDebugMode
        GreDebug("[ERRO] Can't create TextureManager.") << gendl;
#endif
        iInitialized = false ;
        return ;
    }

#ifdef GreIsDebugMode
    GreDebug("[INFO] Created TextureManager.") << gendl;
#endif

    iTechniqueManager = new TechniqueManager () ;

    if ( iTechniqueManager.isInvalid() ) {
#ifdef GreIsDebugMode
        GreDebug("[ERRO] Can't create TechniqueManager.") << gendl;
#endif
        iInitialized = false ;
        return ;
    }

#ifdef GreIsDebugMode
    GreDebug("[INFO] Created TechniqueManager.") << gendl;
#endif

    iFramebufferManager = new RenderFramebufferManager () ;

    if ( iFramebufferManager.isInvalid() ) {
#ifdef GreIsDebugMode
        GreDebug("[ERRO] Can't create RenderFramebufferManager.") << gendl ;
#endif
        iInitialized = false ;
        return ;
    }

#ifdef GreIsDebugMode
    GreDebug("[INFO] Created RenderFramebufferManager.") << gendl ;
#endif

    iProgramManager = new HardwareProgramManager () ;

    if ( iProgramManager.isInvalid() ) {
#ifdef GreIsDebugMode
        GreDebug("[ERRO] Can't create HardwareProgramManager.") << gendl ;
#endif
        iInitialized = false ;
        return ;
    }

#ifdef GreIsDebugMode
    GreDebug("[INFO] Created HardwareProgramManager.") << gendl ;
#endif

    iControllers = new ControllerManager () ;

    if ( iControllers.isInvalid() )
    {
        GreDebug ( "[ERRO] Can't create 'ControllerManager'." ) << gendl ;
        iInitialized = false ;
        return ;
    }

    GreDebug ( "[INFO] Created 'ControllerManager'." ) << gendl ;

	iInitialized = true ;

#ifdef GreIsDebugMode
    GreDebug("[INFO] Initialized.") << gendl ;
#endif
}

ResourceManager::~ResourceManager() noexcept(false)
{
    // unload () ;
}

void ResourceManager::unload ()
{
    GreAutolock ;

    // We have the necessity to destroy every managers before clearing every plugins
    // loaded, because some resource allocated in the plugins libraries will not be freed
    // correctly here (they will be freed when deattaching the library but the application malloc
    // system is not notified so we have a bad delete here). But, notes that the plugins are stopped
    // before clearing any manager, in order to let them free their resources correctly.

    if ( !iPluginManager.isInvalid() ) {
        iPluginManager -> callStops() ;
    }

    if ( !iControllers.isInvalid() )
    iControllers.clear () ;

    if ( !iFramebufferManager.isInvalid() ) {
        iFramebufferManager -> unload () ;
        iFramebufferManager.clear () ;
    }

    if ( !iTechniqueManager.isInvalid() ) {
        iTechniqueManager->unload() ;
        iTechniqueManager.clear() ;
    }

    if ( !iWindowManager.isInvalid() ) {
        iWindowManager->unload();
        iWindowManager.clear() ;
    }

    if ( !iMaterialManager.isInvalid() ) {
		iMaterialManager->unload();
        iMaterialManager.clear() ;
    }
    if ( !iMeshManager.isInvalid() ) {
		iMeshManager->unload();
        iMeshManager.clear() ;
    }

    if ( !iRenderSceneManager.isInvalid() ) {
		iRenderSceneManager->unload();
        iRenderSceneManager.clear() ;
    }
    if ( !iRendererManager.isInvalid() ) {
		iRendererManager->unload();
        iRendererManager.clear() ;
    }

    if ( !iProgramManager.isInvalid() ) {
        iProgramManager->unload();
        iProgramManager.clear() ;
    }
    if ( !iTextureManager.isInvalid() ) {
        iTextureManager->unload() ;
        iTextureManager.clear() ;
    }
    if ( !iRenderContextManager.isInvalid() ) {
        iRenderContextManager->unload() ;
        iRenderContextManager.clear() ;
    }

    if ( !iPluginManager.isInvalid() ) {
		iPluginManager->unload() ;
        iPluginManager.clear() ;
    }
}

bool ResourceManager::isInitialized() const
{
	return iInitialized ;
}

void ResourceManager::setApplicationFactory(const ApplicationLoaderFactory &appfactory)
{
    iApplicationFactory = appfactory ;
}

ApplicationLoaderFactory & ResourceManager::getApplicationFactory()
{
    return iApplicationFactory ;
}

void ResourceManager::setRendererManager(const RendererManagerHolder &rendmanager)
{
    iRendererManager = rendmanager ;
}

RendererManagerHolder ResourceManager::getRendererManager()
{
    return iRendererManager ;
}

void ResourceManager::setWindowManager(const WindowManagerHolder &winmanager)
{
    iWindowManager = winmanager ;
}

WindowManagerHolder ResourceManager::getWindowManager()
{
    return iWindowManager ;
}

void ResourceManager::setRenderSceneManager(const RenderSceneManagerHolder &rsceneholder)
{
    GreAutolock ; iRenderSceneManager = rsceneholder ;
}

RenderSceneManagerHolder ResourceManager::getRenderSceneManager()
{
    GreAutolock ; return iRenderSceneManager ;
}

void ResourceManager::setMeshManager(const MeshManagerHolder &mmholder)
{
    GreAutolock ; iMeshManager = mmholder ;
}

MeshManagerHolder ResourceManager::getMeshManager()
{
    GreAutolock ; return iMeshManager ;
}

void ResourceManager::setTextureManager ( const TextureManagerHolder & manager )
{
    GreAutolock ; iTextureManager = manager ;
}

TextureManagerHolder ResourceManager::getTextureManager()
{
    GreAutolock ; return iTextureManager ;
}

void ResourceManager::setRenderContextManager(const RenderContextManagerHolder &manager)
{
    GreAutolock ; iRenderContextManager = manager ;
}

RenderContextManagerHolder ResourceManager::getRenderContextManager()
{
    GreAutolock ; return iRenderContextManager ;
}

void ResourceManager::setHardwareProgramManager(const HardwareProgramManagerHolder &manager)
{
    GreAutolock ; iProgramManager = manager ;
}

HardwareProgramManagerHolder ResourceManager::getHardwareProgramManager()
{
    GreAutolock ; return iProgramManager ;
}

void ResourceManager::setMaterialManager(const MaterialManagerHolder &manager)
{
    GreAutolock ; iMaterialManager = manager ;
}

MaterialManagerHolder ResourceManager::getMaterialManager()
{
    GreAutolock ; return iMaterialManager ;
}

void ResourceManager::setPluginManager(const PluginManagerHolder &manager)
{
    GreAutolock ; iPluginManager = manager ;
}

PluginManagerHolder ResourceManager::getPluginManager()
{
    GreAutolock ; return iPluginManager ;
}

void ResourceManager::setTechniqueManager(const TechniqueManagerHolder &manager)
{
    GreAutolock ; iTechniqueManager = manager ;
}

TechniqueManagerHolder ResourceManager::getTechniqueManager()
{
    GreAutolock ; return iTechniqueManager ;
}

void ResourceManager::setFramebufferManager(const RenderFramebufferManagerHolder &manager)
{
    GreAutolock ; iFramebufferManager = manager ;
}

RenderFramebufferManagerHolder ResourceManager::getFramebufferManager()
{
    GreAutolock ; return iFramebufferManager ;
}

void ResourceManager::setControllerManager ( const ControllerManagerHolder & holder )
{
    GreAutolock ; iControllers = holder ;
}

ControllerManagerHolder ResourceManager::getControllerManager ()
{
    GreAutolock ; return iControllers ;
}

ResourceBundleHolder ResourceManager::addBundle ( const std::string & name )
{
    GreAutolock ;

    ResourceBundleHolder ret ( new ResourceBundle(name) ) ;
    iBundles.push_back ( ret ) ;
    return ret ;
}

ApplicationHolder ResourceManager::createApplication(const std::string &name,
                                                     const std::string &author,
                                                     const std::string &description)
{
    GreAutolock ;

    ApplicationLoader * loader = iApplicationFactory.getFirst() ;

    if ( loader ) {
        return loader->load(name, author, description) ;
    }
    return ApplicationHolder ( nullptr ) ;
}

std::string ResourceManager::findBundledFile(const ResourceType & type ,
                                             const std::string & filename) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Tries every bundles loaded in the ResourceManager.

    for ( const ResourceBundleHolder & bundle : iBundles )
    {
        if ( !bundle.isInvalid() )
        {
            auto dirs = bundle -> getDirectories(type) ;

            if ( !dirs.empty() )
            {
                //////////////////////////////////////////////////////////////////////
                // Try to find the filename in one of the directories.

                for ( auto dir : dirs )
                {
                    if ( Platform::FindFileInDirectory(dir, filename) )
                    {
                        return dir + Platform::GetSeparator() + filename ;
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////
    // In case of failure , we return just an empty string to indicate no
    // file were found.

    return std::string () ;
}

const std::vector < ResourceBundleHolder > & ResourceManager::getBundles () const
{
    GreAutolock ; return iBundles ;
}

GreEndNamespace
