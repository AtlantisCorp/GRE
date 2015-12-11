//
//  OSXImage.h
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//

#ifndef GRE_OSXImage_h
#define GRE_OSXImage_h

#include "OSXFunctions.h"

class DLL_PUBLIC OSXImage : public ImageResource
{
public:
    
    POOLED(Pools::Resource)
    
    OSXImage(const std::string& name, const std::string& filename)
    : ImageResource(name), _filename(filename)
    {
        GImageLoad(&_nsimageobj, filename.c_str());
        getPixelBatch();
    }
    
    ~OSXImage()
    {
        GImageUnload(&_nsimageobj);
    }
    
    bool isEmpty() const
    {
        return (bool) GImageIsEmpty(&_nsimageobj);
    }
    
    unsigned getWidth() const
    {
        return (unsigned) GImageGetWidth(&_nsimageobj);
    }
    
    unsigned getHeight() const
    {
        return (unsigned) GImageGetHeight(&_nsimageobj);
    }
    
    PixelBatch& getPixelBatch()
    {
        Pixels pixeldata = GImageGetData(&_nsimageobj);
        _pixelBatch.height = getHeight();
        _pixelBatch.width = getWidth();
        _pixelBatch.pixels = pixeldata;
        _pixelBatch.pixelsAlignment = 1;
        _pixelBatch.rowLenght = _pixelBatch.width;
        _pixelBatch.samplesPerPixel = GImageGetSamples(&_nsimageobj);
        _pixelBatch.pixelFormat = _pixelBatch.samplesPerPixel == 3 ? PixelFormat::R8G8B8 : PixelFormat::R8G8B8A8;
        return _pixelBatch;
    }
    
    const PixelBatch& getPixelBatch() const
    {
        return _pixelBatch;
    }
    
private:
    
    std::string _filename;
    CFTypeRef _nsimageobj;///< @brief Pointer to an object of type NSImage (== NSImage*) .
};

class OSXImageLoader : public ImageLoader
{
public:
    
    POOLED(Pools::Loader)
    
    OSXImageLoader() { }
    ~OSXImageLoader() { }
    
    bool isExtensionSupported(const std::string &extension) const
    {
        return true;
    }
    
    Resource* load (Resource::Type type, const std::string& name, const std::string& file) const
    {
        if(isTypeSupported(type) && isExtensionSupported(file))
        {
            return new OSXImage(name, file);
        }
        
        return nullptr;
    }
    
    ResourceLoader* clone() const
    {
        return new OSXImageLoader();
    }
};

#endif
