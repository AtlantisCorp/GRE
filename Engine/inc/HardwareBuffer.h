//
//  HardwareBuffer.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_HardwareBuffer_h
#define GRE_HardwareBuffer_h

#include "Pools.h"
#include "Face.h"
#include "Resource.h"

GRE_BEGIN_NAMESPACE

/// @brief Defines the real Hardware Buffer interface.
/// A Hardware Buffer is a collection of data stored generally by the
/// renderer, to render objects more quickly in giving the subjacing
/// API this raw buffer.
class DLL_PUBLIC HardwareBufferPrivate : public Resource
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    HardwareBufferPrivate ();
    virtual ~HardwareBufferPrivate ();
    
    /// @brief Bind the Hardware Buffer in order to use it.
    virtual void bind() const;
    
    /// @brief Unbind the Hardware Buffer after it has been used.
    virtual void unbind() const;
    
    /// @brief Update the buffer if dirty.
    virtual void update() const;
    
    /// @brief Returns true if Buffer is invalid.
    virtual bool isInvalid() const;
    
    /// @brief Returns the size of the buffer.
    virtual size_t getSize() const;
    
    /// @brief Returns true if this buffer needs to be updated.
    bool isDirty() const;
protected:
    /// @brief Used by the children classes, set this property to true
    /// if the buffer needs update.
    void setDirty(bool dirty) const;
    
protected:
    
    mutable bool _mIsDirty;
};

/// @brief Describes a proxy interface to use correctly the HardwareBufferPrivate
/// object.
/// This object has the same use than ResourceUser, but the HardwareBuffer object is
/// managed by the Renderer.
class DLL_PUBLIC HardwareBuffer : public ResourceUser
{
public:
    
    HardwareBuffer();
    virtual ~HardwareBuffer();
    
    /// @brief Bind the Hardware Buffer in order to use it.
    virtual void bind() const = 0;
    /// @brief Unbind the Hardware Buffer after it has been used.
    virtual void unbind() const = 0;
    /// @brief Returns the size of the Buffer.
    virtual size_t getSize() const = 0;
    /// @brief Returns true if Buffer is invalid.
    virtual bool isInvalid() const = 0;
    /// @brief Returns true if needs update.
    virtual bool isDirty() const = 0;
    /// @brief Returns true if object is invalid.
    virtual bool isExpired() const = 0;
};

GRE_END_NAMESPACE

#endif
