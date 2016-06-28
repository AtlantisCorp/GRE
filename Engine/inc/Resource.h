////////////////////////////////////////////////////
//  File    : Resource.h
//  Project : GRE
//
//  Created by Jacques Tronconi on 06/05/2016.
//  
//
////////////////////////////////////////////////////

#ifndef GRE_Resource_h
#define GRE_Resource_h

#include "Pools.h"
#include "Variant.h"
#include "Event.h"
#include "ReferenceCounter.h"

GreBeginNamespace

class ResourceHolder;
class ResourceUser;

////////////////////////////////////////////////////////////////////////
/// @class Resource
/// @brief A Referenced Object, that can be use to manage 'Resource' typed
/// objects in the Engine.
///
/// To resume, every objects that should be shared in the Engine should
/// subclass Resource.
///
/// The Resource Object contains a Reference Counter. This Counter is
/// initialized ONLY when a Resource Holder acquire for the first time
/// this object. When the Resource Holder release this Resource and the
/// Counter reach 0, the Resource Object commit suicide.
///
/// The Resource Holder is used to hold a pointer to the Resource. When the
/// Resource Holder is destroyed, it decrease the Counter in the Resource.
/// The Resource User is an object attached to a particular Resource Holder.
/// The Resource User use the Holder to get a valid pointer to the object.
/// The ResourceUser::lock() method creates a new ResourceHolder Object that
/// should be used only in scope.
///
/// The Resource Object is also an Event Sender/Receiver.
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Resource
{
public:
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Determines the Resource's Type.
    ////////////////////////////////////////////////////////////////////////
    enum class Type
    {
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
    
public:
    
    POOLED(Pools::Resource)
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Resource Object of type 'Null'.
    ////////////////////////////////////////////////////////////////////////
    Resource(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Resource() noexcept(false);
    
public:
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the Name of this Resource.
    /// @note This Name is not guaranteed to be unique.
    ////////////////////////////////////////////////////////////////////////
    const std::string& getName() const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the Type of this Resource.
    ////////////////////////////////////////////////////////////////////////
    const Type& getType() const;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Add an action to the Listener.
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
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Increase the Reference Counter by one.
    /// If the Counter wasn't initialized, sets iCounterInitialized to true.
    //////////////////////////////////////////////////////////////////////
    void acquire();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Decrease the Reference Counter by one.
    /// If the Counter reach 0, commit suicide.
    //////////////////////////////////////////////////////////////////////
    void release();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of ResourceHolder holding this Resource.
    /// @see ReferenceCounter::getHolderCount().
    //////////////////////////////////////////////////////////////////////
    int getCounterValue() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the ReferenceCounter Object, or null if uninitialized.
    //////////////////////////////////////////////////////////////////////
    ReferenceCounter* getReferenceCounter();
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Return the Variant for given Custom Data's entry.
    //////////////////////////////////////////////////////////////////////
    Variant& getCustomData(const std::string& entry);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Return the Variant for given Custom Data's entry.
    //////////////////////////////////////////////////////////////////////
    const Variant& getCustomData(const std::string& entry) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the Variant for given Custom Data's entry.
    //////////////////////////////////////////////////////////////////////
    void setCustomData(const std::string &entry, const Variant& data);
    
private:
    
    /// @brief Resource's Name.
    std::string iName;
    
    /// @brief Resource's Type.
    Type iType;
    
    /// @brief Actions related to the Listening part from the Resource.
    std::map<EventType, std::vector<std::function<void (const Event&)> > > iActions;
    
    /// @brief Listeners to which we must send Events.
    std::map<std::string, ResourceUser> iListeners;
    
    /// @brief Sets this property to true if the Resource object should transmit its received Event to its listeners.
    bool iShouldTransmit;
    
    /// @brief ReferenceCounter to manage Resource, ResourceUser and ResourceHolder's Objects.
    ReferenceCounter* iCounter;
    
    /// @brief True if Counter has been initialized. By default and before any call to ::acquire(), false.
    bool iCounterInitialized;
    
    /// @brief Holds Variant Data for custom use, or platform specific use.
    std::map<std::string, Variant> iCustomData;
    
};

GreEndNamespace

// This bad piece of code is here in order to prevent ResourceHolder , ResourceUser or ResourceLoader to
// includes one of those file in bad order. Those files should manually includes each one.
#ifndef GRE_ResourceHolder_h
#  ifndef GRE_ResourceLoader_h
#    ifndef GRE_ResourceUser_h

#      include "ResourceHolder.h"
#      include "ResourceUser.h"
#      include "ResourceLoader.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @brief A SpecializedResourceManager.
/// The BaseClass template should be the *Private Resource object, not the
/// user. The Holder derived class should follow the pattern :
///
/// ```
/// typedef SpecializedResourceHolder<MyResourcePrivate> MyResourcePrivateHolder;
/// typedef SpecializedResourceHolderList<MyResourcePrivate> MyResourcePrivateHolderList;
/// ```
////////////////////////////////////////////////////////////////////////
template<typename BaseClass>
class DLL_PUBLIC SpecializedResourceManager
{
public:
    
    POOLED(Pools::Manager)
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    SpecializedResourceManager() { }
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~SpecializedResourceManager() { }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Should add an object to the Manager.
    ////////////////////////////////////////////////////////////////////////
    virtual void add(const SpecializedResourceHolder<BaseClass>& holder)
    {
        iHolders.add(holder);
    }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Should remove a holder from the Manager.
    ////////////////////////////////////////////////////////////////////////
    virtual void remove(const std::string& name)
    {
        iHolders.remove(name);
    }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the given Resource is present.
    ////////////////////////////////////////////////////////////////////////
    virtual bool has(const std::string& name) const
    {
        return iHolders.find(name) != iHolders.end();
    }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a Resource corresponding to given name.
    ////////////////////////////////////////////////////////////////////////
    virtual SpecializedResourceHolder<BaseClass>& get(const std::string& name)
    {
        auto it = iHolders.find(name);
        if(it != iHolders.end())
            return *it;
        return SpecializedResourceHolder<BaseClass>(nullptr);
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Holder's list.
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolderList<BaseClass> getAll()
    {
        return iHolders;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Holder's list.
    //////////////////////////////////////////////////////////////////////
    const SpecializedResourceHolderList<BaseClass>& getAll() const
    {
        return iHolders;
    }
    
protected:
    
    /// @brief Holds SpecializedResourceHolder objects.
    SpecializedResourceHolderList<BaseClass> iHolders;
};

GreEndNamespace

#    endif
#  endif
#endif

#endif
