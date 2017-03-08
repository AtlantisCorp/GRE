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

#include "Camera.h"
#include "SceneNode.h"
#include "Viewport.h"
#include "FrameBuffer.h"

GreBeginNamespace

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

//////////////////////////////////////////////////////////////////////
/// @brief Represents one draw of a set of RenderNodes.
///
/// Those nodes can be the entire scene, or only those specified by the
/// user. A Technique can render those Nodes using one HardwareProgram,
/// and can have several pre-render and post-render techniques.
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
    /// @brief Returns the Camera use for this Technique.
    //////////////////////////////////////////////////////////////////////
    virtual const CameraHolder & getCamera () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iCamera'.
    //////////////////////////////////////////////////////////////////////
    virtual void setCamera ( const CameraUser& camera ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iViewport'.
    //////////////////////////////////////////////////////////////////////
    virtual const Viewport & getViewport () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iViewport'.
    //////////////////////////////////////////////////////////////////////
    virtual void setViewport ( const Viewport& viewport ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iExclusive'.
    //////////////////////////////////////////////////////////////////////
    virtual bool isExclusive () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Changes 'iExclusive'.
    //////////////////////////////////////////////////////////////////////
    virtual void setExclusive ( bool value ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iRenderedNodes'.
    //////////////////////////////////////////////////////////////////////
    virtual const std::vector < RenderNodeHolder > & getNodes () const ;
    
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
    /// @brief Returns 'iActivated'.
    //////////////////////////////////////////////////////////////////////
    virtual bool isActivated () const ;
    
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
    /// @brief Normally called by the RenderScene::drawTechnique() function,
    /// this function should apply per light transformations depending on the
    /// used light.
    /// @note For example, in the ShadowMapping Technique, this function sets
    /// the framebuffer depth texture to the light's shadowmap texture.
    //////////////////////////////////////////////////////////////////////
    virtual void onPerLightRendering ( const Light & light ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a Technique to pre-technique list.
    //////////////////////////////////////////////////////////////////////
    virtual void addPreTechnique ( const TechniqueHolder & tech ) ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void onUpdateEvent ( const UpdateEvent & e ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Updates the Viewport.
    //////////////////////////////////////////////////////////////////////
    virtual void onWindowSizedEvent ( const WindowSizedEvent& e ) ;
    
protected:
    
    // [03.07.2017] NOTES : RenderPass has no significative utility right now, only getting the HardwareProgram
    // to use for the technique rendering. But, we might use only one HardwareProgram by technique, as a technique
    // is already considered as a RenderPass (i.e. one draw of the scene or assimilated objects).
    // std::vector < RenderPassHolder > iPasses ;
    
    /// @brief HardwareProgram used to render this technique.
    HardwareProgramHolder iProgram ;
    
    /// @brief
    CameraHolder iCamera ;
    
    /// @brief
    Viewport iViewport ;
    
    /// @brief A Technique is 'exclusive' if only some special RenderNodes are affected.
    bool iExclusive ;
    
    /// @brief
    std::vector < RenderNodeHolder > iRenderedNodes ;
    
    /// @brief Techniques that should be called 'before' this technique (pre-render effects).
    std::vector < TechniqueHolder > iPreTechniques ;
    
    /// @brief Techniques that should be called 'after' this technique (post-render effects).
    std::vector < TechniqueHolder > iPostTechniques ;
    
    /// @brief True if this Technique can be used to draw the RenderScene.
    bool iActivated ;
    
    /// @brief Lighting Render Mode used by this Technique.
    TechniqueLightingMode iLightingMode ;
    
    /// @brief A Framebuffer set to indicate the Renderer where to render the nodes.
    /// @note This Framebuffer is null if we want to render to the main context framebuffer, for
    /// example the Window's drawing zone. This Framebuffer is not null when we want to draw to a
    /// Texture , for example a Depth Texture for shadow mapping. As a pre-render technique, we use
    /// the framebuffer to render the depth buffer to the ShadowMap texture of lights objects. This lets
    /// us, in the main fragment shader, use the shadow map previously computed.
    RenderFramebufferHolder iFramebuffer ;
};

/// @brief
typedef SpecializedCountedObjectHolder < Technique > TechniqueHolder ;

/// @brief
typedef SpecializedResourceHolderList < Technique > TechniqueHolderList ;

/// @brief
typedef SpecializedCountedObjectUser < Technique > TechniqueUser ;

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
    /// @brief Loads a Technique and returns an holder to it.
    /// If 'techniqueholder' is invalid, this will try to find the preloaded technique
    /// with given name.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolder load ( const std::string & name , const TechniqueHolder& techniqueholder = TechniqueHolder(nullptr) ) ;
};

/// @brief
typedef SpecializedCountedObjectHolder < TechniqueManager > TechniqueManagerHolder ;

GreEndNamespace

#endif /* Technique_h */
