//
//  OSXWindow.h
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#ifndef GRE_OSXWindow_h
#define GRE_OSXWindow_h

#include "OSXFunctions.h"

#define KEYBUF_MAX 256

struct keybuf_t
{
    int key;
    int pressed;
};

class DLL_PUBLIC OsXWindow : public WindowResource
{
public:
    
    OsXWindow (const std::string & name, const WindowPrivate& data);
    
    ~OsXWindow ();
    
    bool pollEvent();
    bool isClosed() const;
    const std::string recommendedRenderer() const;
    void associate (Renderer& renderer);
    void setTitle(const std::string& title);
    void swapBuffers ();
    WindowSize getWindowSize() const;
    void setVerticalSync (bool vsync);
    bool hasVerticalSync () const;
    bool isExposed () const;
    
private:
    
    bool _isclosed;
};

class DLL_PUBLIC OsXWindowLoader : public WindowLoader
{
public:
    
    OsXWindowLoader ();
    ~OsXWindowLoader ();
    
    ResourceLoader* clone() const;
    Resource* load (Resource::Type type, const std::string& name, int x0, int y0, int wid, int height) const;
};

#endif
