//////////////////////////////////////////////////////////////////////
//
//  Scene.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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

GreBeginNamespace

RenderScenePrivate::RenderScenePrivate(const std::string& name)
: Gre::Resource(name)
, iRootNode(nullptr)
{
    // Add the default normal Pass with PassPurpose::First.
    createAndAddPass("Default", (PassNumber) PassPurpose::First);
}

RenderScenePrivate::~RenderScenePrivate()
{
    
}

const RenderNodeHolder& RenderScenePrivate::getRootNode() const
{
    return iRootNode;
}

const RenderNodeHolder& RenderScenePrivate::setRootNode(const RenderNodeHolder &rendernode)
{
    if ( !iRootNode.isInvalid() )
    {
        Resource::removeListener( iRootNode->getName() );
    }
    
    iRootNode = rendernode;
    
    if ( !iRootNode.isInvalid() )
    {
        Resource::addListener( RenderNode(iRootNode) );
    }
    
    return iRootNode;
}

RenderNodeHolder RenderScenePrivate::createNode() const
{
    return RenderNodeHolder ( new RenderNodePrivate() );
}

RenderNodeHolder RenderScenePrivate::createNodeWithName(const std::string &name) const
{
    return RenderNodeHolder ( new RenderNodePrivate(name) );
}

void RenderScenePrivate::addNode(const RenderNodeHolder &rendernode)
{
    if ( !iRootNode.isInvalid() )
    {
        iRootNode->addNode(rendernode);
    }
    
    else
    {
        iRootNode = rendernode;
    }
}

void RenderScenePrivate::removeNode(RenderNodeIdentifier identifier)
{
    if ( RenderNodePrivate::Identificator::IsValid(identifier) )
    {
        if ( !iRootNode.isInvalid() )
        {
            if ( iRootNode->getIdentifier() == identifier )
            {
                iRootNode.reset();
            }
            
            else
            {
                iRootNode->remove(identifier);
            }
        }
        
#ifdef GreIsDebugMode
        else
        {
            GreDebugPretty() << "'iRootNode' is invalid." << std::endl;
        }
#endif
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "'identifier' is invalid." << std::endl;
    }
#endif
}

RenderNodeHolder RenderScenePrivate::findNode(RenderNodeIdentifier identifier)
{
    if ( RenderNodePrivate::Identificator::IsValid(identifier) )
    {
        if ( !iRootNode.isInvalid() )
        {
            if ( iRootNode->getIdentifier() == identifier )
            {
                return iRootNode;
            }
            
            else
            {
                return iRootNode->find(identifier);
            }
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "'iRootNode' is invalid." << std::endl;
#endif
            return RenderNodeHolder ( nullptr );
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' is invalid." << std::endl;
#endif
        return RenderNodeHolder ( nullptr );
    }
}

const RenderNodeHolder RenderScenePrivate::findNode(RenderNodeIdentifier identifier) const
{
    if ( RenderNodePrivate::Identificator::IsValid(identifier) )
    {
        if ( !iRootNode.isInvalid() )
        {
            if ( iRootNode->getIdentifier() == identifier )
            {
                return iRootNode;
            }
            
            else
            {
                return iRootNode->find(identifier);
            }
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "'iRootNode' is invalid." << std::endl;
#endif
            return RenderNodeHolder ( nullptr );
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'identifier' is invalid." << std::endl;
#endif
        return RenderNodeHolder ( nullptr );
    }
}

RenderNodeHolderList RenderScenePrivate::findNodes(const CameraHolder &camera, const RenderSceneFilter& filter) const
{
    if ( !camera.isInvalid() )
    {
        if ( !iRootNode.isInvalid() )
        {
            RenderNodeHolderList ret;
            
            if ( camera->isVisible(iRootNode->getBoundingBox()) )
            {
                ret.add( iRootNode->getVisibleChildren(camera) );
                ret.add( iRootNode );
            }
            
            if ( !ret.empty() )
            {
                filter.sort(ret);
            }
            
            return ret;
        }
        
        else
        {
            return RenderNodeHolderList();
        }
    }
    
    else
    {
        return RenderNodeHolderList();
    }
}

RenderNodeHolderList RenderScenePrivate::findNodesForPass(PassNumber passnumber, const CameraHolder &camera, const RenderSceneFilter& filter) const
{
    if ( !camera.isInvalid() )
    {
        if ( !iRootNode.isInvalid() )
        {
            RenderNodeHolderList ret;
            
            if ( camera->isVisible(iRootNode->getBoundingBox()) )
            {
                ret.add( iRootNode->getVisibleChildren(camera) );
                ret.add( iRootNode );
            }
            
            if ( !ret.empty() )
            {
                PassHolder pass = getPassHolder(passnumber);
                
                if ( !pass.isInvalid() )
                {
                    for ( auto it = ret.begin(); it != ret.end(); it++ )
                    {
                        if ( !(*it).isInvalid() )
                        {
                            if ( !pass->isAcceptable((*it)->getIdentifier()) )
                            {
                                (*it).reset();
                            }
                        }
                    }
                }
                
                filter.sort(ret);
            }
            
            return ret;
        }
        
        else
        {
            return RenderNodeHolderList();
        }
    }
    
    else
    {
        return RenderNodeHolderList();
    }
}

Pass RenderScenePrivate::createAndAddPass(const std::string &name, const PassNumber &passNumber)
{
    if ( !name.empty() )
    {
        PassHolder tmp = getPassHolder(passNumber);
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Pass '" << name << "' is already registered for PassPurpose '" << passNumber << "'." << std::endl;
#endif
            return Pass::Null;
        }
        
        PassHolder newpass = PassHolder ( new PassPrivate(name, passNumber) );
        
        if ( !newpass.isInvalid() )
        {
            iPasses.add(newpass);
            return Pass ( newpass );
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Pass '" << name << "' can't be created." << std::endl;
#endif
            return Pass ( nullptr );
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return Pass ( nullptr );
    }
}

Pass RenderScenePrivate::addPass(const PassHolder &customPass)
{
    if ( !customPass.isInvalid() )
    {
        PassHolder tmp = getPassHolder(customPass->getPassNumber());
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Pass '" << customPass->getName() << "' is already registered for PassPurpose '" << customPass->getPassNumber() << "'." << std::endl;
#endif
            return Pass ( nullptr );
        }
        
        iPasses.add(customPass);
        return Pass ( customPass );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'customPass' is invalid." << std::endl;
#endif
        return Pass ( nullptr );
    }
}

Pass RenderScenePrivate::getPassByName(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( PassHolder& holder : iPasses )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Pass ( holder );
                }
            }
        }
        
        return Pass ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return Pass ( nullptr );
    }
}

