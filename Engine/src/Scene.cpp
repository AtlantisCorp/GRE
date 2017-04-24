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
    iNodeType = RenderNodeType::Light ;
    iLight = new Light ( name + ".light" ) ;
    addFilteredListener ( EventProceederUser(iLight) , { EventType::Update } ) ;
}

LightRenderNode::LightRenderNode ( const LightHolder& light , const std::string& name )
: RenderNode ( name ) , iLight(light)
{

}

LightRenderNode::~LightRenderNode() noexcept ( false )
{

}

const LightHolder& LightRenderNode::getLight() const
{
    GreAutolock ; return iLight ;
}

LightHolder& LightRenderNode::getLight()
{
    GreAutolock ; return iLight ;
}

void LightRenderNode::setLight(const Gre::LightHolder &rhs)
{
    GreAutolock ; iLight = rhs ;
}

const CameraHolder & LightRenderNode::getLightCamera () const
{
    GreAutolock ; return iLightCamera ;
}

CameraHolder & LightRenderNode::getLightCamera ()
{
    GreAutolock ; return iLightCamera ;
}

void LightRenderNode::use ( const TechniqueHolder & technique , bool prepareforrender ) const
{
    GreAutolock ;

    if ( technique.isInvalid() || iLight.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Computes the light's name.

    TechniqueParam lightalias = technique -> getNextLightAlias () ;
    std::string lightname = technique -> getAlias ( lightalias ) ;

    //////////////////////////////////////////////////////////////////////
    // Sends values depending on what we find.

    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightAmbient ,
        HdwProgVarType::Float3 , iLight -> getAmbient().toFloat3() );
    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightDiffuse ,
        HdwProgVarType::Float3 , iLight -> getDiffuse().toFloat3() );
    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightSpecular ,
        HdwProgVarType::Float3 , iLight -> getSpecular().toFloat3() );

    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightPosition ,
        HdwProgVarType::Float3 , iLight -> getPosition() ) ;
    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightDirection ,
        HdwProgVarType::Float3 , iLight -> getDirection() ) ;

    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightAttCst ,
        HdwProgVarType::Float1 , iLight -> getAttenuationCst() ) ;
    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightAttLine ,
        HdwProgVarType::Float1 , iLight -> getAttenuationLinear() ) ;
    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightAttQuad ,
        HdwProgVarType::Float1 , iLight -> getAttenuationQuad() ) ;

    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightSpotAngle ,
        HdwProgVarType::Float1 , iLight -> getAngle () ) ;
    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightSpotExposition ,
        HdwProgVarType::Float1 , iLight -> getExposition() ) ;

    //////////////////////////////////////////////////////////////////////
    // Sends the texture if this light node holds one. Also , use the camera
    // projection view matrix if possible.

    if ( !iShadowTexture.isInvalid() )
    technique -> setAliasedTextureStruct ( lightalias , TechniqueParam::LightTexShadow , iShadowTexture ) ;

    if ( !iLightCamera.isInvalid() )
    technique -> setAliasedParameterStructValue ( lightalias , TechniqueParam::LightShadowMatrix ,
            HdwProgVarType::Matrix4 , iLightCamera -> getProjectionViewMatrix() ) ;

    //////////////////////////////////////////////////////////////////////
    // Don't forget to call the parent's use.

    if ( prepareforrender )
    RenderNode::use ( technique ) ;
}

