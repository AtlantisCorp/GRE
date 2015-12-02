//
//  HardwareIndexBuffer.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_HardwareIndexBuffer_h
#define GRE_HardwareIndexBuffer_h

#include "HardwareBuffer.h"
#include "Face.h"

GRE_BEGIN_NAMESPACE

/// @brief Defines a HardwareBuffer used to hold Indexes (faces).
class DLL_PUBLIC HardwareIndexBufferPrivate : public HardwareBufferPrivate
{
public:
    
    HardwareIndexBufferPrivate();
    HardwareIndexBufferPrivate(PrimitiveType ptype, StorageType stype = StorageType::UnsignedInt);
    virtual ~HardwareIndexBufferPrivate();
    
    /// @brief Add an index (a groupe of indice, or a face) to this buffer.
    void add(const IndexedFace& index);
    
    /// @brief Add a batch of face to this buffer.
    void add(const IndexedFaceBatch& batch);
    
    /// @brief Sets the material used to render this buffer.
    void setMaterial(const Material& material);
    
    /// @brief Returns the Material used.
    Material& getMaterial();
    /// @brief Returns the Material used.
    const Material& getMaterial() const;
    
    /// @brief Returns the Primitive type.
    PrimitiveType getPrimitiveType() const;
    /// @brief Returns the Storage Type.
    StorageType getStorageType() const;
    
    /// @brief Returns the number of elements.
    virtual size_t getElementCount() const;
    
protected:
    
    MaterialIndexedFace _mFaces;///< @brief The indexes, for one material.
    PrimitiveType       _mPrimitiveType;///< @brief Primitive Type to be drawn.
    StorageType         _mStorageType;///< @brief Storage used to store the indices.
};

/// @brief Proxy to the HardwareIndexBufferPrivate object.
class DLL_PUBLIC HardwareIndexBuffer : public HardwareBuffer
{
public:
    
    HardwareIndexBuffer();
    HardwareIndexBuffer(const HardwareIndexBuffer& buffer);
    HardwareIndexBuffer(HardwareIndexBuffer&& buffer);
    explicit HardwareIndexBuffer(std::weak_ptr<HardwareIndexBufferPrivate> bufferptr);
    explicit HardwareIndexBuffer(const ResourceUser& ruser);
    
    ~HardwareIndexBuffer();
    
    /// @brief Bind the Hardware Buffer in order to use it.
    void bind() const;
    /// @brief Unbind the Hardware Buffer after it has been used.
    void unbind() const;
    /// @brief Update the buffer if dirty.
    void update() const;
    /// @brief Returns true if Buffer is invalid.
    bool isInvalid() const;
    /// @brief Returns the size of the buffer.
    size_t getSize() const;
    /// @brief Returns true if needs update.
    bool isDirty() const;
    /// @brief Returns true if invalidated.
    bool isExpired() const;
    
    /// @brief Add an index (a groupe of indice, or a face) to this buffer.
    void add(const IndexedFace& index);
    
    /// @brief Add a batch of face to this buffer.
    void add(const IndexedFaceBatch& batch);
    
    /// @brief Sets the material used to render this buffer.
    void setMaterial(const Material& material);
    
    /// @brief Returns the Material used.
    Material& getMaterial();
    /// @brief Returns the Material used.
    const Material& getMaterial() const;
    
    /// @brief Returns the Primitive type.
    PrimitiveType getPrimitiveType() const;
    /// @brief Returns the Storage Type.
    StorageType getStorageType() const;
    
    /// @brief Returns the number of elements.
    size_t getElementCount() const;
    
    /// @brief A Null HardwareIndexBuffer.
    static HardwareIndexBuffer Null;
    
    HardwareIndexBuffer& operator = (const HardwareIndexBuffer& rhs);
    
private:
    
    std::weak_ptr<HardwareIndexBufferPrivate> _mBuffer;
};

GRE_END_NAMESPACE

#endif
