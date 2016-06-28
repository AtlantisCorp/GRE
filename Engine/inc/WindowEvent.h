//
//  WindowEvents.h
//  GRE
//
//  Created by Jacques Tronconi on 06/04/2016.
//
//

#ifndef GRE_WindowEvents_h
#define GRE_WindowEvents_h

#include "Event.h"
#include "Window.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Base class for Window events.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC WindowEvent : public Event
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    WindowEvent(const Window& emitter);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    WindowEvent(const WindowEvent& event);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~WindowEvent() noexcept(false);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Emitter.
    //////////////////////////////////////////////////////////////////////
    const Window& getEmitter() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Emitter.
    //////////////////////////////////////////////////////////////////////
    Window& getEmitter();
    
private:
    
    /// @brief Holds a ResourceUser to the emitter.
    Window iEmitter;
};

//////////////////////////////////////////////////////////////////////
/// @brief An Event generally sent when the Window Size changed.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC WindowSizedEvent : public WindowEvent
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    WindowSizedEvent(const Window& emitter, const Surface& newsize);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    WindowSizedEvent(const WindowSizedEvent& event);
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~WindowSizedEvent();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the new Surface.
    //////////////////////////////////////////////////////////////////////
    const Surface& getSurface() const;
    
private:
    
    /// @brief Holds the new Surface for the Window.
    Surface iSurface;
};

GreEndNamespace

#endif
