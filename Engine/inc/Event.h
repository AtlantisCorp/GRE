//
//  Event.h
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#ifndef GRE_Event_h
#define GRE_Event_h

#include "Version.h"
#include "Pools.h"
#include "Key.h"

GreBeginNamespace

/// @brief Determines an Event Type.
/// Also list every events available in the Engine.
enum class EventType
{
    KeyDown,
    KeyUp,
    Update,
    
    /// @brief A General Window event.
    Window,
    
    /// @brief Window has been resized.
    WindowSized
};

/// @brief Defines a basic event.
/// You should always check the type of this event before using
/// the ::to() function in order to use it correctly.
class Event
{
public:
    
    POOLED(Pools::Event)
    
    Event (const EventType& etype);
    virtual ~Event() noexcept(false);
    
    /// @brief Returns the event's type.
    const EventType& getType() const;
    
    /// @brief Returns the event but according to its real type.
    template<typename T>
    const T& to() const { return *reinterpret_cast<T*>(const_cast<Event*>(this)); }
    
protected:
    
    EventType _mType;
};

/// @brief An event sent when a key is pressed down.
/// @note This event does not say that the key has been pressed
/// down then released. It only say "the key has been pressed".
class KeyDownEvent : public Event
{
public:
    
    POOLED(Pools::Event)
    
    KeyDownEvent (const Key& pressedKey);
    virtual ~KeyDownEvent ();
    
    Key key;///< @brief The pressed down key.
};

/// @brief An event sent when a key is released up.
class KeyUpEvent : public Event
{
public:
    
    POOLED(Pools::Event)
    
    KeyUpEvent (const Key& releasedKey);
    virtual ~KeyUpEvent ();
    
    Key key;///< @brief The released key.
};

typedef std::chrono::high_resolution_clock::duration   UpdateClock;
typedef std::chrono::high_resolution_clock::time_point UpdateTime;
typedef std::chrono::high_resolution_clock             UpdateChrono;

/// @brief A generic update event that notify the listener it should be updated.
class UpdateEvent : public Event
{
public:
    
    POOLED(Pools::Event)
    
    UpdateEvent();
    ~UpdateEvent();
    
    UpdateClock elapsedTime;
};

GreEndNamespace
#endif
