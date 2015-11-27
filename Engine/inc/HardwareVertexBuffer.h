//
//  HardwareVertexBuffer.h
//  GRE
//
//  Created by Jacques Tronconi on 26/11/2015.
//
//

#ifndef GRE_HardwareVertexBuffer_h
#define GRE_HardwareVertexBuffer_h

#include "HardwareBuffer.h"
#include "Vertex.h"

GRE_BEGIN_NAMESPACE

/// @brief Defines a HardwareBuffer used to hold Vertexs.
class DLL_PUBLIC HardwareVertexBufferPrivate : public HardwareBufferPrivate
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    HardwareVertexBufferPrivate();
    virtual ~HardwareVertexBufferPrivate();
    
    /// @brief Add a Vertex to this buffer.
    /// If allowDuplicate is true, then no checking will be done
    /// about the unicity of the given vertex.
    void add(const Vertex& vertex);
    
    /// @brief Adds a Batch of Vertex to the buffer.
    void add(const VertexBatch& batch);
    
    /// @brief Returns true if this buffer contains at least one copy of given
    /// vertex.
    bool contains(const Vertex& vertex) const;
    
    /// @brief Set this property to true to allow duplicates.
    void setDuplicatesAllowed(bool allowed);
    /// @brief Returns true if duplicates are allowed.
    bool areDuplicatesAllowed() const;
    
protected:
    
    std::vector<Vertex> _mVertexs;///< @brief The Vertexs holded by this buffer.
    bool _mAllowDuplicates;///< @brief True if duplicates are allowed.
};

/// @brief Proxy to the HardwareVertexBufferPrivate object.
class DLL_PUBLIC HardwareVertexBuffer : public HardwareBuffer
{
public:
    
    POOLED(Pools::HwdBuffer)
    
    HardwareVertexBuffer();
    HardwareVertexBuffer(const HardwareVertexBuffer& buffer);
    HardwareVertexBuffer(HardwareVertexBuffer&& buffer);
    explicit HardwareVertexBuffer(std::weak_ptr<HardwareVertexBufferPrivate> bufferptr);
    explicit HardwareVertexBuffer(const ResourceUser& ruser);
    
    ~HardwareVertexBuffer();
    
    /// @brief Bind the Hardware Buffer in order to use it.
    void bind() const;
    /// @brief Returns true if Buffer is invalid.
    bool isInvalid() const;
    /// @brief Returns the size of the buffer.
    size_t getSize() const;
    /// @brief Returns true if needs update.
    bool isDirty() const;
    /// @brief Returns true if invalidated.
    bool isExpired() const;
    
    /// @brief Adds a vertex to the buffer.
    void add(const Vertex& vertex);
    
    /// @brief Adds a Batch of Vertex to the buffer.
    void add(const VertexBatch& batch);
    
    /// @brief Returns true if this buffer contains at least one copy of given
    /// vertex.
    bool contains(const Vertex& vertex) const;
    
    /// @brief Set this property to true to allow duplicates.
    void setDuplicatesAllowed(bool allowed);
    /// @brief Returns true if duplicates are allowed.
    bool areDuplicatesAllowed() const;
    
    /// @brief A Null HardwareVertexBuffer.
    static HardwareVertexBuffer Null;
    
    HardwareVertexBuffer& operator = (const HardwareVertexBuffer& rhs);
    
private:
    
    std::weak_ptr<HardwareVertexBufferPrivate> _mBuffer;
};

GRE_END_NAMESPACE

#endif