//////////////////////////////////////////////////////////////////////
//
//  Application.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 08/10/2016.
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

#ifndef Application_h
#define Application_h

#include "Pools.h"
#include "Resource.h"
#include "ResourceLoader.h"

#include "Window.h"
#include "Renderer.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Flags used to see if the Application should stop.
//////////////////////////////////////////////////////////////////////
enum class ApplicationCloseBehaviour
{
    AllWindowClosed ,
    TerminateCalled ,
    EscapeKey ,

    Invalid
};

/// @brief Bitset fields for ApplicationCloseBehaviour.
typedef std::bitset < (size_t) ApplicationCloseBehaviour::Invalid > ApplicationCloseBehaviours ;

//////////////////////////////////////////////////////////////////////
/// @brief An Application Object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Application : public Resource
{
public:

    POOLED(Pools::Resource)

    /// @brief Holder for Application.
    typedef Holder < Application > ApplicationHolder ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Application ( const std::string& name , const std::string& author = std::string() , const std::string& description = std::string() ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Application () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Basically runs the Application.
    /// By default , this functions send an update event to every Window
    /// objects in the Window Manager. But , platform-dependtly , this should
    /// be avoided in order to use platform-dependent code . ( like NSApplication
    /// on macOS )
    //////////////////////////////////////////////////////////////////////
    virtual void run () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Spawns a 'Worker Thread' to updates the given event proceeder.
    //////////////////////////////////////////////////////////////////////
    virtual void addWorkerThread ( Holder < EventProceeder > eventproceeder ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds the given EventProceeder (generally a Manager) to update
    /// in the Main Thread.
    //////////////////////////////////////////////////////////////////////
    virtual void addMainThread ( EventProceederHolder holder ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Should terminate the run loop.
    //////////////////////////////////////////////////////////////////////
    virtual void terminate () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a CloseBehaviour to the current behaviours.
    //////////////////////////////////////////////////////////////////////
    virtual void addCloseBehaviour ( const ApplicationCloseBehaviour & behaviour ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iShouldTerminate'.
    //////////////////////////////////////////////////////////////////////
    bool shouldTerminate () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Initializes the Application using the given command line
    /// arguments.
    //////////////////////////////////////////////////////////////////////
    virtual void initialize ( int argc , char ** argv ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Start the worker thread.
    //////////////////////////////////////////////////////////////////////
    virtual void startWorkerThread () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the elapsed time corresponds to the minimum 
    /// time for a frame. (This is use to limit Framerate for example)
    //////////////////////////////////////////////////////////////////////
    virtual bool isElapsedTime () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns an update event that can be given to any manager ,
    /// for each frame , with the delta time since the last frame.
    /// 'iMainStart' is updated to the current time.
    //////////////////////////////////////////////////////////////////////
    virtual EventHolder getNextUpdateEvent () ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Main function for every Worker Thread.
    //////////////////////////////////////////////////////////////////////
    static void WorkerThreadMain ( Application * app ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Main function for the Main Thread.
    //////////////////////////////////////////////////////////////////////
    virtual void iMainThreadLoop () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Function called by 'terminate()' , AllWindowClosedListener
    /// and EscapeKeyListener to notifiate terminate request .
    //////////////////////////////////////////////////////////////////////
    void iTerminatePrivate ( ApplicationCloseBehaviour why ) ;

protected:

    // Basic Listener to wait for every Window to be closed .

    class AllWindowClosedListener : public EventProceeder
    {
    public:

        POOLED ( Pools::Referenced ) ;

        AllWindowClosedListener ( Application* app ) ;
        ~AllWindowClosedListener ( ) noexcept ( false ) ;

    protected:

        // As soon as we get this Event , call the application 'terminate' function .
        void onLastWindowClosed ( const LastWindowClosedEvent & e ) ;

    protected:

        Application* appuser ;
    };

    typedef Holder < AllWindowClosedListener > AllWindowClosedListenerHolder ;

    // Basic Listener to wait for an escape key to be sent .

    class EscapeKeyListener : public EventProceeder
    {
    public:

        POOLED ( Pools::Referenced ) ;

        EscapeKeyListener ( Application* appli ) ;
        ~EscapeKeyListener ( ) noexcept ( false ) ;

    protected:

        void onKeyDownEvent ( const KeyDownEvent & e ) ;

    protected:

        Application * app ;
    };

    typedef Holder < EscapeKeyListener > EscapeKeyListenerHolder ;

protected:

    /// @brief Author string.
    std::string iAuthor ;

    /// @brief Description string.
    std::string iDescription ;

    /// @brief Flag to indicate the loop must stop.
    bool iShouldTerminate ;

    /// @brief bitset to use ApplicationCloseBehaviour as flags.
    std::bitset < (size_t) ApplicationCloseBehaviour::Invalid > iCloseBehaviours ;

    /// @brief List of Worker Threads executed by this Application.
    std::vector < EventProceederHolder > iWorkers ;

    /// @brief Workers' thread.
    std::thread iWorkerThread ;

    /// @brief List of Event Proceeder to update in Main Thread.
    std::vector < EventProceederHolder > iMainProceeders ;

    /// @brief Main Thread start time point.
    TimePoint iMainStart ;

    /// @brief Listens to AllWindowClosed behaviour .
    AllWindowClosedListenerHolder iAllWindowClosedListener ;

    /// @brief Listens to Escape Key from KeyboardManager.
    EscapeKeyListenerHolder iEscapeListener ;

    /// @brief Application::run() should be called only once in the program. This
    /// boolean records wheither Application::run() has been already called.
    bool iRunAlreadyCalled ;

    /// @brief Once launched, this variable holds the current WindowManager.
    WindowManagerHolder iWindowManager ;

    /// @brief Once launched, this variable holds the current RendererManager.
    RendererManagerHolder iRendererManager ;
    
    /// @brief Maximum framerate desired by the user. Default is '1 / 120.0f' (120 fps).
    float iMaxFramerate ;
};

/// @brief Holder for Application.
typedef Holder < Application > ApplicationHolder ;

/// @brief SpecializedResourceHolderList for Application.
typedef SpecializedResourceHolderList < Application > ApplicationHolderList ;

//////////////////////////////////////////////////////////////////////
/// @brief ResourceLoader for ApplicationPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ApplicationLoader : public ResourceLoader
{
public:

    POOLED(Pools::Loader)

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ApplicationLoader() ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ApplicationLoader() noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads an Application an returns its Holder.
    //////////////////////////////////////////////////////////////////////
    virtual ApplicationHolder load ( const std::string& name , const std::string& author , const std::string& desc ) const = 0 ;
};

/// @brief ResourceLoaderFactory for ApplicationLoader.
typedef ResourceLoaderFactory < ApplicationLoader > ApplicationLoaderFactory ;

GreEndNamespace

#endif /* Application_h */
