//
//  Pass.h
//  GRE
//
//  Created by Jacques Tronconi on 06/01/2016.
//
//

#ifndef GRE_Pass_h
#define GRE_Pass_h

#include "Pools.h"
#include "Resource.h"
#include "HardwareProgram.h"

GRE_BEGIN_NAMESPACE

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
    
    PassPrivate (const std::string& name, const PassNumber& passNumber);
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
    
protected:
    
    /// @brief The actual number of this Pass.
    PassNumber _mNumber;
    
    /// @brief Activated property : true if the Pass should be drew be the Renderer.
    /// Default value is true.
    bool _mIsActivated;
    
    /// @brief The Program currently linked by this Pass. By default, HardwareProgram::Null.
    HardwareProgram _mLinkedProgram;
    
    /// @brief If true, it creates a HardwareProgram passthrough using the Renderer.
    bool _mAutoCreateProgram;
};

//////////////////////////////////////////////////////////////////////
/// @brief A Rendering Pass User object.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Pass : public ResourceUser
{
public:
    
    POOLED(Pools::Resource)
    
    Pass();
    Pass(const Pass& rhs);
    Pass(Pass&& rhs);
    explicit Pass(const ResourceUser& rhs);
    Pass& operator = (const Pass& rhs);
    
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
    
    /// @brief A Null Pass.
    static Pass Null;
    
private:
    
    /// @brief Holds a Quick weak pointer to PassPrivate object.
    std::weak_ptr<PassPrivate> _mPass;
};

/// @brief Defines a simple list of Pass objects.
typedef std::vector<Pass> PassList;

/// @brief Defines a list of shared PassPrivate objects. (For managers)
typedef std::vector<std::shared_ptr<PassPrivate> > PassPrivateOwnedList;

GRE_END_NAMESPACE

#endif
