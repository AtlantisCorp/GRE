//
//  jpegloader.cpp
//  GRE
//
//  Created by Jacques Tronconi on 03/12/2015.
//
//

#include "Image.h"
#include "ResourceManager.h"

using namespace Gre;

class JpegLoader : public ImageLoader
{
public:
    
    POOLED(Pools::Loader)
    
private:
    
};

extern "C" DLL_PUBLIC void* GetPluginName (void)
{
    return (void*) "JPEG Image Loader";
}

extern "C" DLL_PUBLIC void StartPlugin (void)
{
    ResourceManager::Get().getImageLoaderFactory().registers("JpegLoader", new JpegLoader);
}

extern "C" DLL_PUBLIC void StopPlugin (void)
{
    
}