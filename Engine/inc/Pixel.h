//
//  Pixel.h
//  GRE
//
//  Created by Jacques Tronconi on 05/12/2015.
//
//

#ifndef GRE_Pixel_h
#define GRE_Pixel_h

#include "Pools.h"

GreBeginNamespace

/// @brief Resumes every Pixel Format supported by the Engine.
/// Every Pixel Format is templated. We cannot use a dynamic system, because time wasted to
/// cast everything would be too long. We need preprocessed system to ensure not much time is
/// wasted in converting, iterating in the pixel.
enum class PixelFormat
{
    A8R8G8B8, ///< @brief ARGB 32 bits (8-8-8-8)
    R8G8B8A8, ///< @brief RGBA 32 bits (8-8-8-8)
    R8G8B8    ///< @brief RGB 24 bits (8-8-8)
};

typedef unsigned char* Pixel;   ///< @brief Defines a Pixel generic type. The Pixel is stored in one or more unsigned char.
typedef void*          Channel; ///< @brief Defines a Channel generic type. The Channel real type is given by the PixelFormatHelper struct.
typedef unsigned char* Pixels;  ///< @brief Defines a Set of Pixels.

/// @brief Hold informations about the given Pixel Format.
template <PixelFormat Format>
struct PixelFormatHelper
{
    static const unsigned Channels = 0;            ///< @brief Defines the Number of Channels in the Pixel Format.
    static const unsigned Size = 0;                ///< @brief Defines the Size of the Pixel in bits.
    static const bool     IsHomogeneous = true;    ///< @brief Set to true if each Channel have the same size.
    static const bool     HaveChannelType = false; ///< @brief Set to true if each Channel can be hold in a whole type.
    static const unsigned Channel1Size = 0;        ///< @brief Applicable when ::IsHomogeneous is false, defines the Channel 1 size.
    static const unsigned Channel2Size = 0;        ///< @brief Applicable when ::IsHomogeneous is false, defines the Channel 1 size.
    static const unsigned Channel3Size = 0;        ///< @brief Applicable when ::IsHomogeneous is false, defines the Channel 1 size.
    static const unsigned Channel4Size = 0;        ///< @brief Applicable when ::IsHomogeneous is false, defines the Channel 1 size.
};

template <>
struct PixelFormatHelper<PixelFormat::A8R8G8B8>
{
    typedef uint32_t Type;
    static const unsigned Channels = 4;
    static const unsigned Size = 32;
    static const bool     IsHomogeneous = true;
    static const bool     HaveChannelType = true;
    typedef uint8_t ChannelType;
    static const unsigned Channel1Size = 8;
    static const unsigned Channel2Size = 8;
    static const unsigned Channel3Size = 8;
    static const unsigned Channel4Size = 8;
};

template <>
struct PixelFormatHelper<PixelFormat::R8G8B8A8>
{
    typedef uint32_t Type;
    static const unsigned Channels = 4;
    static const unsigned Size = 32;
    static const bool     IsHomogeneous = true;
    static const bool     HaveChannelType = true;
    typedef uint8_t ChannelType;
    static const unsigned Channel1Size = 8;
    static const unsigned Channel2Size = 8;
    static const unsigned Channel3Size = 8;
    static const unsigned Channel4Size = 8;
};

template <>
struct PixelFormatHelper<PixelFormat::R8G8B8>
{
    typedef uint32_t Type;
    static const unsigned Channels = 3;
    static const unsigned Size = 24;
    static const bool     IsHomogeneous = true;
    static const bool     HaveChannelType = true;
    typedef uint8_t ChannelType;
    static const unsigned Channel1Size = 8;
    static const unsigned Channel2Size = 8;
    static const unsigned Channel3Size = 8;
    static const unsigned Channel4Size = 8;
};

/// @brief An iterator which iterate through the Channels of a Pixel.
/// Its mainly purpose is to be able to copy one Pixel to another one of the same type,
/// without taking account its type.
template<PixelFormat Format>
class PixelChannelIterator
{
public:
    
    typedef PixelFormatHelper<Format> Helper;
    
    PixelChannelIterator(Pixel pixel)
    : _currentPixel(pixel), _currentChannel(0)
    {
        
    }
    
    ~PixelChannelIterator() { }
    
    const Pixel data() const
    {
        return _currentPixel;
    }
    
