//////////////////////////////////////////////////////////////////////
//
//  Image.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 02/12/2015.
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

#ifndef GRE_Image_h
#define GRE_Image_h

/*
 
 (05/07/2016) NOTES : No use of this class has been found. Reason is we already have the Texture class. The
 TextureLoader can be more efficient to load images objects directly to SoftwarePixelBuffer objects. Then the 
 Texture object can freely make operations on this SoftwarePixelBuffer before to upload it to the GPU. So 
 Image objects are no longer used in the Engine.
 To write a Texture to a file, you should use TextureWriter classes.
 
#include "Pools.h"
#include "Resource.h"
#include "Color.h"
#include "FileLoader.h"
#include "Pixel.h"

GreBeginNamespace



struct Dimension2D
{
    unsigned width;
    unsigned height;
};

/// @brief Defines an Image.
/// This class should be subclassed by a plugin, in order
/// to use, either a library or a OS-specific implementation.
class DLL_PUBLIC ImageResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    ImageResource(const std::string& name);
    virtual ~ImageResource();
    
    /// @brief Returns true if empty, or invalid.
    virtual bool isEmpty() const;
    
    /// @brief Returns the width of the image, in pixels.
    virtual unsigned getWidth() const;
    /// @brief Returns the height of the image, in pixels.
    virtual unsigned getHeight() const;
    
    /// @brief Returns the pixel batch.
    virtual PixelBatch& getPixelBatch();
    virtual const PixelBatch& getPixelBatch() const;
    
    /// @brief Destroys the created pixel batch.
    virtual void unloadCache();
    
protected:
    
    PixelBatch _pixelBatch; ///< @brief Pixel batch of the Image.
    Dimension2D _mDimensions;      ///< @brief Size of the Image.
};

class DLL_PUBLIC Image : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    Image();
    Image(Image&& rhs);
    Image(const Image& rhs);
    explicit Image(const ResourceUser& rhs);
    
    ~Image();
    
    Image& operator = (const Image& rhs);
    Image& operator = (const ResourceUser& rhs);
    
    bool isEmpty() const;
    
    unsigned getWidth() const;
    unsigned getHeight() const;
    
    PixelBatch& getPixelBatch();
    const PixelBatch& getPixelBatch() const;
    
    /// @brief Destroys the created pixel batch.
    void unloadCache();
    
    static Image Null;
    
protected:
    
    std::weak_ptr<ImageResource> _mImage;
};

class DLL_PUBLIC ImageLoader : public FileLoader
{
public:
    
    ImageLoader();
    virtual ~ImageLoader();
    
    virtual bool isTypeSupported(Resource::Type type) const;
    virtual bool isExtensionSupported(const std::string& extension) const;
};

class DLL_PUBLIC ImageLoaderFactory : public ResourceLoaderFactory<ImageLoader>
{
public:
    
    POOLED(Pools::Factory)
    
    ImageLoaderFactory();
    ~ImageLoaderFactory();
    
    /// @brief Returns the first loader which support extension of given file.
    /// @param name : Can be a file name or just the extension. The extension must
    /// always begin with a '.' .
    ImageLoader* findBestLoader(const std::string& name) const;
};

GreEndNamespace

*/
#endif

