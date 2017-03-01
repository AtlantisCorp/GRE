//////////////////////////////////////////////////////////////////////
//
//  ResourceManager.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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

ResourceManager* iManager = nullptr;

// ---------------------------------------------------------------------------------------------------

void ResourceManager::Create()
{
    if ( !iManager )
        iManager = new ResourceManager;

#ifdef GreIsDebugMode
    if ( !iManager ) {
        GreDebug("[ERRO] Can't allocate ResourceManager.") << gendl;
    } else {
		GreDebug("[INFO] ResourceManager created.") << gendl;
	}
#endif
}

bool ResourceManager::CreateAndInitialize()
{
	ResourceManager::Create() ;
	
	if ( !iManager ) {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] Can't create ResourceManager.") << gendl;
#endif
		return false ;
	}
	
#ifdef GreIsDebugMode
	GreDebug("[INFO] Created ResourceManager.") << gendl;
#endif
	ResourceManager::Get().initialize() ;
	
	if ( iManager->isInitialized() ) {
#ifdef GreIsDebugMode
		GreDebug("[INFO] ResourceManager created and initialized.") << gendl;
#endif
		return true ;
	}
	
	else {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] ResourceManager can't be initialized.") << gendl;
#endif
		return false ;
	}
}

void ResourceManager::Destroy()
{
	if ( iManager )
	{
		Application::Destroy() ;
		
		delete iManager ;
		iManager = nullptr ;
		
#ifdef GreIsDebugMode
		GreDebug("[INFO] ResourceManager destroyed.") << gendl;
#endif
	}
}

ResourceManager& ResourceManager::Get()
{
    return *iManager;
}

// ---------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager()
: Lockable ( )
, iRendererManager( nullptr ) , iWindowManager( nullptr ) , iRenderSceneManager( nullptr )
, iMaterialManager ( nullptr ) , iPluginManager ( nullptr ) , iMeshManager ( nullptr )
, iAnimatorManager ( nullptr ) , iCameraManager ( nullptr ) , iTextureManager( nullptr )
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
	GreDebug("Created PluginManager.") << gendl;
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
	GreDebug("Created RendererManager.") << gendl;
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
	GreDebug("Created RenderSceneManager.") << gendl;
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
	GreDebug("Created MaterialManager.") << gendl;
#endif
    
    iAnimatorManager = new AnimatorManager () ;
	
	if ( iAnimatorManager.isInvalid() ) {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] Can't create AnimatorManager.") << gendl;
#endif
		iInitialized = false ;
		return ;
	}
	
#ifdef GreIsDebugMode
	GreDebug("Created AnimatorManager.") << gendl;
#endif
	
    iCameraManager = new CameraManager () ;
    
	if ( iCameraManager.isInvalid() ) {
#ifdef GreIsDebugMode
		GreDebug("[ERRO] Can't create CameraManager.") << gendl;
#endif
		iInitialized = false ;
		return ;
	}
	
#ifdef GreIsDebugMode
	GreDebug("Created CameraManager.") << gendl;
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
    GreDebug("Created TextureManager.") << gendl;
#endif

	iInitialized = true ;
}

ResourceManager::~ResourceManager() noexcept(false)
{
    unload () ;
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
    
    if ( !iWindowManager.isInvalid() ) {
        iWindowManager->unload();
        iWindowManager.clear() ;
    }
    
    if ( !iCameraManager.isInvalid() ) {
		iCameraManager->unload();
        iCameraManager.clear() ;
    }
    if ( !iAnimatorManager.isInvalid() ) {
		iAnimatorManager->unload() ;
        iAnimatorManager.clear() ;
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

void ResourceManager::setAnimatorManager(const AnimatorManagerHolder &animholder)
{
    GreAutolock ; iAnimatorManager = animholder ;
}

AnimatorManagerHolder ResourceManager::getAnimatorManager()
{
    GreAutolock ; return iAnimatorManager ;
}

void ResourceManager::setCameraManager(const CameraManagerHolder &camholder)
{
    GreAutolock ; iCameraManager = camholder ;
}

CameraManagerHolder ResourceManager::getCameraManager()
{
    GreAutolock ; return iCameraManager ;
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

int ResourceManager::loadPluginsIn(const std::string &dirname)
{
    GreAutolock ;
    
    int res = 0;
    DIR *dir;
    struct dirent *ent;
    
    if ( (dir = opendir (dirname.c_str())) != NULL )
    {
        /* print all the files and directories within directory */
        while ( (ent = readdir (dir)) != NULL )
        {
            std::string d_name(ent->d_name);
            if (d_name == "." || d_name == "..")
                continue;
            
            std::string plugname = d_name + "-plugin" ;
            std::string plugpath = dirname + "/" + d_name ;
            
            PluginUser plugin = iPluginManager->load(plugname, plugpath);
            
            {
                auto pluginholder = plugin.lock();
                if ( pluginholder.isInvalid() ) {
#ifdef GreIsDebugMode
                    GreDebug("[WARN] Plugin '") << d_name << "' not loaded correctly." << Gre::gendl;
#endif
                } else {
                    pluginholder -> start() ;
#ifdef GreIsDebugMode
                    GreDebug("[INFO] Plugin '") << pluginholder -> getName() << "' started." << Gre::gendl ;
#endif
                    res ++ ;
                }
            }
        }
        
        closedir (dir);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebug("[WARN] Directory '") << dirname << "' not found." << Gre::gendl ;
#endif
        return 0;
    }
    
    return res;
}

GreEndNamespace

