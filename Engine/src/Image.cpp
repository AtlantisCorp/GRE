//
//  Image.cpp
//  GRE
//
//  Created by Jacques Tronconi on 02/12/2015.
//
//

#include "Image.h"

GRE_BEGIN_NAMESPACE

ImageResource::ImageResource(const std::string& name)
: Resource(name), _pixelBatch()
{
    _mDimensions.width = 0;
    _mDimensions.height = 0;
}

ImageResource::~ImageResource()
{
    
}

bool ImageResource::isEmpty() const
{
    return _pixelBatch.isEmpty() ||
          (_mDimensions.height == 0 || _mDimensions.width == 0);
}

unsigned ImageResource::getWidth() const
{
    return _mDimensions.width;
}

unsigned ImageResource::getHeight() const
{
    return _mDimensions.height;
}

PixelBatch& ImageResource::getPixelBatch()
{
    return _pixelBatch;
}

const PixelBatch& ImageResource::getPixelBatch() const
{
    return _pixelBatch;
}

void ImageResource::unloadCache()
{
    if(_pixelBatch.pixels)
    {
        free(_pixelBatch.pixels);
    }
}

Image::Image()
: ResourceUser(), _mImage()
{
    
}

Image::Image(Image&& rhs)
: ResourceUser(rhs), _mImage(std::move(rhs._mImage))
{
    
}

Image::Image(const Image& rhs)
: ResourceUser(rhs), _mImage(rhs._mImage)
{
    
}

Image::Image(const ResourceUser& rhs)
: ResourceUser(rhs), _mImage(std::dynamic_pointer_cast<ImageResource>(rhs.lock()))
{
    
}

Image::~Image()
{
    
}

Image& Image::operator=(const ResourceUser &rhs)
{
    _mImage = std::dynamic_pointer_cast<ImageResource>(rhs.lock());
    return *this;
}

bool Image::isEmpty() const
{
    auto ptr = _mImage.lock();
    if(ptr)
        return ptr->isEmpty();
    return true;
}

unsigned Image::getWidth() const
{
    auto ptr = _mImage.lock();
    if(ptr)
        return ptr->getWidth();
    return 0;
}

unsigned Image::getHeight() const
{
    auto ptr = _mImage.lock();
    if(ptr)
        return ptr->getHeight();
    return 0;
}

PixelBatch& Image::getPixelBatch()
{
    auto ptr = _mImage.lock();
    if(ptr)
        return ptr->getPixelBatch();
    return PixelBatch::Empty;
}

const PixelBatch& Image::getPixelBatch() const
{
    auto ptr = _mImage.lock();
    if(ptr)
        return ptr->getPixelBatch();
    return PixelBatch::Empty;
}

void Image::unloadCache()
{
    auto ptr = _mImage.lock();
    if(ptr)
        return ptr->unloadCache();
}

ImageLoader::ImageLoader()
{
    
}

ImageLoader::~ImageLoader()
{
    
}

bool ImageLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::Image;
}

bool ImageLoader::isExtensionSupported(const std::string &extension) const
{
    return false;
}

ImageLoaderFactory::ImageLoaderFactory()
{
    
}

ImageLoaderFactory::~ImageLoaderFactory()
{
    
}

ImageLoader* ImageLoaderFactory::findBestLoader(const std::string &name) const
{
    for (auto loader : _loaders)
    {
        if(loader.second->isExtensionSupported(name))
            return (ImageLoader*) loader.second->clone();
    }
    
    return nullptr;
}

PixelBatch PixelBatch::Empty = PixelBatch();
Image Image::Null = Image();

GRE_END_NAMESPACE
