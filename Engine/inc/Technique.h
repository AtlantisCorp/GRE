//////////////////////////////////////////////////////////////////////
//
//  Technique.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/02/2017.
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

#ifndef Technique_h
#define Technique_h

#include "Pools.h"
#include "Resource.h"
#include "SpecializedResourceManager.h"
#include "FrameBuffer.h"
#include "Light.h"
#include "HardwareProgram.h"
#include "ResourceBundle.h"

GreBeginNamespace

class Renderer ;

//////////////////////////////////////////////////////////////////////
/// @brief Determines lighting render mode for a Technique.
enum class TechniqueLightingMode : int
{
    /// @brief Uses every Light objects activated in the Scene.
    AllLights ,

    /// @brief Uses the involved Technique once per light activated in the Scene.
    PerLight ,

    /// @brief No lights may be used.
    None
};

/// @brief Translates a string to a lighting mode .
TechniqueLightingMode TechniqueLightingModeFromString ( const std::string & mode ) ;

//////////////////////////////////////////////////////////////////////
/// @brief Defines every alias used by the engine.
enum class TechniqueParam : int
{
    ModelMatrix ,
    ViewMatrix ,
    ProjectionMatrix ,
    ProjectionViewMatrix ,
    NormalMatrix ,
    NormalMatrix3 ,

    CameraPosition ,
    CameraDirection ,

    ViewportWidth ,
    ViewportHeight ,
    ViewportLeft ,
    ViewportTop ,

    ClearColor ,
    ClearDepth ,

    Light0 , Light1 , Light2 , Light3 , Light4 , Light5 , Light6 , Light7 ,
    Light8 , Light9 ,

    LightAmbient ,
    LightDiffuse ,
    LightSpecular ,
    LightPosition ,
    LightDirection ,
    LightAttCst ,
    LightAttLine ,
    LightAttQuad ,
    LightSpotAngle ,
    LightSpotExposition ,
    LightTexShadow ,
    LightShadowMatrix ,

    Texture0 , Texture1 , Texture2 , Texture3 , Texture4 , Texture5 , Texture6 ,
    Texture7 , Texture8 , Texture9 ,

    MaterialAmbient ,
    MaterialDiffuse ,
    MaterialSpecular ,
    MaterialTexAmbient ,
    MaterialTexDiffuse ,
    MaterialTexSpecular ,
    MaterialTexNormal ,
    MaterialShininess
};

/// @brief Translates a string into a TechniqueParam.
TechniqueParam TechniqueParamFromString ( const std::string & param ) ;

//////////////////////////////////////////////////////////////////////
/// @brief Represents a set of shader's parameters to draw something
/// in the binded context by the renderer.
///
/// A technique defines how the renderer will draw a node. The technique
/// holds the program object , the parameters that come along with it ,
/// and the values that will be bound to it when using the program.
///
/// Aliases are also stored. There are many aliases the user can use to
/// define custom names for standard values , as camera's position and
/// direction , or light's and material's properties. When the technique
/// will be used , binding the camera will bind its values to the
/// alias previously set by the user.
///
/// Managing the lights : lights are objects that can't 'use' a technique.
/// This is a technique's job to use a light. Because lights must be pushed
/// each times using it , the technique is responsible for managing its
/// lights as it wants. By default , lights are pushed each time the
/// function 'useLight' is called and so the uploaded light will have the
/// name given by the alias corresponding to the given light. Lights also have
/// the special alias LightTexShadow which should be only used by the light to
/// provide a shadowmap to the shader program. Notes that you should use the same
/// naming conventions for all your lights in the same program , as the lights
/// are accessed using the following : 'LightAlias.LightParam' . Notes that if the
/// LightAlias is empty , it will use only LightParam without the '.' .
///
/// Managing the samplers/textures : Textures are used by the technique using
/// the same system as lights. They can be pushed and bound by the technique
/// object each time the 'useTexture' function is called. The user can
/// customize the name for each texture using aliases.
///
/// Using materials : Materials uses the technique to bind every parameters
/// to the shader program. Colors , and textures are bound. Texture binding
/// is done only if the material gets some. Also , material textures have
/// some special aliases , like MaterialTexDiffuse , MaterialTexSpecular ,
/// MaterialTexAmbient.
///
//////////////////////////////////////////////////////////////////////
class Technique : public Resource
{
public:

