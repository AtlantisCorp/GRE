//
//  Renderer.h
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef GResource_Renderer_h
#define GResource_Renderer_h

#include "Resource.h"
#include "Window.h"
#include "Color.h"
#include "HardwareVertexBuffer.h"
#include "HardwareIndexBuffer.h"
#include "Mesh.h"
#include "Camera.h"

GRE_BEGIN_NAMESPACE

typedef std::chrono::milliseconds ElapsedTime;

class DLL_PUBLIC RendererResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    RendererResource (const std::string& name);
    
    virtual ~RendererResource () { }
    
    virtual void associateWindow (Window& window) { _window = window; }
    
    virtual void render();
    
protected:
    
    virtual void _preRender();
    virtual void _render();
    virtual void _postRender();
    
public:
    
    virtual void renderExample () { }
    
    void setFramerate(float fps) { _wantedFps = fps; }
    
    float getCurrentFramerate() const { return _currentFps; }
    
    ElapsedTime getElapsedTime() const;
    void resetElapsedTime();
    
    virtual void beginRender();
    virtual void endRender();
    
    virtual void setClearColor(const Color& color) { }
    virtual void setClearDepth(float depth) { }
    
    bool isActive() const;
    void setActive(bool active);
    
    bool isImmediate() const;
    void setImmediateMode(bool mode);
    
    void addImmediateAction(std::function<void(void)> action);
    void resetImmediateActions();
    
public:
    
    virtual HardwareVertexBuffer createVertexBuffer();
    virtual HardwareIndexBuffer createIndexBuffer(PrimitiveType ptype, StorageType stype);
    virtual Mesh createMeshFromBuffers(const std::string& name, const HardwareVertexBuffer& vbuf, const HardwareIndexBufferBatch& ibufs);
    virtual Texture createTexture(const std::string& name, const std::string& file);
    virtual Camera createCamera(const std::string& name);
    
protected:
    
    virtual void _renderImmediate();
    
public:
    
    virtual void translate(float x, float y, float z) { }
    virtual void rotate(float angle, float x, float y, float z) { }
    
    virtual void drawTriangle(float sz, const Color& color1, const Color& color2, const Color& color3) { }
    virtual void drawQuad(float sz, const Color& color1, const Color& color2, const Color& color3, const Color& color4) { }
    virtual void draw(const Mesh& mesh);
    virtual void prepare(const Camera& cam) { }
    
protected:
    
    Window _window;
    float  _wantedFps; ///< @brief Hold the desired framerate.
    float  _currentFps; ///< @brief Hold the current fps.
    bool   _mIsActive;
    bool   _mIsImmediateMode; ///< @brief True if renderer is in immediate mode.
    std::vector<std::function<void(void)> > _mImmediateFunctions; ///< @brief Vector of functions to be called in immediate mode.
    
private:
    
    int _sum_frames;
    float _mMaxFps;
    float _mMinFps;
    std::chrono::high_resolution_clock::time_point _mClockReset;
    std::chrono::high_resolution_clock::time_point _mClockAtPreviousFrameEnd;
    std::chrono::high_resolution_clock::duration   _mClockElapsedSinceReset;
};

class DLL_PUBLIC Renderer : public ResourceUser
{
public:
    
    Renderer (Renderer&& movref);
    Renderer (const Renderer& renderer);
    explicit Renderer (const ResourceUser& ruser);
    
    virtual ~Renderer();
    
    Renderer& operator = (const ResourceUser& ruser);
    Renderer& operator = (const Renderer& ruser);
    
    void render();
    void renderExample();
    
    void associateWindow (Window& window);
    
    void setFramerate (float fps);
    
    float getCurrentFramerate () const;
    ElapsedTime getElapsedTime() const;
    void resetElapsedTime();
    
    void beginRender();
    void endRender();
    
    void setClearColor(const Color& color);
    void setClearDepth(float depth);
    
    void setActive(bool active);
    bool isActive() const;
    
    bool isImmediate() const;
    void setImmediateMode(bool mode);
    
    void addImmediateAction(std::function<void(void)> action);
    void resetImmediateActions();
    
    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    
    void drawTriangle(float sz, const Color& color1 = Color::White, const Color& color2 = Color::White, const Color& color3 = Color::White);
    void drawQuad(float sz, const Color& color1 = Color::White, const Color& color2 = Color::White, const Color& color3 = Color::White, const Color& color4 = Color::White);
    void draw(const Mesh& mesh);
    void prepare(const Camera& cam);
    
    HardwareVertexBuffer createVertexBuffer();
    HardwareIndexBuffer createIndexBuffer(PrimitiveType ptype, StorageType stype);
    Mesh createMeshFromBuffers(const std::string& name, const HardwareVertexBuffer& vbuf, const HardwareIndexBufferBatch& ibufs);
    Texture createTexture(const std::string& name, const std::string& file);
    Camera createCamera(const std::string& name);
    
private:
    
    std::weak_ptr<RendererResource> _mRenderer;
};

class DLL_PUBLIC DLL_PUBLIC RendererLoader : public ResourceLoader
{
public:
    
    POOLED(Pools::Loader)
    
    RendererLoader();
    virtual ~RendererLoader();
    
    virtual bool isTypeSupported (Resource::Type type) const;
    virtual Resource* load (Resource::Type type, const std::string& name) const;
    virtual ResourceLoader* clone() const;
    
protected:
    
    
};

typedef ResourceLoaderFactory<RendererLoader> RendererLoaderFactory;

GRE_END_NAMESPACE
#endif
