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

ResourceManager::NameGenerator::NameGenerator()
{
    
}

ResourceManager::NameGenerator::~NameGenerator()
{
    
}

std::string ResourceManager::NameGenerator::generateName(const std::string& base)
{
    if(iUsedNames.find(base) == iUsedNames.end()) {
        iUsedNames[base] = 1;
        return base;
    } else {
        iUsedNames[base]++;
        return base + std::to_string(iUsedNames[base] - 1);
    }
}

// ---------------------------------------------------------------------------------------------------

void ResourceManager::Create()
{
    iManager = new ResourceManager;
}

void ResourceManager::Destroy()
{
    if(iManager)
    {
        delete iManager;
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Called ResourceManager::Destroy() more than once, or before ResourceManager::Create()." << std::endl;
    }
#endif
}

ResourceManager& ResourceManager::Get()
{
    return *iManager;
}

// ---------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager()
: Gre::Resource("DefaultResourceManager")
{
    
}

ResourceManager::~ResourceManager() noexcept(false)
{
    
}

ResourceHolder ResourceManager::loadEmptyResource(const std::string& name)
{
    ResourceHolder holder (new Resource(name));
    
#ifdef GreIsDebugMode
    if(!holder)
    {
        GreDebugPretty() << "Error allocating new Resource." << std::endl;
    }
#endif
    
    return holder;
}

void ResourceManager::clear ()
{
    iWindowManager.clear();
    iRendererManager.clear();
    iRenderSceneManager.clear();
    iMaterialManager.clear();
    iPluginManager.clear();
}

unsigned ResourceManager::getResourceUsage() const
{
    return Pool<Pools::Resource> :: Get().getCurrentSize();
}

ResourceManager::NameGenerator& ResourceManager::getNameGenerator()
{
    return iNameGenerator;
}

int ResourceManager::loadPluginsIn(const std::string &dirname)
{
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
            
            Plugin plugin = iPluginManager.load(std::string(ent->d_name) + "-plugin", dirname + "/" + ent->d_name);
            
#ifdef GreIsDebugMode
            if( plugin.isInvalid() )
            {
                GreDebugPretty() << "Couldn't load plugin '" << ent->d_name << "'." << std::endl;
                continue;
            }
#endif
            
            plugin.start();
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

void ResourceManager::setCloseBehaviour(const CloseBehaviour &behaviour)
{
    iCloseBehaviour = behaviour;
}

void ResourceManager::addLoopBehaviour(LoopBehaviour behaviour)
{
    iLoopBehaviours.add(behaviour);
}

void ResourceManager::clearLoopBehaviour()
{
    iLoopBehaviours.clear();
}

void ResourceManager::stop()
{
    iMustStopLoop = true;
}

void ResourceManager::loop()
{
    iMustStopLoop = false;
    
    while( !iMustStopLoop )
    {
        _updateElapsedTime();
        
        UpdateEvent updateEvent;
        updateEvent.elapsedTime = iElapsedTime;
        
        // Normally, the ResourceManager's loop should take place in a different thread,
        // in order to update the Windows and the Scenes objects separately.
        
        // Every object can be accepted as a Listener to listen for update event from this
        // class, but take in mind that, for example, RenderScene will send this event to
        // every RenderNode, Camera, etc... and Window object will send it to RenderContext
        // which will send it to Renderer.
        
        // THEORY : The Render loop should be done in the main thread (i.e. this function). This
        // is done by updating every Window objects. The update of its surface should update the
        // Renderer object and thus re-draw the Scene.
        //     The logical loop (which is, for now, only the RenderScene updating), should take
        // place in another thread.
        // NOTES : The Render loop's time should not excess 16,6 ms (60FPS).
        
        // First, we update every Windows objects.
        WindowHolderList whlist = iWindowManager.getWindows();
        
        if(whlist.empty() && iCloseBehaviour == CloseBehaviour::AllWindowClosed)
        {
            iMustStopLoop = true;
        }
        
        else
        {
            if( !whlist.empty() )
            {
                for(auto wholder : whlist)
                {
                    if(wholder)
                    {
                        /*
                         
                         This should be done in WindowPrivate::onUpdateEvent()
                         
                        wholder->pollEvent();
                        wholder->update();
                        
                        if(wholder->hasBeenClosed())
                        {
                            // Window has been closed, we can destroy it.
                            iWindowManager.remove(wholder.getName());
                        }
                         */
                        
                        wholder->onEvent(updateEvent);
                        iPerWindowBehaviours.call();
                    }
                }
            }
            
            iLoopBehaviours.call();
        }
        
        
                        /* This should be done by the renderer. 
                         The Renderer should draw every Window object as RenderTarget.
                         
                        // We begin updating the Window.
                        // This permits pollEvent, and Context binding.
                        // The Context should notifiate the Renderer that the Current Context
                        // has changed.
                        windowUser.beginUpdate();
                        
                        // Then we use the Window's Context to render the Scene defined for
                        // the renderer. Renderer object renders the Window as a normal RenderTarget.
#ifndef GreIsDebugMode
                        windowUser.getRenderContext().getRenderer().render(windowUser);
#else
                        RenderContext ctxt = windowUser.getRenderContext();
                        Renderer renderer = ctxt.getRenderer();
                        renderer.render(windowUser);
#endif
                        
                        // Finally we end the Window's update.
                        windowUser.endUpdate();
                         
                         */
        
        // Then, we update also the RenderScene's objects.
        RenderSceneHolderList smhlist = iRenderSceneManager.getAll();
        
        if( !smhlist.empty() )
        {
            for(auto smholder : smhlist)
            {
                if( smholder )
                {
                    smholder->onEvent(updateEvent);
                }
            }
        }
        
        // Finally we send update event to every Listeners.
        sendEvent(updateEvent);
    }
}

KeyboardLoader& ResourceManager::getKeyboardLoader()
{
    return iKeyboardLoader;
}

KeyboardHolder ResourceManager::createKeyboard(const std::string &name)
{
    return loadResourceFrom<KeyboardHolder>(getKeyboardLoader() , name);
}

void ResourceManager::_updateElapsedTime()
{
    if(iLastUpdate != iLastUpdate.min())
    {
        UpdateTime now = UpdateChrono::now();
        iElapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - iLastUpdate);
        iLastUpdate = now;
    }
    else
    {
        iLastUpdate = UpdateChrono::now();
    }
}

