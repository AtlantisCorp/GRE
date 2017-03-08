//////////////////////////////////////////////////////////////////////
//
//  Scene.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 14/12/2015.
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

#include "Scene.h"
#include "RenderingQuery.h"

#include "ResourceManager.h"

GreBeginNamespace

LightRenderNode::LightRenderNode ( const std::string& name )
: RenderNode ( name )
{
    
}

LightRenderNode::LightRenderNode ( const Light& light , const std::string& name )
: RenderNode ( name ) , iLight(light)
{
    
}

LightRenderNode::~LightRenderNode() noexcept ( false )
{
    
}

const Light& LightRenderNode::getLight() const
{
    GreAutolock ; return iLight ;
}

Light& LightRenderNode::getLight()
{
    GreAutolock ; return iLight ;
}

void LightRenderNode::setLight(const Gre::Light &rhs)
{
    GreAutolock ; iLight = rhs ;
}

void LightRenderNode::onPositionChangedEvent(const Gre::PositionChangedEvent &e)
{
    GreAutolock ; iLight.setPosition ( e.Position ) ;
    RenderNode::onPositionChangedEvent(e) ;
}

void LightRenderNode::onDirectionChangedEvent(const Gre::DirectionChangedEvent &e)
{
    GreAutolock ; iLight.setDirection( e.Direction ) ;
    RenderNode::onDirectionChangedEvent(e) ;
}

// ---------------------------------------------------------------------------------------------------

RenderScene::RenderScene(const std::string& name, const RenderSceneOptions& options)
: Gre::Resource(name) , iRootNode(nullptr) , iGlobalLight(Vector3(-1000.0f, 1000.0f, -1000.0f))
{
    
}

RenderScene::~RenderScene()
{
    
}

const RenderNodeUser RenderScene::getRootNode() const
{
    return iRootNode;
}

const RenderNodeHolder& RenderScene::setRootNode(const RenderNodeHolder &rendernode)
{
    GreAutolock ;
    
    if ( !iRootNode.isInvalid() )
    {
        Resource::removeListener( EventProceederUser(iRootNode) );
    }
    
    iRootNode = rendernode;
    
    if ( !iRootNode.isInvalid() )
    {
        Resource::addListener( EventProceederUser(iRootNode) );
    }
    
    return iRootNode;
}

RenderNodeHolder RenderScene::createNode( const std::string & name ) const
{
    return RenderNodeHolder ( new RenderNode(name) );
}

LightRenderNodeHolder RenderScene::createLightNode( const std::string & name ) const
{
    return LightRenderNodeHolder ( new LightRenderNode(name) ) ;
}

LightRenderNodeHolder RenderScene::createLightNode(const Gre::Light &light, const std::string& name) const
{
    return LightRenderNodeHolder ( new LightRenderNode(light, name) ) ;
}

void RenderScene::addLightNode(const LightRenderNodeHolder &lightnode)
{
    if ( !lightnode.isInvalid() )
    {
        iLightNodes.push_back(lightnode) ;
        addNode(lightnode) ;
    }
}

void RenderScene::addNode(const RenderNodeHolder &rendernode)
{
    GreAutolock ;
    
    if ( !iRootNode.isInvalid() )
    {
        iRootNode->addNode(rendernode);
    }
    
    else
    {
        setRootNode ( rendernode ) ;
    }
}

void RenderScene::removeNode( const RenderNodeHolder & holder )
{
    if ( !holder.isInvalid() )
    {
        GreAutolock ;
        
        // See if this node has a parent.
        
        RenderNodeHolder parent = holder->getParent () ;
        
        if ( !parent.isInvalid() )
        {
            parent -> remove ( holder-> getIdentifier() ) ;
        }
        
        // Now this child will be destroyed when the last holder holding it
        // will be destroyed. Normally, this holder is used by the USER , and at
        // the end of its function, should be destroyed.
        
        // Note : This function version also destroys every children of the
        // RenderNode to destroy. See 'RenderScene::removeNodeNoChildren()' to remove
        // a RenderNode without removing its children.
    }
}

