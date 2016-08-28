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
#include "SceneNode.h"

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
///
/// A Pass is an object, able to add a step in the Rendering Procees.
/// Normally, one Pass is used by the Renderer to draw the Scene with
/// one HardwareShader onto a Renderer's private framebuffer. Then, this
/// RenderFramebuffer is blended with the following pass's ones.
/// The result of this is a blend between all Pass's effect, from first
/// to last.
///
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
    /// @brief Returns 'true' if given RenderNodeIdentifier is acceptable
    /// to draw in this Pass object.
    /// @note By default, this function returns always 'true'.
    //////////////////////////////////////////////////////////////////////
    virtual bool isAcceptable ( const RenderNodeIdentifier& identifier ) const;
    
protected:
    
    /// @brief The actual number of this Pass.
    PassNumber iNumber;
    
    /// @brief Activated property : true if the Pass should be drew be the Renderer.
    /// Default value is true.
    bool iIsActivated;
    
    /// @brief The Program currently linked by this Pass. By default, HardwareProgram::Null.
    HardwareProgram iLinkedProgram;
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
    Pass(const PassPrivate* pointer);
    
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
    
    /// @brief A Null Pass.
    static Pass Null;
};

/// @brief Defines a simple list of Pass objects.
typedef std::list<Pass> PassList;

GreEndNamespace

#endif
