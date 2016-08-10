//////////////////////////////////////////////////////////////////////
//
//  ResourceManager.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 28/10/2015.
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

#ifndef GResource_ResourceManager_h
#define GResource_ResourceManager_h

#include "Resource.h"
#include "TextResource.h"
#include "Plugin.h"
#include "Window.h"
#include "Renderer.h"
#include "MeshLoader.h"
#include "Scene.h"
#include "FrameBuffer.h"
#include "LoopBehaviours.h"
#include "Keyboard.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @class ResourceManager
/// @brief Manages every Resource, ResourceFactory.
///
/// The ResourceManager is a Singleton, that can be accessed from any point
/// of any application or program linked with the main library.
///
/// # Application
///
/// A program should always imbricate operations relating to the ResourceManager
/// in a try/catch block. This is a way to destroy everything correctly when
/// having an exception launched.
///
/// Right in the try block, a user should call ResourceManager::Create()
/// BEFORE using it, in order to initialize properly the Singleton.
///
/// At the end of the try block, the user should always call
/// ResourceManager::Destroy() , in order to properly destroy every Resource
/// objects allocated in the ResourceManager, and thus before the end of
/// the try/catch block.
///
/// # Plugins / Libraries
///
/// You can directly use the ResourceManager, but a call to ResourceManager::Get()
/// is not guaranteed to be successfull, because the main program normally should
/// have called ResourceManager::Create(), but that is not sure. You also
/// can imbricate your calls in a try/catch block.
///
/// # Uses to load Resources
///
/// You have ways to load Resource :
///   - Use the appropriate Manager (like HardwareProgramManager for HardwareProgram
/// objects) , using the function ::createSomething().
///   - Use the ResourceManager loading system.
///
/// ## The ResourceManager loading system
///
/// For example, if you want to load a Mesh object. You can either use
/// MeshManager::createFromFile() or use the ResourceManager. The big
/// difference is that the ResourceManager loading system returns to you
/// an object where you have the owning.
///
/// ```c++
/// MeshHolder mymesh = ResourceManager::Get() .loadResourceWith(ResourceManager::Get().getMeshLoaderFactory()..., args...);
/// Mesh mymeshuser = ResourceManager::Get().getMeshManager().add(mymesh);
/// ```
///
/// The ::loadResourceWith() function can take a variable number of args. Those
/// args are dependent of the Loader you will use. Some Loaders wants plenty of
/// args (for options), some only wants the Resource's name.
///
/// Whenever possible, you should use the Manager's functions to load your
/// Resource's, because they handle the ResourceHolder directly.
///
/// ## ResourceManager's holding
///
/// You can make the ResourceManager holds some Resource for some reasons. You
/// can use add() and remove() function in order to register ResourceHolder objects.
/// Those objects will be destroyed when applying a call to ::Destroy(), or ::clear(),
/// or clearResources().
///
/// @see Resource, ResourceFactory
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceManager : public Resource
{
public:
    
    POOLED(Pools::Manager)
    
    /// @brief An object that generate names for a resource.
    /// Example : You want to add 10 resources with name "MyName", it will generate
    /// automaticly "MyName", "MyName0", "MyName1", ... "MyName8"
    class NameGenerator
    {
    public:
        
        NameGenerator();
        ~NameGenerator();
        
        /// @return A Name based on current used copies.
        std::string generateName(const std::string& base = "default");
        
    private:
        
        std::map<std::string, unsigned> iUsedNames;
    };
    
protected:
    
    /// @brief Factory to create FileLoaders.
    FileLoaderFactory iFileloaders;
    
    /// @brief Factory to create WindowLoaders.
    WindowLoaderFactory iWindowLoaders;
    
    /// @brief Factory to create RendererLoaders.
    RendererLoaderFactory iRendererLoaders;
    
    /// @brief Factory to create MeshLoaders.
    MeshLoaderFactory iMeshLoaders;
    
    /// @brief Creates SceneLoader objects.
    SceneManagerLoaderFactory iSceneLoaders;
    
    /// @brief Creates HardwareShaderLoader objects.
    HardwareShaderLoaderFactory iShaderLoaders;
    
    /// @brief Creates HardwareProgramManagerLoader objects.
    HardwareProgramManagerLoaderFactory iProgManLoaders;
    
    /// @brief Creates FrameBufferLoader objects.
    RenderFramebufferLoaderFactory iFboLoaders;
    
    /// @brief Utility class to create Names.
    NameGenerator iNameGenerator;
    
    /// @brief A simple Keyboard Loader.
    KeyboardLoader iKeyboardLoader;
    
    /// @brief Determines when the ResourceManager should stop the Loop.
    CloseBehaviour iCloseBehaviour;
    
    /// @brief Holds LoopBehaviour's functions.
    LoopBehaviours iLoopBehaviours;
    
    /// @brief Holds LoopBehaviour's functions for PerWindow loop.
    LoopBehaviours iPerWindowBehaviours;
    
    /// @brief A property that take effect only when the Loop has started.
    /// When the Loop starts, this property is set to false. When the User
    /// or any other events calls ResourceManager::stop(), this property is
    /// set to true.
    bool iMustStopLoop;
    
    /// @brief The Elapsed Time delta.
    UpdateClock iElapsedTime;
    
    /// @brief The current Elapsed Time since app started.
    UpdateTime iLastUpdate;
    
    /// @brief Holds Resource's owned by the ResourceManager.
    std::list<ResourceHolder> iResources;
    
    /// @brief WindowManager.
    WindowManager iWindowManager;
    
    /// @brief SceneManager's Manager.
    SceneManagerManager iSceneManagerManager;
    
public:
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates the ResourceManager singleton object.
    /// @note This function should be called only once per program. Behaviour
    /// is undefined when called more times.
    ////////////////////////////////////////////////////////////////////////
    static void Create();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Destroys the ResourceManager object.
    /// @note This function should be called only once per program. Behaviour
    /// is undefined when called more times.
    ////////////////////////////////////////////////////////////////////////
    static void Destroy();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the ResourceManager singleton object.
    /// @note ResourceManager::Create() must have been called before using
    /// this function.
    ////////////////////////////////////////////////////////////////////////
    static ResourceManager& Get();
    
private:
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates the ResourceManager.
    /// By default, the Constructor registers two FileLoader, the TextLoader
    /// (under name "TextLoader") and the PluginLoader (under name "PluginLoader").
    /// The PluginLoader is the only loader which is necessary to the Engine, to
    /// operate normally.
    ////////////////////////////////////////////////////////////////////////
    ResourceManager();
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ResourceManager(const ResourceManager& rhs) = delete;
    
public:
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Destroys the ResourceManager and, all the Resource objects.
    ////////////////////////////////////////////////////////////////////////
    virtual ~ResourceManager ();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Adds a new Resource, created by the user.
    /// This resource is owned by the ResourceManager only.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceUser addResource(const ResourceHolder& holder);
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates a generic empty Resource.
    /// This Resource can be, for example, used as a Listener.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceHolder loadEmptyResource(const std::string& name);
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a Resource and return it to the form of a ResourceHolder
    /// of given type.
    ///
    /// Using the Resource::Type assertions should be avoided. The ResourceLoader
    /// should have a load() function of type :
    ///
    ///     MyResourceHolder load ( const std::string& name, myargs ... ) const ;
    ///
    /// This function waits for a referenced loader. This loader is not destroyed
    /// in this function, so if you need this behaviour please use a pointer
    /// instead.
    ////////////////////////////////////////////////////////////////////////
    template <class Holder , class Loader , class ... Args>
    Holder loadResourceWith ( const Loader& loader , const std::string& name , Args&& ... args )
    {
        Holder retvalue = loader->load(name , std::forward<Args>(args)...);
        
#ifdef GreIsDebugMode
        if ( retvalue.isInvalid() )
        {
            GreDebugPretty() << "Can't load Resource '" << name << "'." << std::endl;
        }
#endif

        return retvalue;
    }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a Resource and return it to the form of a ResourceHolder
    /// of given type.
    ///
    /// Using the Resource::Type assertions should be avoided. The ResourceLoader
    /// should have a load() function of type :
    ///
    ///     MyResourceHolder load ( const std::string& name, myargs ... ) const ;
    ///
    /// This function waits for a pointer to a Loader. This loader is destroyed
    /// by this function. If you don't need this behaviour, use ::loadResourceWith()
    /// with a referenced loader instead of a pointer.
    ////////////////////////////////////////////////////////////////////////
    template <class Holder , class Loader , class ... Args>
    Holder loadResourceWith ( const Loader* loader , const std::string& name , Args&& ... args )
    {
        if ( loader )
        {
            Holder retvalue = loader->load(name , std::forward<Args>(args)...);
            
#ifdef GreIsDebugMode
            if ( retvalue.isInvalid() )
            {
                GreDebugPretty() << "Can't load Resource '" << name << "'." << std::endl;
            }
#endif
            
            delete loader;
            return retvalue;
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No loader provided to load Resource '" << name << "'." << std::endl;
#endif
            
            return Holder ( nullptr );
        }
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first Resource that has given name.
    //////////////////////////////////////////////////////////////////////
    virtual ResourceUser findResourceByName(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unload Resource giving its name.
    //////////////////////////////////////////////////////////////////////
    virtual void unloadResource(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unload every Resource objects.
    //////////////////////////////////////////////////////////////////////
    virtual void clear ();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Memory used by every ResourceObject, using the
    /// Pool system.
    //////////////////////////////////////////////////////////////////////
    virtual unsigned getResourceUsage() const;
    
    /// @brief Returns the FileLoader Factory.
    FileLoaderFactory& getFileLoaderFactory();
    /// @brief Returns the WindowLoader Factory.
    WindowLoaderFactory& getWindowLoaderFactory();
    /// @brief Returns the RendererLoader Factory.
    RendererLoaderFactory& getRendererLoaderFactory();
    /// @brief Returns the MeshLoader Factory
    MeshLoaderFactory& getMeshLoaderFactory();
    /// @brief Returns the SceneLoader Factory.
    SceneManagerLoaderFactory& getSceneManagerLoaderFactory();
    /// @brief Returns the NameGenerator object.
    NameGenerator& getNameGenerator();
    /// @brief Returns the HardwareShaderLoader Factory.
    HardwareShaderLoaderFactory& getHardwareShaderLoaderFactory();
    /// @brief Returns the HardwareProgramManagerLoader Factory.
    HardwareProgramManagerLoaderFactory& getHardwareProgramManagerLoaderFactory();
    /// @brief Returns the FrameBufferLoader Factory.
    RenderFramebufferLoaderFactory& getFrameBufferLoaderFactory();
    /// @brief Returns the Keyboard Loader.
    KeyboardLoader& getKeyboardLoader();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads every plugins in given directory name.
    ///
    /// Plugins loaded are every files finishing with '.so' for Linux,
    /// '.dylib' for OsX, or '.dll' for Windows' users.
    ///
    /// @note The directory must be different from the Working Directory.
    //////////////////////////////////////////////////////////////////////
    virtual int loadPluginsIn(const std::string& dirname);
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Set the CloseBehaviour.
    ////////////////////////////////////////////////////////////////////////
    virtual void setCloseBehaviour(const CloseBehaviour& behaviour);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a loop behaviour function.
    //////////////////////////////////////////////////////////////////////
    virtual void addLoopBehaviour(LoopBehaviour behaviour);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Erases every Loop Behaviours.
    //////////////////////////////////////////////////////////////////////
    virtual void clearLoopBehaviour();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Stops the current loop processing.
    /// This is done by simply setting the property _mMustStopLoop to true.
    //////////////////////////////////////////////////////////////////////
    virtual void stop();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Window objects. 
    //////////////////////////////////////////////////////////////////////
    virtual void loop();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Resource that should be used only for Event handling/
    /// sending.
    /// @deprecated
    /// This function is the same as ::loadEmptyResource().
    //////////////////////////////////////////////////////////////////////
    virtual ResourceHolder createPureListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a simple Keyboard Resource.
    //////////////////////////////////////////////////////////////////////
    virtual KeyboardHolder createKeyboard(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Window Manager.
    //////////////////////////////////////////////////////////////////////
    virtual WindowManager& getWindowManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Window Manager.
    //////////////////////////////////////////////////////////////////////
    virtual const WindowManager& getWindowManager() const;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Time elapsed between each turn in ::loop().
    //////////////////////////////////////////////////////////////////////
    void _updateElapsedTime();
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unites some Utilities function to help the user do some
    /// basics things.
    //////////////////////////////////////////////////////////////////////
    class DLL_PUBLIC Helper
    {
    public:
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Let the final user choose a RendererLoader using Console
        /// prompting.
        ///
        /// It returns the name of the Chosen Renderer.
        //////////////////////////////////////////////////////////////////////
        static std::string ChooseRenderer(RendererLoaderFactory& rFactory);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Load a Renderer object using Renderer Name, Loader Name
        /// and the RendererLoaderFactory.
        //////////////////////////////////////////////////////////////////////
        static Renderer LoadRenderer(const std::string& rname, const std::string& lname, RendererLoaderFactory& rFactory);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Let the final user choose a WindowLoader using Console
        /// prompting.
        ///
        /// It returns the name of the Chosen Window.
        //////////////////////////////////////////////////////////////////////
        static std::string ChooseWindowLoader(WindowLoaderFactory& wFactory);
        
        //////////////////////////////////////////////////////////////////////
        /// @brief Load a Window Object using its Name, Loader Name, Window
        /// Loader Factory, and a Surface that corresponds to the future size
        /// of the Window.
        //////////////////////////////////////////////////////////////////////
        static Window LoadWindow(const std::string& wname, const std::string& lname, WindowLoaderFactory& wFactory, const Surface& param);
    };
};

GreEndNamespace
#endif