void RenderScene::removeNodeNoChildren ( const RenderNodeHolder & holder )
{
    if ( !holder.isInvalid() )
    {
        GreAutolock ;
        
        // See if this node has a parent.
        
        RenderNodeHolder parent = holder->getParent () ;
        
        if ( !parent.isInvalid() )
        {
            parent -> removeNotRecursive ( holder-> getIdentifier() ) ;
        }
        
        // Now this child will be destroyed when the last holder holding it
        // will be destroyed. Normally, this holder is used by the USER , and at
        // the end of its function, should be destroyed.
        
        // Note : This function destroys only the given RenderNode. Its children
        // are distribuited in the parent's children using 'RenderNode::addNode()'.
    }
}

void RenderScene::removeNodeFromIdentifier(const Gre::ResourceIdentifier &identifier)
{
    GreAutolock ;
    
    // Try to find a Node with given identifier. This function is therefore much more
    // time-cost than 'RenderScene::removeNode()'.
    
    RenderNodeHolder node = findHolder ( identifier ) ;
    
    if ( !node.isInvalid() )
    {
        removeNode(node);
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Invalid ResourceIdentifier given : '" << (uint32_t) identifier << "'." << Gre::gendl;
    }
#endif
    
}

RenderNodeHolder RenderScene::findHolder ( const Gre::ResourceIdentifier & identifier )
{
    GreAutolock ;
    
    if ( !iRootNode.isInvalid() )
    {
        if ( iRootNode->getIdentifier() == identifier )
            return iRootNode ;
        
        return iRootNode -> find(identifier) ;
    }
    
    return RenderNodeHolder ( nullptr ) ;
}

RenderNodeUser RenderScene::findNode ( const Gre::ResourceIdentifier & identifier)
{
    GreAutolock ;
    
    if ( !iRootNode.isInvalid() )
    {
        if ( iRootNode->getIdentifier() == identifier )
            return iRootNode ;
        
        return iRootNode -> find(identifier) ;
    }
    
    return RenderNodeHolder ( nullptr ) ;
}

const RenderNodeUser RenderScene::findNode ( const Gre::ResourceIdentifier & identifier) const
{
    GreAutolock ;
    
    if ( !iRootNode.isInvalid() )
    {
        if ( iRootNode->getIdentifier() == identifier )
            return iRootNode ;
        
        return iRootNode -> find(identifier) ;
    }
    
    return RenderNodeHolder ( nullptr ) ;
}

void RenderScene::draw(const EventHolder &elapsed) const
{
    GreAutolock ;
    
    if ( iRenderTarget.isInvalid() )
        return ;
    
    if ( elapsed.isInvalid() || elapsed->getType() != EventType::Update )
        return ;
    
    if ( !iTechnique.isInvalid() )
    {
        iRenderTarget -> bind () ;
        
        {
            EventHolder e = EventHolder ( new RenderScenePreRenderEvent(this) ) ;
            sendEvent ( e ) ;
            
            const RendererHolder rholder = iRenderer.lock() ;
            if ( !rholder.isInvalid() ) {
                rholder->preRender (iClearColor) ;
            }
            
            _preRender () ;
        }
        
        {
            drawTechnique(iTechnique, elapsed) ;
        }
        
        {
            EventHolder e = EventHolder ( new RenderScenePostRenderEvent(this) ) ;
            sendEvent ( e ) ;
            
            const RendererHolder rholder = iRenderer.lock() ;
            if ( !rholder.isInvalid() ) {
                rholder->postRender () ;
            }
            
            _postRender () ;
        }
    
        iRenderTarget -> swapBuffers () ;
        iRenderTarget -> unbind () ;
    }
}

void RenderScene::setTechnique(const TechniqueUser &technique)
{
    GreAutolock ;
    
    if ( !iTechnique.isInvalid() )
        removeListener(EventProceederUser(iTechnique)) ;
    
    iTechnique = technique.lock() ;
    
    if ( !iTechnique.isInvalid() )
        addListener(EventProceederUser(iTechnique)) ;
    
#ifdef GreIsDebugMode
    if ( technique.isInvalid() ) {
        GreDebug("[WARN] Setting invalid Technique to RenderScene '") << getName() << "'." << Gre::gendl ;
    }
#endif
}

TechniqueHolder& RenderScene::getTechnique ()
{
    GreAutolock ; return iTechnique ;
}

