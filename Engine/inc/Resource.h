//////////////////////////////////////////////////////////////////////
//
//  Resource.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/05/2016.
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Registers a new EventCallback function that will be called
    /// when the next presenting event is one of given type.
    //////////////////////////////////////////////////////////////////////
    virtual void onNextEvent ( const EventType& etype , EventCallback callback );
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Called when receiving Update Event.
    /// [thread-safe]
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
    /// @brief Send an Event pointed object to every Listeners.
    /// If 'destroy' flag is true , the event is deleted after being sent
    /// to every Listeners. This is the default behaviour when launching a
    /// new event , but transmitting an event from the '::onEvent()' function
    /// should not destroy the event as there may be other objects that will
    /// use this event.
    //////////////////////////////////////////////////////////////////////
    virtual void sendEvent ( Event* e , bool destroy = true ) ;
    
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a custom property defined by given name.
    //////////////////////////////////////////////////////////////////////
    virtual const void* getProperty ( const std::string& name ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Locks the internal mutex to prevent multithreading.
    //////////////////////////////////////////////////////////////////////
    void lockGuard() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unlocks the internal mutex.
    //////////////////////////////////////////////////////////////////////
    void unlockGuard() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every listeners.
    //////////////////////////////////////////////////////////////////////
    void clearListeners() ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Reset the Resource.
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;
    
private:
    
    /// @brief Resource's Name.
    std::string iName;
    
    /// @brief Resource's Type.
    Type iType;
    
    /// @brief Actions related to the Listening part from the Resource.
    std::map<EventType, std::vector<std::function<void (const Event&)> > > iActions;
    
    /// @brief Callbacks to call on next event.
    std::map < EventType , std::vector < EventCallback > > iNextCallbacks;
    
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
    
protected:
    
    /// @brief A Mutex to prevent this Resource to conflict when multithreaded.
    mutable std::recursive_mutex iMutex;
};

//////////////////////////////////////////////////////////////////////
/// @brief Simple Autolocker Class.
//////////////////////////////////////////////////////////////////////
class ResourceAutolock
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ResourceAutolock ( std::recursive_mutex& mutex );
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~ResourceAutolock ();
    
private:
    
    /// @brief Reference to the Mutex we must lock and unlock.
    std::recursive_mutex* iMutexReference;
};

/// @brief Use this macro inside a Resource's subclass to generate an auto-mutex locker.
/// The ResourceAutolock objects lock the mutex until destruction of this object. Use the
/// brackets to make locked blocks.
#define GreResourceAutolock \
    Gre::ResourceAutolock __autolock ( iMutex ) 

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
template< typename BaseClass , typename LoaderBaseClass >
class DLL_PUBLIC SpecializedResourceManager : public Resource
{
public:
    
    POOLED(Pools::Manager)
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    SpecializedResourceManager( const std::string& name ) : Resource ( name ) { }
    
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    virtual ~SpecializedResourceManager() { }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Should add an object to the Manager , if this object is not
    /// already loaded.
    ////////////////////////////////////////////////////////////////////////
    virtual void load(const SpecializedResourceHolder<BaseClass>& holder)
    {
        if ( iHolders.find(holder) == iHolders.end() )
        {
            iHolders.add(holder);
        }
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
    virtual SpecializedResourceHolder<BaseClass> get(const std::string& name)
    {
        auto it = iHolders.find(name);
        if(it != iHolders.end())
            return *it;
        return SpecializedResourceHolder<BaseClass>(nullptr);
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Holder's list.
    //////////////////////////////////////////////////////////////////////
    SpecializedResourceHolderList<BaseClass>& getAll()
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
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears every Holders.
    //////////////////////////////////////////////////////////////////////
    void clear()
    {
        iHolders.clear();
    }
    
protected:
    
    /// @brief Holds SpecializedResourceHolder objects.
    SpecializedResourceHolderList < BaseClass > iHolders;
    
    /// @brief Holds the SpecializedLoaderFactory.
    ResourceLoaderFactory < LoaderBaseClass > iFactory;
};

GreEndNamespace

#    endif
#  endif
#endif

#endif
