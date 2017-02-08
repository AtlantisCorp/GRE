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
    iManager = new ResourceManager;
}

void ResourceManager::Destroy()
{
    if( iManager )
    {
        delete iManager;
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Called ResourceManager::Destroy() more than once, or before ResourceManager::Create()." << Gre::gendl;
    }
#endif
}

ResourceManager& ResourceManager::Get()
{
    return *iManager;
}

// ---------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager()
: Lockable ( )
, iRendererManager( new RendererManager() )
, iWindowManager( nullptr )
, iRenderSceneManager( new RenderSceneManager() )
, iMaterialManager ( new MaterialManager() )
, iPluginManager ( new PluginManager() )
, iMeshManager ( nullptr )
, iAnimatorManager ( new AnimatorManager() )
, iCameraManager ( new CameraManager() )
, iTextureManager( nullptr )
, iRenderContextManager( nullptr )
, iProgramManager( nullptr )
, iApplicationFactory ()
, iGlobalEventDispatcher( new EventDispatcher("GlobalEventDispatcher") )
{
    GreAutolock ;
    
#ifdef GreIsDebugMode
    
    if ( iGlobalEventDispatcher.isInvalid() )
    {
        throw GreConstructorException("ResourceManager", "Error loading 'iGlobalEventDispatcher'") ;
    }
    
#endif
    
    iGlobalEventDispatcher->start() ;
}

ResourceManager::~ResourceManager() noexcept(false)
{
    
}

void ResourceManager::unload ()
{
    GreAutolock ;
    
    iCameraManager->unload();
    iAnimatorManager->unload();
    iMaterialManager->unload();
    iMeshManager->unload();
    iRenderSceneManager->unload();
    iWindowManager->unload();
    iRendererManager->unload();
    iPluginManager->unload();
}

void ResourceManager::setApplicationFactory(const ApplicationLoaderFactory &appfactory)
{
    GreAutolock ; iApplicationFactory = appfactory ;
}

ApplicationLoaderFactory & ResourceManager::getApplicationFactory()
{
    GreAutolock ; return iApplicationFactory ;
}

void ResourceManager::setRendererManager(const RendererManagerHolder &rendmanager)
{
    GreAutolock ; iRendererManager = rendmanager ;
}

RendererManagerHolder ResourceManager::getRendererManager()
{
    GreAutolock ; return iRendererManager ;
}

void ResourceManager::setWindowManager(const WindowManagerHolder &winmanager)
{
    GreAutolock ; iWindowManager = winmanager ;
}

WindowManagerHolder ResourceManager::getWindowManager()
{
    GreAutolock ; return iWindowManager ;
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
            
            PluginUser plugin = iPluginManager->load(std::string(ent->d_name) + "-plugin", dirname + "/" + ent->d_name);
            
#ifdef GreIsDebugMode
            if( plugin.isInvalid() )
            {
                GreDebugPretty() << "Couldn't load plugin '" << ent->d_name << "'." << Gre::gendl;
                continue;
            }
#endif
            
            plugin.lock() ->start();
        }
        
        closedir (dir);
    }
    
    else
    {
        /* could not open directory */
        perror("");
        return 0;
    }
    
    return res;
}

GreEndNamespace

