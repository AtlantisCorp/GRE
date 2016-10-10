//////////////////////////////////////////////////////////////////////
//
//  HardwarePixel.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 05/07/2016.
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

#ifndef GRE_HardwarePixel_h
#define GRE_HardwarePixel_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Describes the format used for the Pixel data.
//////////////////////////////////////////////////////////////////////
enum class HardwarePixelFormat
{
    /// @brief Invalid HardwarePixelFormat value.
    Null = 0,
    
    /// @brief Every Pixel component has a Value between 0 and 255
    RGBAShort = 1,
    
    /// @brief Every Pixel component is sizeof ( int ).
    RGBAInt = 2,
    
    /// @brief Every Pixel component is sizeof ( float ) .
    RGBAFloat = 3,
    
    /// @brief Every registered HardwarePixelFormat not added by the Core
    /// Engine should have this type.
    Custom = 999
};

//////////////////////////////////////////////////////////////////////
/// @brief Descriptor for HardwarePixelFormat.
//////////////////////////////////////////////////////////////////////
struct HardwarePixelFormatDescriptor
{
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwarePixelFormatDescriptor();
    
    /// @brief The format this Descriptor is referred to.
    HardwarePixelFormat iFormat;
    
    /// @brief The size of one pixel.
    size_t iSize;
    
    /// @brief The name of this Descriptor.
    std::string iName;
};

/// @brief std::list for HardwarePixelFormatDescriptor.
typedef std::vector<HardwarePixelFormatDescriptor> HardwarePixelFormatDescriptorList;

//////////////////////////////////////////////////////////////////////
/// @brief Returns the HardwarePixelFormatDescriptor for each
/// HardwarePixelFormat.
///
/// This Manager is also able to register new pixel formats. It can helps
/// you handle unknown pixel formats. Access the global manager using
/// HardwarePixelFormatDescriptorManager::Instance.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwarePixelFormatDescriptorManager
{
public:
    
    POOLED(Pools::Default)
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwarePixelFormatDescriptorManager();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwarePixelFormatDescriptorManager();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Registers a new HardwarePixelFormatDescriptor.
    //////////////////////////////////////////////////////////////////////
    void registerDescriptor(const HardwarePixelFormatDescriptor& descriptor);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a Descriptor for given type.
    //////////////////////////////////////////////////////////////////////
    const HardwarePixelFormatDescriptor getDescriptor(const HardwarePixelFormat& format) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds a Descriptor with given name.
    //////////////////////////////////////////////////////////////////////
    const HardwarePixelFormatDescriptor getDescriptorByName(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finds every Descriptor matching given size.
    //////////////////////////////////////////////////////////////////////
    HardwarePixelFormatDescriptorList getDescriptorsBySize(size_t size) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every Descriptors.
    //////////////////////////////////////////////////////////////////////
    HardwarePixelFormatDescriptorList getDescriptors() const;
    
    /// @brief A Global HardwarePixelFormatDescriptorManager.
    static HardwarePixelFormatDescriptorManager Instance;
    
protected:
    
    /// @brief Holds the Descriptors.
    HardwarePixelFormatDescriptorList iDescriptors;
};

GreEndNamespace

#endif