    /// @brief Set the pixel channels to given one.
    void set(const PixelChannelIterator<Format>& it)
    {
        const Pixel src = it._currentPixel;
        if(Helper::HaveChannelType)
        {
            _currentChannel = 0;
            typename Helper::ChannelType* pchannel = (typename Helper::ChannelType*) (_currentPixel);
            while(_currentChannel < Helper::Channels)
            {
                const typename Helper::ChannelType* pchannelsrc = ((const typename Helper::ChannelType*)src) + _currentChannel;
                *pchannel = *pchannelsrc;
                _currentChannel++;
                pchannel++;
            }
            _currentChannel = 0;
        }
        else
        {
            // If we have every channels, we can simply copy it.
            _currentChannel = 0;
            memcpy(_currentPixel, it.data(), Helper::Size/8);
        }
    }
    
    void set(Channel channel1, Channel channel2 = nullptr, Channel channel3 = nullptr, Channel channel4 = nullptr)
    {
        if(Helper::HaveChannelType)
        {
            _currentChannel = 0;
            typename Helper::ChannelType* _channel = (typename Helper::ChannelType*) _currentPixel;
            
            if(_currentChannel < Helper::Channels) {
                *_channel = * ((const typename Helper::ChannelType*) &channel1);
                _currentChannel++;
                _channel++;
            }
            if(_currentChannel < Helper::Channels) {
                *_channel = * ((const typename Helper::ChannelType*) &channel2);
                _currentChannel++;
                _channel++;
            }
            if(_currentChannel < Helper::Channels) {
                *_channel = * ((const typename Helper::ChannelType*) &channel3);
                _currentChannel++;
                _channel++;
            }
            if(_currentChannel < Helper::Channels) {
                *_channel = * ((const typename Helper::ChannelType*) &channel4);
                _currentChannel++;
                _channel++;
            }
        }
        
        else
        {
            throw GreUnsupportedOperation(std::string("No iteration supported on non-typed channels."));
        }
    }
    
    Channel channel()
    {
        if(Helper::HaveChannelType)
        {
            typename Helper::ChannelType* pchannel = ((typename Helper::ChannelType*) (_currentPixel)) + _currentChannel;
            Channel ret = *((Channel*) pchannel);
            if(_currentChannel >= Helper::Channels - 1)
                _currentChannel = 0;
            else
                _currentChannel ++;
            return ret;
        }
        
        else
        {
            throw GreUnsupportedOperation(std::string("No iteration supported on non-typed channels."));
        }
        
        // unreachable
        return nullptr;
    }
    
private:
    
    unsigned char* _currentPixel; ///< @brief Current Pixel.
    unsigned       _currentChannel; ///< @brief Current Channel in the Pixel.
    unsigned       _pixelCount; ///< @brief Number of Pixels in the batch to iterate.
};

/// @brief Convert a pixel form format1 to format2.
template <PixelFormat Format1, PixelFormat Format2>
inline void PixelConvert(Pixel pixel1, Pixel pixel2)
{
    throw GreUnsupportedOperation(std::string("[PixelConvert] Unsupported conversion from PixelFormat '") +
                                  typeid(Format1).name() + "' to PixelFormat '" + typeid(Format2).name() + "'.");
}

template<PixelFormat Format>
inline void PixelConvert(Pixel pixel1, Pixel pixel2)
{
    PixelChannelIterator<Format> it1(pixel1);
    PixelChannelIterator<Format> it2(pixel2);
    it2.set(it1);
}

template <>
inline void PixelConvert<PixelFormat::A8R8G8B8, PixelFormat::R8G8B8>(Pixel pixel1, Pixel pixel2)
{
    PixelChannelIterator<PixelFormat::A8R8G8B8> it1(pixel1);
    PixelChannelIterator<PixelFormat::R8G8B8> it2(pixel2);
    it1.channel();
    it2.set(it1.channel(), it1.channel(), it1.channel());
}

/// @brief Defines some basic caracteristics of a set of Pixels.
class PixelBatch
{
public:
    static PixelBatch Empty;
    
    int rowLenght;              ///< @brief Number of pixels in a row.
    int pixelsAlignment;        ///< @brief Specifies the alignment requirements for the start of each pixel row in memory.
    int samplesPerPixel;        ///< @brief The number of sample for each pixels.
    int width;                  ///< @brief Width of the Pixel Batch, in pixels.
    int height;                 ///< @brief Height of the Pixel Batch, in pixels.
    
    PixelFormat pixelFormat;    ///< @brief Format to store the pixels.
    Pixels      pixels;         ///< @brief Raw pixels data.
    
    PixelBatch() { }
    bool isEmpty() const { return pixels == nullptr; }
    
};

GreEndNamespace

#endif