Pass RenderScenePrivate::getPassByNumber(const PassNumber &passNumber)
{
    if ( passNumber > 0 )
    {
        for ( PassHolder& holder : iPasses )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getPassNumber() == passNumber )
                {
                    return Pass ( holder );
                }
            }
        }
        
        return Pass ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'passNumber' is invalid." << std::endl;
#endif
        return Pass ( nullptr );
    }
}

PassHolder RenderScenePrivate::getPassHolder(const PassNumber &passnumber)
{
    for ( PassHolder& holder : iPasses )
    {
        if ( !holder.isInvalid() )
        {
            if ( holder->getPassNumber() == passnumber )
            {
                return holder;
            }
        }
    }
    
    return PassHolder ( nullptr );
}

const PassHolder RenderScenePrivate::getPassHolder(const PassNumber &passnumber) const
{
    for ( const PassHolder& holder : iPasses )
    {
        if ( !holder.isInvalid() )
        {
            if ( holder->getPassNumber() == passnumber )
            {
                return holder;
            }
        }
    }
    
    return PassHolder ( nullptr );
}

void RenderScenePrivate::removePassByName(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( auto it = iPasses.begin(); it != iPasses.end(); it++ )
        {
            if ( !(*it).isInvalid() )
            {
                if ( (*it)->getName() == name )
                {
                    iPasses.erase(it);
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Pass '" << name << "' not found." << std::endl;
#endif
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "'name' is invalid." << std::endl;
    }
#endif
}

void RenderScenePrivate::removePassByNumber(const PassNumber &passNumber)
{
    if ( passNumber > 0 )
    {
        for ( auto it = iPasses.begin(); it != iPasses.end(); it++ )
        {
            if ( !(*it).isInvalid() )
            {
                if ( (*it)->getPassNumber() == passNumber )
                {
                    iPasses.erase(it);
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Pass '" << passNumber << "' not found." << std::endl;
#endif
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "'passNumber' is invalid." << std::endl;
    }
#endif
}

PassHolderList RenderScenePrivate::getActivePasses() const
{
    PassHolderList ret;
    
    for ( const PassHolder& holder : iPasses )
    {
        if ( !holder.isInvalid() )
        {
            if ( holder->isActivated() )
            {
                ret.add(holder);
            }
        }
    }
    
    return ret;
}

// ---------------------------------------------------------------------------------------------------

RenderScene::RenderScene(const RenderScenePrivate* pointer)
: Gre::ResourceUser(pointer)
, SpecializedResourceUser<Gre::RenderScenePrivate>(pointer)
{
    
}

RenderScene::RenderScene(const RenderSceneHolder& holder)
: Gre::ResourceUser(holder)
, SpecializedResourceUser<Gre::RenderScenePrivate>(holder)
{
    
}

RenderScene::RenderScene(const RenderScene& user)
: Gre::ResourceUser(user)
, SpecializedResourceUser<Gre::RenderScenePrivate>(user)
{
    
}

RenderScene::~RenderScene()
{
    
}

RenderScene RenderScene::Null = RenderScene ( nullptr );

// ---------------------------------------------------------------------------------------------------

RenderSceneLoader::RenderSceneLoader()
{
    
}

RenderSceneLoader::~RenderSceneLoader()
{
    
}

GreEndNamespace