    POOLED ( Pools::Render )

    /// @brief
    typedef SpecializedCountedObjectHolder < Technique > TechniqueHolder ;

    /// @brief
    typedef SpecializedResourceHolderList < Technique > TechniqueHolderList ;

    /// @brief
    typedef SpecializedCountedObjectUser < Technique > TechniqueUser ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Technique ( const std::string & name = "DefaultTechnique" ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Technique () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the HardwareProgram this technique uses.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramHolder & getHardwareProgram () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets 'iProgram'.
    //////////////////////////////////////////////////////////////////////
    virtual void setHardwareProgram ( const HardwareProgramHolder & program ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief True if has techniques before itself.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasPreTechniques () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPreTechniques'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < TechniqueHolder > & getPreTechniques () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief True if 'iPostTechniques' is not empty.
    //////////////////////////////////////////////////////////////////////
    virtual bool hasPostTechniques () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iPostTechniques'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < TechniqueHolder > & getPostTechniques () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iLightingMode'.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueLightingMode getLightingMode () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets 'iLightingMode'.
    //////////////////////////////////////////////////////////////////////
    virtual void setLightingMode ( const TechniqueLightingMode& lightingmode ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iFramebuffer'.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderFramebufferHolder & getFramebuffer () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets 'iFramebuffer'.
    //////////////////////////////////////////////////////////////////////
    virtual void setFramebuffer ( const RenderFramebufferHolder & framebuffer ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Technique to pre-technique list.
    //////////////////////////////////////////////////////////////////////
    virtual void addPreTechnique ( const TechniqueHolder & tech ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the alias for the given parameter.
    //////////////////////////////////////////////////////////////////////
    virtual void setAlias ( const TechniqueParam & param , const std::string & alias ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the alias for given parameter.
    //////////////////////////////////////////////////////////////////////
    virtual std::string getAlias ( const TechniqueParam & param ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sends a parameter value from two alias , the object and the
    /// subobject aliases.
    //////////////////////////////////////////////////////////////////////
    virtual void setAliasedParameterStructValue (const TechniqueParam & alias1 ,
                                                 const TechniqueParam & alias2 ,
                                                 const HdwProgVarType & type ,
                                                 const RealProgramVariable & value ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Uses an alias to bind a parameter value.
    //////////////////////////////////////////////////////////////////////
    virtual void setAliasedParameterValue (const TechniqueParam & name ,
                                           const HdwProgVarType & type ,
                                           const RealProgramVariable & value) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds to parameter the given value.
    //////////////////////////////////////////////////////////////////////
    virtual void setNamedParameterValue (const std::string & name ,
                                         const HdwProgVarType & type ,
                                         const RealProgramVariable & value) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the technique's shader program.
    //////////////////////////////////////////////////////////////////////
    virtual void bind () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds the technique's shader program.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the next Alias a node or a render object may use to
    /// send values to the shader object lights.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueParam getNextLightAlias () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the current Alias used to send values to the shader
    /// object lights.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueParam getCurrentLightAlias () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Resets the light counter to Zero.
    //////////////////////////////////////////////////////////////////////
    virtual void resetLights () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a texture to an object structure using two aliases.
    //////////////////////////////////////////////////////////////////////
    virtual void setAliasedTextureStruct (const TechniqueParam & alias1 ,
                                          const TechniqueParam & alias2 ,
                                          const TextureHolder & tex) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a texture to the aliased parameter and binds the texture
    /// to the next texture unit.
    //////////////////////////////////////////////////////////////////////
    virtual void setAliasedTexture ( const TechniqueParam & param , const TextureHolder & tex ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the name used to bind given Vertex Attribute Alias.
    //////////////////////////////////////////////////////////////////////
    virtual void setAttribName ( const VertexAttribAlias & alias , const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the name used to bind given Vertex Attribute Alias ,
    /// or an empty string if it doesn't exist.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string getAttribName ( const VertexAttribAlias & alias ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Resets the texture unit counter to 0 .
    //////////////////////////////////////////////////////////////////////
    virtual void resetTextures () const ;

protected:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent ( const UpdateEvent & e ) ;

protected:

    /// @brief HardwareProgram used to render this technique.
    HardwareProgramHolder iProgram ;

    /// @brief Techniques that should be called 'before' this technique (pre-render effects).
    std::vector < TechniqueHolder > iPreTechniques ;

    /// @brief Techniques that should be called 'after' this technique (post-render effects).
    std::vector < TechniqueHolder > iPostTechniques ;

    /// @brief Lighting Render Mode used by this Technique.
    TechniqueLightingMode iLightingMode ;

    /// @brief A Framebuffer set to indicate the Renderer where to render the nodes.
    /// @note This Framebuffer is null if we want to render to the main context framebuffer, for
    /// example the Window's drawing zone. This Framebuffer is not null when we want to draw to a
    /// Texture , for example a Depth Texture for shadow mapping. As a pre-render technique, we use
    /// the framebuffer to render the depth buffer to the ShadowMap texture of lights objects. This lets
    /// us, in the main fragment shader, use the shadow map previously computed.
    RenderFramebufferHolder iFramebuffer ;

    /// @brief Holds aliases used for this technique. They will be used when using 'setAliasedParameter'
    /// to bind a parameter to a shader program.
    std::map < TechniqueParam , std::string > iAliases ;

    /// @brief Holds the real attributes name for their alias. They will be used by the HardwareProgram
    /// to look for a valid attribute location.
    std::map < VertexAttribAlias , std::string > iAttribAliases ;

    /// @brief Holds the current texture unit used in the binded program. When rendering the
    /// technique , it uses this number to know which texture unit the next texture should
    /// activate. When binding a new program, this number is -1 (next texture unit is 0). When
    /// unbinding the program, this number falls down from the number of activated units minus 1
    /// to -1 . This number should never be accessed to technique's user.
    mutable int iCurrentTextureUnit ;

    /// @brief Counter used to access light's alias. The light is accessed using 'Light0' + this
    /// counter. It must be inferior or equal to 'Light9'. This counter is incremented by using 'useLight'
    /// and reset to Zero when using 'resetLights'. Notes that 'resetLights' also sets every light
    /// parameters to Zero in the shader program. By default , it begins with value '-1'.
    mutable int iCurrentLight ;
};

/// @brief
typedef SpecializedCountedObjectHolder < Technique > TechniqueHolder ;

/// @brief
typedef SpecializedResourceHolderList < Technique > TechniqueHolderList ;

/// @brief
typedef SpecializedCountedObjectUser < Technique > TechniqueUser ;

//////////////////////////////////////////////////////////////////////
/// @brief Loads a specific Technique.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TechniqueLoader : public ResourceLoader
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TechniqueLoader ( ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TechniqueLoader () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Should load a named technique with given args.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolder load ( const std::string & name , const ResourceLoaderOptions & options ) const ;
};

//////////////////////////////////////////////////////////////////////
/// @brief A Manager to holds every Technique used in the engine.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TechniqueManager : public ResourceManagerBase < Technique >
{
public:

    POOLED ( Pools::Manager )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TechniqueManager ( const std::string & name = "DefaultTechniqueManager" ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TechniqueManager () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads every Technique's found in the given bundles , and
    /// returns the number of technique's loaded.
    //////////////////////////////////////////////////////////////////////
    virtual int loadFromBundles ( const std::vector < ResourceBundleHolder > & bundles ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads every Technique's found in the given bundle , and returns
    /// the number of technique's loaded.
    //////////////////////////////////////////////////////////////////////
    virtual int loadFromBundle ( const ResourceBundleHolder & bundle ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads Technique from given file. Uses the parser bundled with
    /// this Engine to parse the Technique file , but notes that the parser
    /// may holds differents parser subversion to parse old files. Also ,
    /// notes that a valid renderer may be loaded before loading any
    /// Technique , due to the programs creations process. Notes also that
    /// one Technique's file may holds more than one technique.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolderList loadFromFile ( const std::string & path ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Registers the given technique from its holder. A check will
    /// occur to verify the given technique is not already loaded.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolder loadFromHolder ( const TechniqueHolder & technique ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a blank technique.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolder loadBlank ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first technique encountered with given name.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolder get ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the first technique encountered with given name.
    //////////////////////////////////////////////////////////////////////
    virtual const TechniqueHolder get ( const std::string & name ) const ;
};

/// @brief
typedef SpecializedCountedObjectHolder < TechniqueManager > TechniqueManagerHolder ;

GreEndNamespace

#endif /* Technique_h */