MaterialManager& ResourceManager::getMaterialManager()
{
    return iMaterialManager;
}

const MaterialManager& ResourceManager::getMaterialManager() const
{
    return iMaterialManager;
}

RendererManager& ResourceManager::getRendererManager()
{
    return iRendererManager;
}

const RendererManager& ResourceManager::getRendererManager() const
{
    return iRendererManager;
}

RenderSceneManager& ResourceManager::getRenderSceneManager()
{
    return iRenderSceneManager;
}

const RenderSceneManager& ResourceManager::getRenderSceneManager() const
{
    return iRenderSceneManager;
}

// ---------------------------------------------------------------------------------------------------

std::string ResourceManager::Helper::ChooseRenderer(RendererLoaderFactory& rFactory)
{
    // First of all, you should create the Renderer which will render your Scene.
    StringList renderers = rFactory.getLoadersName();
    GreDebugPretty() << "Please choose a Renderer in the List : " << std::endl;
    GreDebugPretty() << Gre::DebugListNumeroted(renderers) << std::endl;
    GreDebugPretty() << ":> ";
    
    int rNum = Gre::DebugGetNumber();
    return renderers[rNum-1];
}

Renderer ResourceManager::Helper::LoadRenderer(const std::string& rname, const std::string& lname, RendererLoaderFactory& rFactory)
{
    RendererLoader* rLoader = rFactory.get(lname);
    if(rLoader)
    {
        RendererHolder holder = ResourceManager::Get().loadResourceWith<RendererHolder>(rLoader, rname);
        
#ifdef GreIsDebugMode
        if(!holder)
        {
            GreDebugPretty() << "Couldn't load Renderer '" << rname << "'." << std::endl;
        }
#endif
        
        ResourceManager::Get().getRendererManager().load(holder);
        Renderer user(holder);
        
        // Another method would have been :
        // Renderer user = ResourceManager::Get().getRendererManager().create(rname, rLoader);
        
        return user;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Loader '" << lname << "' not found." << std::endl;
#endif
        
        return Renderer::Null;
    }
}

std::string ResourceManager::Helper::ChooseWindowLoader(WindowLoaderFactory& wFactory)
{
    StringList windowloaders = wFactory.getLoadersName();
    GreDebugPretty() << "Please choose a Window Loader in the List : " << std::endl;
    GreDebugPretty() << Gre::DebugListNumeroted(windowloaders) << std::endl;
    GreDebugPretty() << ":> ";
    
    int wNum = Gre::DebugGetNumber();
    return windowloaders[wNum-1];
}

Window ResourceManager::Helper::LoadWindow(const std::string& wname, const std::string& lname, WindowLoaderFactory& wFactory, const Surface& param)
{
    WindowLoader* wLoader = wFactory.get(lname);
    if(wLoader)
    {
        WindowHolder holder = ResourceManager::Get().loadResourceWith<WindowHolder>(wLoader, wname, param.left, param.top, param.width, param.height);
        
#ifdef GreIsDebugMode
        if(!holder)
        {
            GreDebugPretty() << "Couldn't load Window '" << wname << "'." << std::endl;
        }
#endif
        
        Window user(holder);
        ResourceManager::Get().getWindowManager().load(holder);
        
        // Another method would have been :
        // Window user = ResourceManager::Get().getWindowManager().create(wname, wLoader, param.left, param.top, param.width, param.height);
        
        return user;
    }
    
    else
    {
        return Window::Null;
    }
}

WindowManager& ResourceManager::getWindowManager()
{
    return iWindowManager;
}

const WindowManager& ResourceManager::getWindowManager() const
{
    return iWindowManager;
}

GreEndNamespace