const TechniqueHolder& RenderScene::getTechnique () const
{
    GreAutolock ; return iTechnique ;
}

void RenderScene::setRenderer ( const RendererUser& renderer )
{
    GreAutolock ; iRenderer = renderer ;
    
#ifdef GreIsDebugMode
    if ( renderer.isInvalid() ) {
        GreDebug("[WARN] Setting invalid Renderer to RenderScene '") << getName() << "'." << Gre::gendl ;
    }
#endif
}

void RenderScene::setRenderTarget(const RenderTargetUser &target)
{
    GreAutolock ;
    
    if ( !iRenderTarget.isInvalid() )
        iRenderTarget -> removeListener(EventProceederUser(this)) ;
    
    iRenderTarget = target.lock() ;
    
    if ( !iRenderTarget.isInvalid() )
        iRenderTarget -> addListener(EventProceederUser(this)) ;
    
#ifdef GreIsDebugMode
    if ( target.isInvalid() ) {
        GreDebug("[WARN] Setting invalid RenderTarget to RenderScene '") << getName() << "'." << Gre::gendl ;
    }
#endif
}

void RenderScene::setClearColor ( const Color& color )
{
    GreAutolock ; iClearColor = color ;
}

const Light& RenderScene::getGlobalLight () const
{
    GreAutolock ; return iGlobalLight ;
}

Light& RenderScene::getGlobalLight ()
{
    GreAutolock ; return iGlobalLight ;
}

void RenderScene::setGlobalLight ( const Light& light )
{
    GreAutolock ; iGlobalLight = light ;
}

void RenderScene::setMainCamera(const CameraUser &camera)
{
    GreAutolock ;

    if ( !iTechnique.isInvalid() ) {
        iTechnique->setCamera(camera);
    }

}

void RenderScene::drawTechnique( const TechniqueHolder& technique , const EventHolder &elapsed) const
{
#ifdef GreIsDebugMode
    if ( technique.isInvalid() ) {
        GreDebug("[WARN] Invalid Technique to draw RenderScene '") << getName() << "'." << Gre::gendl ;
        return ;
    } else if ( elapsed.isInvalid() ) {
        GreDebug("[WARN] Invalid EventHolder to draw RenderScene '") << getName() << "'." << Gre::gendl ;
        return ;
    }
#endif
    
    if ( technique->isActivated() )
    {
        if ( technique->hasPreTechniques() )
        {
            for ( auto & tech : technique->getPreTechniques() )
            {
                if ( !tech.isInvalid() && tech->isActivated() )
                {
                    drawTechnique(tech, elapsed) ;
                }
            }
        }
        
        RenderingQuery query ;
        query.setRenderScene ( RenderSceneUser(this) ) ;
//      query.setRenderPass ( pass ) ;
        query.setCamera ( technique->getCamera() ) ;
        query.setHardwareProgram ( technique->getHardwareProgram() ) ;
        query.setViewport ( technique->getViewport() ) ;
        
        const UpdateEvent& u = elapsed->to<UpdateEvent>() ;
        query.setElapsedTime ( u.elapsedTime ) ;
        
        if ( technique->isExclusive() ) {
            query.setRenderedNodes ( technique->getNodes() ) ;
        } else {
            query.setRenderedNodes ( { getRootNode().lock() } ) ;
        }
        
        RendererHolder rholder = iRenderer.lock() ;
        if ( rholder.isInvalid() )
            return ;
        
        // For shadow mapping, in order to let the technique set some variables, we must associate
        // a shadow map for each lights. The trick here is to call a special technique function, to
        // modify the framebuffer in order to bind the light shadowmap texture to the framebuffer.
        
        // If the technique is allowed to use every lights in the scene (default behaviour), we compute
        // every activated lights and give them to the query to be rendered (Blinn-Phong model).
        
        if ( technique->getLightingMode () == TechniqueLightingMode::AllLights )
        {
            std::vector < Light > lights ;
            lights.push_back(iGlobalLight) ;
            
            for ( LightRenderNodeUser lightnode : iLightNodes ) {
                if ( !lightnode.isInvalid() ) {
                    LightRenderNodeHolder light = lightnode.lock() ;
                    if ( light -> getLight().isEnabled() ) {
                        lights.push_back(light -> getLight()) ;
                    }
                }
            }
            
            query.setLights(lights) ;
            query.setFramebuffer ( technique->getFramebuffer() ) ;
            rholder -> draw ( query ) ;
        }
        
        // If the technique wants to be rendered differently for every lights, the 'perlight' flag is on
        // and we call the technique for each activated light. This let the technique do 'per-light' rendering
        // like setting light shadowmaps.
        
        else if ( technique->getLightingMode () == TechniqueLightingMode::PerLight )
        {
            std::vector < Light > lights ;
            lights.push_back(iGlobalLight) ;
            
            for ( LightRenderNodeUser lightnode : iLightNodes ) {
                if ( !lightnode.isInvalid() ) {
                    LightRenderNodeHolder light = lightnode.lock() ;
                    if ( light -> getLight().isEnabled() ) {
                        lights.push_back(light -> getLight()) ;
                    }
                }
            }
            
            for ( Light& light : lights )
            {
                std::vector < Light > tmp ;
                tmp.push_back(light) ;
                
                // This is a special function called for each light. This will let the technique change
                // the framebuffer depth texture to the one created in light.
                // [07.03.2017] NOTES : As 'onPerLightRendering' might modify some of the techniques properties,
                // we use this holder's trick to use non-const method on technique. This acts like a threadsafe
                // const_cast .
                
                TechniqueHolder(technique) -> onPerLightRendering ( light ) ;
                
                // Next, we bind the light and the technique framebuffer.
                
                query.setLights(tmp) ;
                query.setFramebuffer ( technique->getFramebuffer() ) ;
                rholder -> draw ( query ) ;
            }
        }
        
        // If the technique has disabled every lighting, we render it using only the technique's framebuffer
        // (if has one) and draw the query. The result is generally expected to be black.
        
        else
        {
            query.setFramebuffer ( technique->getFramebuffer() ) ;
            rholder -> draw(query) ;
        }
        
        if ( technique->hasPostTechniques() )
        {
            for ( const TechniqueHolder & tech : technique->getPostTechniques() )
            {
                drawTechnique(tech, elapsed) ;
            }
        }
    }
}

