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

GreBeginNamespace

RenderScene::RenderScene(const std::string& name)
: Gre::Resource(name) , iRootNode(nullptr)
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
        
        drawTechnique(iTechnique, elapsed) ;
        
        iRenderTarget -> swapBuffers () ;
        iRenderTarget -> unbind () ;
    }
}

void RenderScene::drawTechnique( const TechniqueHolder& technique , const EventHolder &elapsed) const
{
    for ( auto & pass : technique->getPasses() )
    {
        RenderingQuery query ;
        query.setRenderScene ( RenderSceneUser(this) ) ;
        query.setRenderPass ( pass ) ;
        query.setCamera ( technique->getCamera() ) ;
        query.setHardwareProgram ( pass->getHardwareProgram() ) ;
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
        
        rholder -> draw ( query ) ;
    }
    
    if ( technique->hasSubtechniques() )
    {
        for ( const TechniqueHolder & tech : technique->getSubtechniques() )
        {
            drawTechnique(tech, elapsed) ;
        }
    }
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

RenderSceneHolder RenderSceneLoader::load ( const std::string &name , const std::string& filepath ) const
{
    return RenderSceneHolder ( new RenderScene(name) );
}

// ---------------------------------------------------------------------------------------------------

RenderSceneManager::RenderSceneManager ( const std::string& name )
: SpecializedResourceManager<Gre::RenderScene, Gre::RenderSceneLoader>(name)
{
    GreAutolock ;
    
    iLoaders.registers( "Default" , new RenderSceneLoader() );
}

RenderSceneManager::~RenderSceneManager() noexcept ( false )
{
    
}

RenderSceneUser RenderSceneManager::load ( const std::string &name , const std::string &filename )
{
    if ( !name.empty() )
    {
        
        {
            GreAutolock ;
            
            RenderSceneHolder tmp = findFirstHolder ( name ) ;
            
            if ( !tmp.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "RenderScene '" << name << "' already registered." << Gre::gendl;
#endif
                return RenderSceneUser ( nullptr );
            }
        }
        
        if ( filename.empty() )
        {
            // Loads a RenderScene using default RenderSceneLoader.
            
            GreAutolock ;
            
            RenderSceneLoader* ptrloader = iLoaders.get( "Default" ) ;
            
            if ( ptrloader )
            {
                RenderSceneHolder holder = ptrloader->load(name);
                
                if ( holder.isInvalid() )
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "'holder' can't be initialized." << Gre::gendl;
#endif
                    return RenderSceneUser ( nullptr );
                }
                
                else
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "RenderScene '" << name << "' initialized." << Gre::gendl;
#endif
                    iHolders.add(holder);
                    addListener(EventProceederUser(holder));
                    return RenderSceneUser ( holder ) ;
                }
            }
            
            else
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "RenderSceneLoader 'Default' not found." << Gre::gendl;
#endif
                return RenderSceneUser ( nullptr );
            }
        }
        
        else
        {
            GreAutolock ;
            
            RenderSceneLoader * loader = iFindBestLoader(filename) ;
            
            if ( loader )
            {
                RenderSceneHolder holder = loader -> load( name , filename ) ;
                
                if ( !holder.isInvalid() )
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "RenderScene '" << name << "' initialized." << Gre::gendl;
#endif
                    iHolders.add(holder);
                    return RenderSceneUser ( holder ) ;
                }
                
                else
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "RenderScene '" << name << "' not initialized." << Gre::gendl;
#endif
                    return RenderSceneUser ( nullptr );
                }
            }
            
#ifdef GreIsDebugMode
            else
            {
                GreDebugPretty() << "Can't find a loader for file : '" << filename << "'." << Gre::gendl ;
            }
#endif
            
            return RenderSceneUser ( nullptr ) ;
        }
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "'name' is invalid." << Gre::gendl;
    }
#endif
    
    return RenderSceneUser ( nullptr );
}

RenderSceneUser RenderSceneManager::load(const RenderSceneHolder &holder)
{
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

GreEndNamespace
