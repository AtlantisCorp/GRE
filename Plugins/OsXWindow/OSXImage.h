//
//  OSXImage.h
//  GRE
//
//  Created by Jacques Tronconi on 10/12/2015.
//
//
/*
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
        _pixelBatch.height          = 0;
        _pixelBatch.width           = 0;
        _pixelBatch.pixels          = nullptr;
        _pixelBatch.pixelsAlignment = 0;
        _pixelBatch.rowLenght       = 0;
        _pixelBatch.samplesPerPixel = 0;
        _pixelBatch.pixelFormat     = (PixelFormat) 0;
        
        _isCached   = false;
        _nsimageobj = nullptr;
        
        cache();
    }
    
    ~OSXImage()
    {
        if(_isCached) {
            unloadCache();
        }
    }
    
    bool isEmpty() const
    {
        return _pixelBatch.pixels == nullptr;
    }
    
    unsigned getWidth() const
    {
        return _pixelBatch.width;
    }
    
    unsigned getHeight() const
    {
        return _pixelBatch.height;
    }
    
    PixelBatch& getPixelBatch()
    {
        if(!_isCached)
            cache();
        
        return _pixelBatch;
    }
    
    const PixelBatch& getPixelBatch() const
    {
        return _pixelBatch;
    }
    
    void cache()
    {
        if(!_isCached)
        {
            GImageLoad(&_nsimageobj, _filename.c_str());
            {
                Pixels pixeldata            = GImageGetData(&_nsimageobj);
                _pixelBatch.height          = (unsigned) GImageGetHeight(&_nsimageobj);
                _pixelBatch.width           = (unsigned) GImageGetWidth(&_nsimageobj);
                _pixelBatch.pixels          = pixeldata;
                _pixelBatch.pixelsAlignment = 1;
                _pixelBatch.rowLenght       = _pixelBatch.width;
                _pixelBatch.samplesPerPixel = GImageGetSamples(&_nsimageobj);
                _pixelBatch.pixelFormat     = _pixelBatch.samplesPerPixel == 3 ? PixelFormat::R8G8B8 : PixelFormat::R8G8B8A8;
            }
            GImageUnload(&_nsimageobj);
            
            _isCached   = true;
            _nsimageobj = nullptr;
        }
    }
    
    void unloadCache()
    {
        if(_isCached)
        {
            ImageResource::unloadCache();

            _pixelBatch.height          = 0;
            _pixelBatch.width           = 0;
            _pixelBatch.pixels          = nullptr;
            _pixelBatch.pixelsAlignment = 0;
            _pixelBatch.rowLenght       = 0;
            _pixelBatch.samplesPerPixel = 0;
            _pixelBatch.pixelFormat     = (PixelFormat) 0;
            
            _isCached   = false;
            _nsimageobj = nullptr;
        }
    }
    
private:
    
    std::string _filename;
    CFTypeRef _nsimageobj;///< @brief Pointer to an object of type NSImage (== NSImage*).
    bool       _isCached;///< @brief Determine viability of the NSImage pointer.
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
*/