void LightRenderNode::loadLightCamera ()
{
    GreAutolock ;

    if ( iLight.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Tries to create a camera for this light.

    iLightCamera = ResourceManager::Get() -> getCameraManager() -> loadCamera ( getName() + ".lightcamera" ) ;

    if ( !iLightCamera.isInvalid() )
    {
        //////////////////////////////////////////////////////////////////////
        // Initialize Camera values.

        iLightCamera -> setPosition ( iLight -> getPosition() ) ;
        iLightCamera -> setDirection ( iLight -> getDirection() ) ;

        if ( iLight -> getType() == LightType::Directionnal )
        {
            Frustrum ortho ({-10.0f , 10.0f , -10.0f , 10.0f} , 0.01f , 100.0f);
            iLightCamera -> setFrustrum(ortho) ;
        }

        else
        {
            Frustrum proj ( 60.0f , 16/9 , 0.01f , 100.0f ) ;
            iLightCamera -> setFrustrum(proj) ;
        }

        //////////////////////////////////////////////////////////////////////
        // Registers Camera as a light listener.

        std::vector < EventType > filters ;
        filters.push_back ( EventType::PositionChanged ) ;
        filters.push_back ( EventType::DirectionChanged ) ;
        iLight -> addFilteredListener ( EventProceederUser(iLightCamera) , filters ) ;
    }
}

void LightRenderNode::loadShadowTexture ( uint32_t width , uint32_t height )
{
    GreAutolock ;

    if ( iLight.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Tries to create a shadow texture for the light.
    
    TextureManagerHolder tm = ResourceManager::Get() -> getTextureManager() ;
    
    if ( tm.isInvalid() )
    {
        GreDebug( "[WARN] Trying to create Shadow Texture but TextureManager is not loaded." ) << gendl ;
        return ;
    }
    
    iShadowTexture = tm -> loadFromNewPixelBuffer (getName() + ".shadowtex" ,
                                                   width , height , 0 ,
                                                   PixelFormat::DepthComponent ,
                                                   InternalPixelFormat::DepthComponent ,
                                                   PixelType::Float ,
                                                   TextureType::Texture2D ,
                                                   sizeof(float) ) ;
    
    if ( iShadowTexture.isInvalid() )
    GreDebug( "[WARN] Error creating Shadow Texture '" ) << getName() + ".shadowtex" << "'." << gendl ;
}

bool LightRenderNode::isLightVisibleFrom ( const CameraHolder & camera ) const
{
    GreAutolock ;

    if ( camera.isInvalid() )
    return false ;

    //////////////////////////////////////////////////////////////////////
    // Computes intersection between light and camera frustrum.

    bool value = isVisible() ;

    if ( value && !iLight.isInvalid() )
        value = iLight -> intersectFrustrum ( camera -> getFrustrum() ) != IntersectionResult::Outside ;

    return value ;
}

void LightRenderNode::onPositionChangedEvent(const Gre::PositionChangedEvent &e)
{
    GreAutolock ; iLight->setPosition ( e.Position ) ;
    RenderNode::onPositionChangedEvent(e) ;
}

void LightRenderNode::onDirectionChangedEvent(const Gre::DirectionChangedEvent &e)
{
    GreAutolock ; iLight->setDirection( e.Direction ) ;
    RenderNode::onDirectionChangedEvent(e) ;
}

// ---------------------------------------------------------------------------------------------------

RenderScene::RenderScene(const std::string& name, const RenderSceneOptions& options)
: Gre::Renderable(name) , iRootNode(nullptr)
{
    setTransmitBehaviour(EventProceederTransmitBehaviour::SendsBefore);
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
        std::vector < EventType > filters ;
        filters.push_back(EventType::Update) ;
        filters.push_back(EventType::RenderScenePreRender) ;
        filters.push_back(EventType::RenderScenePostRender) ;

        Resource::addFilteredListener( EventProceederUser(iRootNode) , filters );
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

LightRenderNodeHolder RenderScene::createLightNode(const Gre::LightHolder &light, const std::string& name) const
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

const RenderNodeHolderList & RenderScene::getNodesForCamera ( const CameraHolder & camera ) const
{
    GreAutolock ;

    auto it = iNodesByCamera.find(camera) ;
    if ( it == iNodesByCamera.end() ) return RenderNodeHolderList::Empty ;

    return it -> second ;
}

const std::list<LightRenderNodeHolder> & RenderScene::getActivatedLightsForCamera ( const CameraHolder & camera ) const
{
    GreAutolock ; return iLightNodesByCamera.at(camera) ;
}

void RenderScene::addCamera(const CameraHolder &camera)
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Adds a Camera to the Scene. This object will updates the nodes list
    // available to it. (i.e. visible) Notes the 'clear()' function called
    // here is just to ensure the '[]' operator is called and the entry for
    // the camera is created.

    if ( !camera.isInvalid() )
    {
        iLightNodesByCamera[camera] = std::list < LightRenderNodeHolder > () ;
        iNodesByCamera[camera] = RenderNodeHolderList () ;
    }
}

const CameraHolder RenderScene::getCamera(const std::string &name) const
{
    GreAutolock ;

    for ( auto it : iNodesByCamera )
    {
        if ( !it.first.isInvalid() )
        {
            if ( it.first->getName() == name )
            {
                return it.first ;
            }
        }
    }

    return CameraHolder ( nullptr ) ;
}

void RenderScene::onUpdateEvent(const Gre::UpdateEvent &e)
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Converts the elapsed time in milliseconds.

    float milliseconds = e.elapsedTime.count() * 1000.0f ;

    //////////////////////////////////////////////////////////////////////
    // This function needs to updates the visible lights , and computes the
    // nodes visible by the camera.

    for ( auto it = iLightNodesByCamera.begin() ; it != iLightNodesByCamera.end() ; it++ )
    (*it).second = iComputeLightNodes ( it->first , milliseconds ) ;

    //////////////////////////////////////////////////////////////////////
    // Calls the 'iComputeNodes' for each camera. This function should returns
    // a valid list of nodes visible by the camera.

    for ( auto it = iNodesByCamera.begin() ; it != iNodesByCamera.end() ; it++ )
    it->second = iComputeNodes ( it->first , milliseconds ) ;
}

std::list < LightRenderNodeHolder > RenderScene::iComputeLightNodes(const CameraHolder &camera, float elapsed) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Traverse every light nodes registered and make a list of those visible
    // by the camera given.

    std::list < LightRenderNodeHolder > nodes ;

    for ( auto node : iLightNodes )
    if ( node -> isLightVisibleFrom(camera) ) nodes.push_back(node) ;

    return nodes ;
}