void RenderScene::_preRender () const
{
    
}

void RenderScene::_postRender () const
{
    
}

// ---------------------------------------------------------------------------------------------------

RenderSceneLoader::RenderSceneLoader()
{
    
}

RenderSceneLoader::~RenderSceneLoader()
{
    
}

ResourceLoader* RenderSceneLoader::clone() const
{
    return new RenderSceneLoader () ;
}

bool RenderSceneLoader::isLoadable(const std::string &filepath) const
{
    return false;
}

RenderSceneHolder RenderSceneLoader::load(const std::string &name, const RenderSceneOptions &options) const
{
    return RenderSceneHolder ( new RenderScene(name, options) ) ;
}

// ---------------------------------------------------------------------------------------------------

RenderSceneManager::RenderSceneManager ( const std::string& name )
: SpecializedResourceManager<Gre::RenderScene, Gre::RenderSceneLoader>(name)
{
    iLoaders.registers( "Default" , new RenderSceneLoader() );
	iInitialized = false ;
}

RenderSceneManager::~RenderSceneManager() noexcept ( false )
{
    
}

void RenderSceneManager::initialize()
{
	GreAutolock ;
	
	if ( iInitialized )
		return ;
	
	// We create here the 'Default' technique. This technique does not do anything special. It loads a
    // default program. Every nodes are included. And it has, by default, no subtechniques. 
	// NOTES : This default behaviour can only be loaded when a HardwareProgramManager and a CameraManager
	// are available. Thus, this can be done ONLY when the Renderer is installed. The Renderer should
	// use the 'initialize' method from the RenderSceneManager in order to install this.
	
	if ( ResourceManager::Get().getCameraManager().isInvalid() ||
		 ResourceManager::Get().getHardwareProgramManager().isInvalid() ) {
#ifdef GreIsDebugMode
		GreDebug("[WARN] Can't initialize RenderSceneManager because CameraManager OR HardwareProgramManager") 
		<< " are invalid. Please load both of them before initializing the scene manager." << gendl ;
#endif
		return ;
	}

    RenderPassHolder pass = RenderPassHolder ( new RenderPass () ) ;
    pass->setHardwareProgram ( ResourceManager::Get().getHardwareProgramManager()->getProgram("Default") ) ;
    
    TechniqueHolder tech = TechniqueHolder ( new Technique() ) ;
	tech -> setName ("Default") ;
    tech -> setExclusive (false) ;
    tech -> setViewport ( Viewport(1.0f, 1.0f, 1.0f, 1.0f) ) ;
    tech -> setCamera ( ResourceManager::Get().getCameraManager()->findFirst("Default") ) ;
    iTechniques.push_back(tech) ;
    addListener(EventProceederUser(tech));
	
	iInitialized = true ;
}

