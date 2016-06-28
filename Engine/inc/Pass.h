//////////////////////////////////////////////////////////////////////
//
//  Pass.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/01/2016.
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

#ifndef GRE_Pass_h
#define GRE_Pass_h

#include "Pools.h"
#include "Resource.h"
#include "HardwareProgram.h"
#include "FrameBuffer.h"

GreBeginNamespace

class DLL_PUBLIC RendererResource;

/// @brief Type defining a Pass Number.
typedef uint32_t PassNumber;

/// @brief Defines the Pass Numbers that can be allocated.
enum class PassPurpose
{
    /// @brief The first pass. Generally already allocated by the Renderer, should draw
    /// every objects with a generic passthrough shader.
    First = 1,
    
    /// @brief The last pass. This Pass can't be allocated. The number of Pass is high, i
    /// hope you'll never have to get through 999 rendering of the Scene or you will have a
    /// dramatic perf issue !
    Last = 999
};

//////////////////////////////////////////////////////////////////////
/// @brief A Rendering Pass Private object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC PassPrivate : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a Pass given its name and purpose.
    //////////////////////////////////////////////////////////////////////
    PassPrivate (const std::string& name, const PassNumber& passNumber);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destructs the Pass.
    //////////////////////////////////////////////////////////////////////
    virtual ~PassPrivate();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This method is called by the RendererResource Object to
    /// notify the Pass that it has to be renderer.
    /// @note If the HardwareShaderProgram used by this Pass is null, it
    /// should create a new Passthrough ShaderProgram using the renderer
    /// function RendererResource::createHardwareProgram(HardwareShader::PassThrough,
    /// HardwareShader::PassThrough). HardwareShader::Passthrough is a special
    /// value that tells the Renderer to create pass-through shaders.
    //////////////////////////////////////////////////////////////////////
    virtual void renderWith(RendererResource* renderer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the property to true if you want this Pass to be enabled.
    //////////////////////////////////////////////////////////////////////
    void setActivated(bool activate);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Tells if the Pass is enabled.
    //////////////////////////////////////////////////////////////////////
    bool isActivated() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the HardwareProgram used by this PassPrivate object.
    //////////////////////////////////////////////////////////////////////
    void setHardwareProgram(const HardwareProgram& hwdProgram);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwareProgram object. By default, HardwareProgram::Null.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram getHardwareProgram() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the current PassNumber.
    //////////////////////////////////////////////////////////////////////
    PassNumber getPassNumber() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the _mAutoCreateProgram property.
    //////////////////////////////////////////////////////////////////////
    void setAutoCreateProgram(bool enabled);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Pass is allowed to generate the passthrough
    /// shader.
    //////////////////////////////////////////////////////////////////////
    bool autoCreateProgram() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the FrameBuffer object currently used by this Pass.
    /// @note If the Pass had never been used, this property has good chances
    /// to be FrameBuffer::Null.
    //////////////////////////////////////////////////////////////////////
    FrameBuffer& getFrameBuffer();
    const FrameBuffer& getFrameBuffer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets if this Pass should be draw using Fbo method.
    //////////////////////////////////////////////////////////////////////
    void setFrameBufferedRendering(bool fborender);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if _mFrameBufferedRendering is true.
    //////////////////////////////////////////////////////////////////////
    bool isFrameBufferedRendering() const;
    
protected:
    
    /// @brief The actual number of this Pass.
    PassNumber iNumber;
    
    /// @brief Activated property : true if the Pass should be drew be the Renderer.
    /// Default value is true.
    bool iIsActivated;
    
    /// @brief The Program currently linked by this Pass. By default, HardwareProgram::Null.
    HardwareProgram iLinkedProgram;
    
    /// @brief If true, it creates a HardwareProgram passthrough using the Renderer.
    bool iAutoCreateProgram;
    
    /// @brief A FrameBuffer object. This object is created by the Renderer.
    FrameBuffer iFbo;
    
    /// @brief True if FrameBuffered rendering is desired.
    /// When _mNumber is 0 (default Pass), this value is false by
    /// default. When _mNumber is not 0 (custom Pass), this value
    /// is true.
    bool iFrameBufferedRendering;
};

/// @brief SpecializedResourceHolder for PassPrivate.
typedef SpecializedResourceHolder<PassPrivate> PassHolder;

/// @brief SpecializedResourceHolderList for PassHolder list.
typedef SpecializedResourceHolderList<PassPrivate> PassHolderList;

//////////////////////////////////////////////////////////////////////
/// @brief SpecializedResourceUser for PassPrivate.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Pass : public SpecializedResourceUser<PassPrivate>
{
public:
    
    POOLED(Pools::Resource)
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs from pointer.
    //////////////////////////////////////////////////////////////////////
    Pass(PassPrivate* pointer);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs from holder.
    //////////////////////////////////////////////////////////////////////
    Pass(const PassHolder& holder);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs from user.
    //////////////////////////////////////////////////////////////////////
    Pass(const Pass& user);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destructs the Pass user.
    //////////////////////////////////////////////////////////////////////
    ~Pass();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief This method is called by the RendererResource Object to
    /// notify the Pass that it has to be renderer.
    //////////////////////////////////////////////////////////////////////
    void renderWith(RendererResource* renderer) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the property to true if you want this Pass to be enabled.
    //////////////////////////////////////////////////////////////////////
    void setActivated(bool activate);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Tells if the Pass is enabled.
    //////////////////////////////////////////////////////////////////////
    bool isActivated() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes the HardwareProgram used by this PassPrivate object.
    //////////////////////////////////////////////////////////////////////
    void setHardwareProgram(const HardwareProgram& hwdProgram);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwareProgram object. By default, HardwareProgram::Null.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram getHardwareProgram() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the current PassNumber.
    //////////////////////////////////////////////////////////////////////
    PassNumber getPassNumber() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the _mAutoCreateProgram property.
    //////////////////////////////////////////////////////////////////////
    void setAutoCreateProgram(bool enabled);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the Pass is allowed to generate the passthrough
    /// shader.
    //////////////////////////////////////////////////////////////////////
    bool autoCreateProgram() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the FrameBuffer object currently used by this Pass.
    /// @note If the Pass had never been used, this property has good chances
    /// to be FrameBuffer::Null.
    //////////////////////////////////////////////////////////////////////
    FrameBuffer& getFrameBuffer();
    const FrameBuffer& getFrameBuffer() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets if this Pass should be draw using Fbo method.
    //////////////////////////////////////////////////////////////////////
    void setFrameBufferedRendering(bool fborender);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if _mFrameBufferedRendering is true.
    //////////////////////////////////////////////////////////////////////
    bool isFrameBufferedRendering() const;
    
    /// @brief A Null Pass.
    static Pass Null;
};

/// @brief Defines a simple list of Pass objects.
typedef std::list<Pass> PassList;

GreEndNamespace

#endif