RenderNodeHolderList RenderScene::iComputeNodes(const CameraHolder &camera, float elapsed) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Constructs a list of nodes visible by the camera. Notes those nodes
    // must not be treated as subtree's root nodes when displaying them.

    RenderNodeHolderList nodes ;

    if ( !iRootNode.isInvalid() )
    iRootNode -> iComputeChildren ( nodes , camera , elapsed ) ;

    return nodes ;
}

/*
void RenderScene::updateNode ( const RenderNodeHolder & node )
{
    if ( !node.isInvalid() )
    {

        if ( node -> getNodeType() == RenderNodeType::Light )
        {
            for ( auto it = iLightNodesByCamera.begin() ; it != iLightNodesByCamera.end() ; it++ )
            {
                if ( !it->first.isInvalid() )
                {
                    if ( node->isVisible(it->first) )
                    {
                        auto it2 = std::find(it->second.begin(), it->second.end(), LightRenderNodeHolder(node)) ;
                        if ( it2 == it->second.end() ) it->second.emplace_back ( node ) ;
                    }
                }
            }
        }

        for ( auto it = iNodesByCamera.begin() ; it != iNodesByCamera.end() ; it++ )
        {
            if ( !it->first.isInvalid() )
            {
                if ( node->isVisible(it->first) )
                {
                    if ( node -> isRenderable () )
                    {
                        auto it2 = std::find(it->second.begin(), it->second.end(), node) ;
                        if ( it2 == it->second.end() ) it->second.emplace_back ( node ) ;
                    }

                    for ( auto child : node -> getChildren() )
                    {
                        updateNode ( child ) ;
                    }
                }

                else
                {

                }
            }
        }
    }
}
 */

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
        loader = findLoader (it->second.to<std::string>()) ;
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

    bool sendupdate = true ;
    it = options.find ( "SendUpdate" ) ;
    if ( it != options.end() ) sendupdate = it -> second .to<bool>() ;

    if ( sendupdate ) {
        addFilteredListener(EventProceederUser(scene.getObject()), { EventType::Update });
    }

    iHolders.push_back(scene);

#ifdef GreIsDebugMode
    GreDebug("[INFO] RenderScene '") << name << "' registered." << Gre::gendl ;
#endif

    return scene ;
}

GreEndNamespace
