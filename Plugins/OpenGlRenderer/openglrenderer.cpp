//
//  openglrenderer.cpp
//  GResource
//
//  Created by Jacques Tronconi on 08/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "OpenGlIncludes.h"
#include "OpenGlVertexBuffer.h"
#include "OpenGlIndexBuffer.h"
#include "OpenGlTexture.h"
#include "OpenGlRenderer.h"

class OpenGlRendererLoader : public RendererLoader
{
public:
    
    OpenGlRendererLoader ()
    {
        
    }
    
    ResourceLoader* clone() const
    {
        return (ResourceLoader*) new OpenGlRendererLoader;
    }
    
    Resource* load (Resource::Type type, const std::string& name) const
    {
        return dynamic_cast<Resource*>(new OpenGlRenderer(name));
    }
};

extern "C" DLL_PUBLIC void* GetPluginName (void)
{
    return (void*) "OpenGl Renderer";
}

extern "C" DLL_PUBLIC void StartPlugin (void)
{
    ResourceManager::Get().getRendererLoaderFactory().registers("OpenGlLoader", new OpenGlRendererLoader);
}

extern "C" DLL_PUBLIC void StopPlugin (void)
{
    
}
