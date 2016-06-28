//
//  Resource.h
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//
/*
#ifndef GResource_Resource_h
#define GResource_Resource_h

#include "Pools.h"
#include "Variant.h"
#include "Event.h"

GreBeginNamespace

class ResourceManager;
class Resource;
//class ResourceUser;
class ResourceLoader;

////////////////////////////////////////////////////////////////////////
/// @class Resource
/// @brief An Object that can be loaded and unloaded.
///
/// Basically, a Resource is an Object owned by the ResourceManager. You
/// can load it using a ResourceLoader. A Resource can represent anything,
/// something material like a file, a photo, but also something software,
/// like a Window where you can render in, a Renderer that you can use to
/// render into the Window.
///
/// A Resource is loaded using the right loader corresponding to its type.
/// For now, there are a limited amount of type for any Resource, but this
/// list is intended to be longer.
///
/// For example, if you want to load a '.txt' file, i.e. a simple and basic
/// text file, you will use the TextLoader and use the Resource::Type::Text
/// type. The Resource::Type field is used by the loader to ensure you know
/// the real type of a file (so you can load any file you want as text using
/// the Resource::Type::Text type) .
///
/// The Resource is unloaded by the ResourceManager. For now, you have two
/// options conecerning Resource unloading management :
/// - Let the ResourceManager unload it at destruction, i.e. at the end of the
/// program using the ResourceManager::Destroy() function.
/// - Using the ResourceManager::Get().unloadResource() function.
///
/// The second function let you manage manually the amount of currently loaded
/// Resource objects.
///
/// @see ResourceUser, ResourceLoader
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Resource
{
public:
    
    friend class ResourceManager;
    
    /// @brief Enumerates every types (not updated).
    enum class DLL_PUBLIC Type {
        Null,           ///< @brief A Null Type. Should not be used.
        
        // Registered in ResourceManager
        
        Text,           ///< @brief Text type : to load files as text.
        Plugin,         ///< @brief Plugin Type : to load a file as a plugin.
        Window,         ///< @brief Window Type : to load a Window (specific type for Window Loaders).
        Renderer,       ///< @brief Renderer Type : to load a Renderer (specific type for Renderer Loaders).
        Mesh,           ///< @brief Mesh Type : A resource representing a 3D object.
        HwdBuffer,      ///< @brief Hardware Buffer : Resource to store Hardware Buffers and data.
        Image,          ///< @brief Image : Speak by itself...
        Texture,        ///< @brief Textures objects. Can only be loaded by the Renderer.
        Scene,          ///< @brief A simple Type to describe a Scene object.
        HwdProgManager, ///< @brief For HardwareProgramManager.
        HdwShader,      ///< @brief HardwareShader Resource.
        FrameBuff,      ///< @brief FrameBuffer Resource.
        PureListener,   ///< @brief A PureListener Resource Object is an Object which only listen to others.
        Keyboard        ///< @brief A Listener specialized in listening Key Events.
        
        // Others
    };
    
    POOLED(Pools::Resource);
    
    /// @brief Constructs the Resource using its name.
    Resource (const std::string& name);
    
    /// @brief Destructs the Resource. (Do nothing)
    virtual ~Resource ();
    
    /// @brief Returns the name of the Resource.
    const std::string& getName() const;
    
    /// @brief Returns the Data structure, if one.
    /// @warning This function is not safe at all. You should use the functions
    /// in ResourceUser to use your Resource. Nothing is guaranteed with this function,
    /// except for TextResource which is a special basic Resource (this function so return
    /// a std::string containing the whole file in text).
    template<typename T>
    const T& getData() const
    { return *reinterpret_cast<const T*>(_getData()); }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns expected data for user.
    //////////////////////////////////////////////////////////////////////
    virtual const void* getCustomData(const std::string& dataname) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns expected data directly templated.
    //////////////////////////////////////////////////////////////////////
    template<typename DataType>
    const DataType& getCustomDataTemplated(const std::string& name)
    {
        return reinterpret_cast<DataType&>(getCustomData(name));
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Stores Variant data at given index.
    //////////////////////////////////////////////////////////////////////
    void storeVariantData(int index, const Variant& data);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Variant data at given index.
    /// @note Index inferior to 0 are reserved.
    //////////////////////////////////////////////////////////////////////
    Variant& getVariantData(int index);
    const Variant& getVariantData(int index) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Type for this Resource.
    //////////////////////////////////////////////////////////////////////
    Type getType() const;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add ann action to the Listener.
    /// An Action is a function which do something when an event of the correct
    /// type reach it. You can use it thanks to lambda functions.
    //////////////////////////////////////////////////////////////////////
    void addAction(EventType etype, std::function<void (const Event&)> eaction);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every functions in the Listener.
    /// @note The Listener continues to listen to objects it has been
    /// registered to.
    //////////////////////////////////////////////////////////////////////
    void resetActions();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls every methods registered in the Listener.
    /// When overwriting this method, if you still want to add actions, use
    /// ::onEvent(e) to call this class function which correctly calls every
    /// methods.
    //////////////////////////////////////////////////////////////////////
    virtual void onEvent (const Event& e);
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    ///
    /// The UpdateEvent can be emitted when Window objects themself, using
    /// ResourceManager::loop() or Window::update().
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent(const UpdateEvent& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Key is up.
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyUpEvent(const KeyUpEvent& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when a Key is down.
    //////////////////////////////////////////////////////////////////////
    virtual void onKeyDownEvent(const KeyDownEvent& e);
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a listener giving him a new name.
    /// This function creates a new Generic Listener and returns it.
    //////////////////////////////////////////////////////////////////////
    ResourceUser& addListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a listener to the emitter list, and returns a reference
    /// to the generic listener added.
    //////////////////////////////////////////////////////////////////////
    ResourceUser& addListener(const ResourceUser& listener);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a shared_pointer to the Generic Listener pointer
    ///by its name.
    //////////////////////////////////////////////////////////////////////
    ResourceUser getListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Remove the given Listener from the list.
    //////////////////////////////////////////////////////////////////////
    void removeListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Send an event to every listeners.
    //////////////////////////////////////////////////////////////////////
    virtual void sendEvent(const Event& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets this property to true if the Resource object should
    /// transmit its received Event to its listeners.
    //////////////////////////////////////////////////////////////////////
    void setShouldTransmitEvents(bool p);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Resource object should transmit its
    /// received Event to its listeners.
    //////////////////////////////////////////////////////////////////////
    bool shouldTransmitEvents() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of Reference this object count.
    //////////////////////////////////////////////////////////////////////
    int getReferenceCount() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Increase the Reference Counter.
    //////////////////////////////////////////////////////////////////////
    void addHolder() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Decrease the Reference Counter by one.
    ///
    /// If the Reference Counter decrease to zero, this function performs
    /// a suicide process. The object will be destroyed.
    //////////////////////////////////////////////////////////////////////
    void subHolder() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set the _IsLocked flag to true.
    ///
    /// If the object is already used by another thread, this function waits
    /// that the Resource is unlocked.
    //////////////////////////////////////////////////////////////////////
    bool lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Set the _IsLocked flag to false.
    //////////////////////////////////////////////////////////////////////
    void unlock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Resource is already in use.
    //////////////////////////////////////////////////////////////////////
    bool isLocked() const;
    
protected:
    
    /// @brief Returns the Data structure, if one.
    /// @warning This function is not safe at all. You should use the functions
    /// in ResourceUser to use your Resource. Nothing is guaranteed with this function,
    /// except for TextResource which is a special basic Resource (this function so return
    /// a std::string containing the whole file in text).
    virtual const void* _getData() const;
    
    /// @brief Defines a descriptor for a Variant entry in the resource table.
    typedef struct
    {
        Variant variant;
    } VariantDescriptor;
    
private: // General
    
    /// @brief The name of the Resource.
    std::string _name;
    
    /// @brief Holds the type of this Resource.
    Type _type;
    
private: // Event
    
    /// @brief A dictionnary containing Variant data.
    std::map<int, VariantDescriptor> _mVariantDatas;
    
    /// @brief Actions related to the Listening part from the Resource.
    std::map<EventType, std::vector<std::function<void (const Event&)> > > _actions;
    
    /// @brief Listeners to which we must send Events.
    std::map<std::string, ResourceUser> _mListeners;
    
    /// @brief Sets this property to true if the Resource object should transmit its received Event
    /// to its listeners.
    bool _shouldTransmit;
    
private: // Mutable Lock/Unlock system.
    
    /// @brief The Reference Counter.
    mutable std::atomic<int> _mCounter;
    
    /// @brief Holds true if this Resource is already locked.
    mutable std::atomic<bool> _mIsLocked;
    
    /// @brief Holds the Resource's mutex.
    mutable std::recursive_mutex _mMutex;
};

#ifdef GreUndefined

////////////////////////////////////////////////////////////////////////
/// @class ResourceUser
/// @brief A proxy to use a Resource object correctly.
///
/// A ResourceUser is an object that can be used as a Proxy to the Resource
/// object. The ResourceUser object does not own the Resource. Therefore, you
/// should always check with ResourceUser::expired() that the Resource is still
/// available.
///
/// @note The ResourceManager can, currently, destroys the Resource object at
/// any time but it will always affect the ResourceUser object.
///
/// The ResourceUser object is typically specific to the Type of the
/// Resource object. Then, it exists :
/// - A Plugin user object to use PluginResource objects.
/// - A Window user object to use WindowResource objects.
/// - A Renderer user object to use RendererPrivate objects.
/// - ...
///
/// The ResourceUser object can be copied, transfered, used by every one without
/// care of the owning or the destruction of the Resource. The Resource object
/// is always owned by the ResourceManager.
///
/// @see Resource, ResourceLoader
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceUser
{
public:
    
    /// @brief Default constructor.
    ResourceUser (bool persistent = false);
    /// @brief Moves the ResourceUser from another one.
    ResourceUser (ResourceUser&& movref);
    /// @brief Copies the ResourceUser from another one.
    ResourceUser (const ResourceUser& rhs);
    /// @brief Constructs the ResourceUser from a weak pointer.
    ResourceUser (std::weak_ptr<Resource> r, bool persistent = false);
    
    /// @brief Destroys the ResourceUser.
    virtual ~ResourceUser ();
    
    ResourceUser& operator = (const ResourceUser& rhs)
    {
        _resource = rhs._resource;
        _isPersistent = rhs._isPersistent;
        if(isPersistent())
        {
            _persistentResource = _resource.lock();
            _resource = _persistentResource;
        }
        return *this;
    }
    
    /// @brief Returns true if pointer has been invalidated.
    /// @note This usually means that the Resource has been destroyed.
    bool expired () const;
    
    /// @brief Returns the name of the Resource.
    const std::string& getName() const;
    
    /// @brief Returns the Data structure, if one.
    /// @warning This function is not safe at all. You should use the functions
    /// in ResourceUser to use your Resource. Nothing is guaranteed with this function,
    /// except for TextResource which is a special basic Resource (this function so return
    /// a std::string containing the whole file in text).
    template<typename T>
    const T getData() const { auto ptr = _resource.lock(); return ptr->getData<T>(); }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns expected data for user.
    //////////////////////////////////////////////////////////////////////
    const void* getCustomData(const std::string& dataname) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns expected data directly templated.
    //////////////////////////////////////////////////////////////////////
    template<typename DataType>
    const DataType getCustomDataTemplated(const std::string& name)
    {
        return reinterpret_cast<const DataType>(getCustomData(name));
    }
    
    /// @brief Lock the pointer in order to use it.
    /// This function guarantees the pointer is not destroyed while using it.
    std::shared_ptr<Resource> lock();
    /// @brief Lock the pointer in order to use it.
    /// This function guarantees the pointer is not destroyed while using it.
    const std::shared_ptr<Resource> lock() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Stores Variant data at given index.
    //////////////////////////////////////////////////////////////////////
    void storeVariantData(int index, const Variant& data);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Variant data at given index.
    /// @note Index inferior to 0 are reserved.
    //////////////////////////////////////////////////////////////////////
    Variant& getVariantData(int index);
    const Variant& getVariantData(int index) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true or false depending if the Resource is a
    /// ReferencedResource or just a Resource.
    /// @see Resource
    //////////////////////////////////////////////////////////////////////
    bool isReferenced() const;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add ann action to the Listener.
    //////////////////////////////////////////////////////////////////////
    void addAction(EventType etype, std::function<void (const Event&)> eaction);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every functions in the Listener.
    //////////////////////////////////////////////////////////////////////
    void resetActions();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls every methods registered in the Listener.
    //////////////////////////////////////////////////////////////////////
    void onEvent (const Event& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Make the given emitter to be listened by this listener.
    //////////////////////////////////////////////////////////////////////
    virtual void listen(ResourceUser& emitter);
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a listener giving him a new name.
    /// This function creates a new Generic Listener and returns it.
    //////////////////////////////////////////////////////////////////////
    ResourceUser& addListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a listener to the emitter list, and returns a reference
    /// to the generic listener added.
    //////////////////////////////////////////////////////////////////////
    ResourceUser& addListener(const ResourceUser& listener);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a shared_pointer to the Generic Listener pointer
    ///by its name.
    //////////////////////////////////////////////////////////////////////
    ResourceUser getListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Remove the given Listener from the list.
    //////////////////////////////////////////////////////////////////////
    void removeListener(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Send an event to every listeners.
    //////////////////////////////////////////////////////////////////////
    void sendEvent(const Event& e);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets this property to true if the Resource object should
    /// transmit its received Event to its listeners.
    //////////////////////////////////////////////////////////////////////
    void setShouldTransmitEvents(bool p);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Resource object should transmit its
    /// received Event to its listeners.
    //////////////////////////////////////////////////////////////////////
    bool shouldTransmitEvents() const;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if this Resource holds a shared_ptr.
    //////////////////////////////////////////////////////////////////////
    bool isPersistent() const;

    static ResourceUser Null; ///< @brief A Generic null ResourceUser.
    
protected:
    
    /// @brief The pointer to the Resource object.
    std::weak_ptr<Resource> _resource;
    
    /// @brief Used in case the Object depends from ReferencedResource.
    std::weak_ptr<ReferencedResource> _RefResource;
    
    /// @brief If the Resource Object is initialized with property
    /// persistent, it will holds a shared_ptr object.
    std::shared_ptr<Resource> _persistentResource;
    
    /// @brief During initialization, set this property to true if
    /// you want the ResourceUser object to hold a copy of the shared_ptr
    /// object. This is generally set for Resource that are not loaded
    /// by the ResourceManager.
    bool _isPersistent;
};

#endif // if 0

////////////////////////////////////////////////////////////////////////
/// @class ResourceLoader
/// @brief Used to load a Resource from something.
///
/// The ResourceLoader is an atypic object, because this class defines the
/// basic interface but not the main loading function.
///
/// The main loading function is of the form :
/// Resource* load (Resource::Type type, const std::string& name, ...) const
/// where '...' is a list of other argues that must be known from the user.
///
/// The ResourceLoader object must not save data, because of the factory
/// system.
/// At each loading, the factory constructs a new loader. This loader is
/// used by the ResourceManager, then is destroyed by the ResourceManager.
///
/// @see Resource, ResourceUser
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ResourceLoader : public Clonable<ResourceLoader>
{
public:
    
    POOLED(Pools::Loader)
    
    /// @brief Constructs the object.
    ResourceLoader();
    
    /// @brief Copies the object.
    ResourceLoader(const ResourceLoader&);
    
    /// @brief Destroys the object.
    virtual ~ResourceLoader();
    
    /// @brief Returns true if given Resource::Type is supported.
    virtual bool isTypeSupported (Resource::Type type) const;
    
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    virtual ResourceLoader* clone() const;
};

////////////////////////////////////////////////////////////////////////
/// @class ResourceLoaderFactory
/// @brief Constructs a Loader given its name.
///
/// In fact, this class just use the ResourceFactory::clone() function to
/// clone th registered factory for the user to use it.
///
/// # The ResourceFactory process
///
/// 2 steps :
/// - A plugin or the user creates a new Factory class. This Factory should
/// inherit from one of the typical factory (Window factory, Renderer factory,
/// ..., see Resource::Type for all factories types).
/// - The plugin 'registers' the Factory class using the ResourceLoaderFactory::registers()
/// function, giving the Factory name, and a new Factory object.
///
/// - The Factory user can use the registered Factory in using the ResourceLoaderFactory::get()
/// function.
/// - The user is responsible of the deletion of the factory, but you have to keep
/// in mind that ResourceManager::load() destroys the Factory object.
///
/// @see ResourceLoader
////////////////////////////////////////////////////////////////////////
template <typename T>
class ResourceLoaderFactory
{
public:
    
    POOLED(Pools::Factory)
    
    ResourceLoaderFactory () { }
    virtual ~ResourceLoaderFactory () { }
    
    void registers(const std::string& name, T* loader)
    { _loaders[name].reset(loader); }
    
    T* get(const std::string& name)
    {
        auto it = _loaders.find(name);
        if(it != _loaders.end())
            return reinterpret_cast<T*>(_loaders[name]->clone());
        else
            return nullptr;
    }
    
    T* getFirst ()
    {
        if(!_loaders.empty())
            return reinterpret_cast<T*>(_loaders.begin()->second->clone());
        return nullptr;
    }
    
    void clear()
    { _loaders.clear(); }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a list with the name of every Loader.
    ////////////////////////////////////////////////////////////////////////
    StringList getLoadersName() const
    {
        StringList ret;
        for(auto it = _loaders.begin(); it != _loaders.end(); it++) {
            ret.push_back(it->first);
        }
        
        return ret;
    }
    
protected:
    
    std::map<std::string, std::shared_ptr<T> > _loaders;
};

/// @brief Use this Exception to notifiate that the wanted Loader
/// has not been found in the Loader Factory.
typedef GreExceptionWithText GreNoLoaderFound;

#ifdef GreExtraMacros

#define GreResourceUserMakeConstructors( ClassName ) \
    ClassName (); \
    ClassName (const ClassName & rhs); \
    ClassName ( ClassName && rhs); \
    explicit ClassName (const ResourceUser& rhs); \
    ClassName & operator = (const ClassName & rhs); \
    bool operator == (const ClassName & rhs) const; \
    bool operator != (const ClassName & rhs) const; \
    virtual ~ClassName()

#define GreResourceUserMakeConstructorsPooled( ClassName , PoolName ) \
    POOLED ( Pools:: PoolName ) \
    GreResourceUserMakeConstructors( ClassName );

#endif

GreEndNamespace

#endif*/