bool RenderSceneManager::isInitialized() const 
{
	GreAutolock ; return iInitialized ;
}

RenderSceneUser RenderSceneManager::load(const RenderSceneHolder &holder)
{
	GreAutolock ;
	
    if ( !holder.isInvalid() )
    {
        GreAutolock ;
        
        {
            RenderSceneHolder tmp = findHolder ( holder->getIdentifier() ) ;
            
            if ( !tmp.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "RenderScene '" << holder->getName() << "' already registered." << Gre::gendl;
                GreDebugPretty() << "  Resource Identifier was : " << holder->getIdentifier() << "." << Gre::gendl ;
#endif
                return RenderSceneUser ( nullptr );
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "RenderScene '" << holder->getName() << "' registered." << Gre::gendl;
#endif
        
        iHolders.add(holder);
        addListener(EventProceederUser(holder));
        return holder;
    }
    
    return RenderSceneUser ( nullptr );
}

RenderSceneUser RenderSceneManager::load ( const std::string & name , const RenderSceneOptions & options )
{
	GreAutolock ;
	
#ifdef GreIsDebugMode
    if ( name.empty() ) {
        GreDebug("[WARN] Initialized RenderScene with no name.") << Gre::gendl;
    }
#endif

	if ( !isInitialized() ) {
#ifdef GreIsDebugMode
		GreDebug("[WARN] Not initialized RenderSceneManager when loading '") << name << "'." << gendl ;
#endif
		return RenderSceneUser ( nullptr ) ;
	}
    
    RenderSceneLoader* loader = nullptr ;
    auto it = options.find("Loader" );
    
    if ( it != options.end() ) {
        loader = findLoader (it->second.toString()) ;
    } else {
        loader = findLoader ("Default") ;
    }
    
    if ( !loader )
    {
#ifdef GreIsDebugMode
        GreDebug("[WARN] Can't find a loader for RenderScene '") << name << "'." << Gre::gendl;
#endif
        return RenderSceneUser ( nullptr ) ;
    }
    
    RenderSceneHolder scene = loader -> load(name, options) ;
    if ( scene.isInvalid() ) {
#ifdef GreIsDebugMode
        GreDebug("[WARN] Can't load RenderScene '") << name << "'." << Gre::gendl ;
#endif 
        return RenderSceneUser ( nullptr ) ;
    }
    
    std::string technique = options.find("Technique") == options.end() ? "Default" : options.at("Technique").toString() ;
    if ( technique.empty() ) technique = "Default" ;
	
	TechniqueUser tech = findTechnique(technique);
    scene -> setTechnique ( tech ) ;
    
    iHolders.push_back(scene);
    addListener(EventProceederUser(scene));
    
#ifdef GreIsDebugMode
    GreDebug("[INFO] RenderScene '") << name << "' registered." << Gre::gendl ;
#endif
    
    return scene ;
}

void RenderSceneManager::drawScenes( const EventHolder& e ) const
{
    GreAutolock ;
    
    for ( auto it : iHolders )
    {
        if ( !it.isInvalid() )
        {
            it->draw ( e ) ;
        }
    }
}

TechniqueUser RenderSceneManager::findTechnique ( const std::string & name )
{
	GreAutolock ;
	
    for ( auto tech : iTechniques ) {
        if ( !tech.isInvalid() ) {
            if ( tech->getName() == name ) return TechniqueUser (tech) ;
        }
    }
    
    return TechniqueUser ( nullptr ) ;
}

void RenderSceneManager::onUpdateEvent ( const UpdateEvent& e )
{
    
}

GreEndNamespace
