//
//  Image.h
//  GRE
//
//  Created by Jacques Tronconi on 02/12/2015.
//
//

#ifndef GRE_Image_h
#define GRE_Image_h

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

#endif
