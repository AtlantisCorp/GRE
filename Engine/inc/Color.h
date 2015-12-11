//
//  Color.h
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#ifndef GRE_Color_h
#define GRE_Color_h

#include "Version.h"

GRE_BEGIN_NAMESPACE

enum class ColorFormat
{
    RGBA    = 0,
    ARGB    = 1,
    RGB     = 2
};

template <ColorFormat Format>
struct ColorFormatHelper
{
    static const unsigned Components = 4;
};

template <>
struct ColorFormatHelper<ColorFormat::RGB>
{
    static const unsigned Components = 3;
};

/// @brief Defines a generic Color class.
/// A Color object is defined primary by its type (float, int, etc), but also
/// by the format it uses.
template<typename Type, ColorFormat Format>
class GenColor
{
public:
    
    static const size_t Size = ColorFormatHelper<Format>::Components * sizeof(Type);
    
    GenColor()
    {
        
    }
    
    GenColor(Type* data)
    {
        _data[0] = data[0];
        _data[1] = data[1];
        _data[2] = data[2];
        if(Format < ColorFormat::RGB) {
            _data[3] = data[3];
        }
    }
    
    GenColor(Type channel1, Type channel2, Type channel3, Type channel4)
    {
        _data[0] = channel1;
        _data[1] = channel2;
        _data[2] = channel3;
        if(Format < ColorFormat::RGB)
            _data[3] = channel4;
    }
    
    GenColor(Type channel1, Type channel2, Type channel3)
    {
        _data[0] = channel1;
        _data[1] = channel2;
        _data[2] = channel3;
        if(Format < ColorFormat::RGB)
            _data[3] = 1;
    }
    
    GenColor(const GenColor& rhs)
    {
        _data[0] = rhs._data[0];
        _data[1] = rhs._data[1];
        _data[2] = rhs._data[2];
        if(Format < ColorFormat::RGB) {
            _data[3] = rhs._data[3];
        }
    }
    
    ~GenColor()
    {
        
    }
    
    ColorFormat getFormat() const
    {
        return Format;
    }
    
    Type* getData() { return _data; }
    const Type* getData() const { return _data; }
    
    Type getRed() const
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
            case ColorFormat::RGB:
                return _data[0];
            case ColorFormat::ARGB:
                return _data[1];
            default:
                return 0;
        }
    }
    
    Type getGreen() const
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
            case ColorFormat::RGB:
                return _data[1];
            case ColorFormat::ARGB:
                return _data[2];
            default:
                return 0;
        }
    }
    
    Type getBlue() const
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
            case ColorFormat::RGB:
                return _data[2];
            case ColorFormat::ARGB:
                return _data[3];
            default:
                return 0;
        }
    }
    
    Type getAlpha() const
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
                return _data[3];
            case ColorFormat::ARGB:
                return _data[0];
            default:
                return 0;
        }
    }
    
    void setRed(Type channel)
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
            case ColorFormat::RGB:
                _data[0] = channel;
                break;
            case ColorFormat::ARGB:
                _data[1] = channel;
                break;
        }
    }
    
    void setGreen(Type channel)
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
            case ColorFormat::RGB:
                _data[1] = channel;
                break;
            case ColorFormat::ARGB:
                _data[2] = channel;
                break;
        }
    }
    
    void setBlue(Type channel)
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
            case ColorFormat::RGB:
                _data[2] = channel;
                break;
            case ColorFormat::ARGB:
                _data[3] = channel;
                break;
        }
    }
    
    void setAlpha(Type channel)
    {
        switch (Format)
        {
            case ColorFormat::RGBA:
                _data[3] = channel;
                break;
            case ColorFormat::ARGB:
                _data[0] = channel;
                break;
            default:
                break;
        }
    }
    
    bool operator == (const GenColor& rhs) const
    {
        if(Format < ColorFormat::RGB)
            return _data[0] == rhs._data[0] && _data[1] == rhs._data[1] && _data[2] == rhs._data[2] && _data[3] == rhs._data[3];
        else
            return _data[0] == rhs._data[0] && _data[1] == rhs._data[1] && _data[2] == rhs._data[2];
    }
    
    bool operator != (const GenColor& rhs) const
    {
        return !(*this == rhs);
    }
    
private:
    
    Type _data [ColorFormatHelper<Format>::Components];
    
public:
    
    static GenColor<Type,Format> White;
    static GenColor<Type,Format> Black;
};

template<typename T, ColorFormat F>
GenColor<T,F> GenColor<T,F>::White = GenColor<T,F>(1, 1, 1, 1);
template<typename T, ColorFormat F>
GenColor<T,F> GenColor<T,F>::Black = GenColor<T,F>(0, 0, 0, 1);

typedef GenColor<float, ColorFormat::RGBA> Color;

GRE_END_NAMESPACE
#endif
