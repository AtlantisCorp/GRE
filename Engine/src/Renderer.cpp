//////////////////////////////////////////////////////////////////////
//
//  Renderer.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 08/11/2015.
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

#include "Renderer.h"
#include "ResourceManager.h"
#include "Pass.h"
#include "RenderContext.h"

#include "Window.h"
#include "RenderingQuery.h"

GreBeginNamespace

typedef std::chrono::high_resolution_clock Clock;

Renderer::Renderer (const std::string& name, const RendererOptions& options)
: Gre::Resource( name )
, iDefaultProgram(nullptr) , iDefaultTexturedProgram(nullptr)
, iDefaultMaterial(nullptr)
, iDefaultOrthoProjectionMatrix(0) , iDefaultViewMatrix(0) , iDefaultModelMatrix(0)
, iFeatures( ) , iInstalled(false)
{
    
}

Renderer::~Renderer() noexcept ( false )
{
    
}

void Renderer::draw(const Gre::RenderingQuery &query) const
{
    GreAutolock ;
    
    _setViewport ( query.getViewport() ) ;
    setHardwareProgram ( query.getHardwareProgram() ) ;
    setCamera ( query.getCamera(), query.getHardwareProgram() ) ;
    
    for ( const RenderNodeHolder& node : query.getRenderedNodes() )
    {
        if ( node.isInvalid() )
            continue ;
        
        
        if ( node->isRenderable() && node->isVisible(query.getCamera()) )
        {
            setNode ( node , query.getHardwareProgram() ) ;
            _drawNodeMesh ( node->getMesh() , query.getHardwareProgram() ) ;
        }
        
        postNode ( node , query.getCamera() , query.getHardwareProgram() ) ;
    }
}

void Renderer::preRender ( const Color& clearcolor ) const
{
    _setClearColor(clearcolor) ;
    _preRender () ;
}

void Renderer::postRender () const
{
    _postRender () ;
}

void Renderer::setHardwareProgram ( const HardwareProgramUser& program ) const
{
    HardwareProgramHolder holder = program.lock() ;
    
    if ( holder.isInvalid () )
    {
        // No HardwarePrograms is set . We can use the default HardwareProgram , if this is the Renderer's
        // behaviour .
        if ( hasFeature(RendererFeature::LoadDefaultProgram) && !iDefaultProgram.isInvalid() )
        {
            iDefaultProgram.lock() -> use() ;
        }
    }
    
    else
    {
        holder->use();
    }
}

void Renderer::setCamera(const CameraUser &camera, const HardwareProgramUser &program) const
{
    CameraHolder camerah = camera.lock() ;
    HardwareProgramHolder programh = program.isInvalid() ? iDefaultProgram.lock() : program.lock() ;
    
    Matrix4 ProjectionMatrix ;
    Matrix4 ViewMatrix ;
    
    if ( !camerah.isInvalid() )
    {
        ProjectionMatrix = camerah->getProjectionMatrix() ;
        ViewMatrix = camerah->getViewMatrix() ;
    }
    
    HardwareProgramVariable iProjectionMatrix ;
    iProjectionMatrix.name = "iProjectionMatrix" ;
    iProjectionMatrix.type = HdwProgVarType::Mat4 ;
    iProjectionMatrix.value.mat4 = ProjectionMatrix ;
    programh->setVariable ( iProjectionMatrix ) ;
    
    HardwareProgramVariable iViewMatrix ;
    iViewMatrix.name = "iViewMatrix" ;
    iViewMatrix.type = HdwProgVarType::Mat4 ;
    iViewMatrix.value.mat4 = ViewMatrix ;
    programh->setVariable ( iViewMatrix ) ;
    
    _setCamera ( ProjectionMatrix , ViewMatrix ) ;
}

void Renderer::setNode ( const RenderNodeHolder& node , const HardwareProgramUser& programu ) const
{
    HardwareProgramHolder program = programu.isInvalid() ? iDefaultProgram.lock() : programu.lock() ;
    
    // Sets the 'iModelMatrix' .
    
    const Matrix4 & ModelMatrix = node->getModelMatrix () ;
    
    HardwareProgramVariable iModelMatrix ;
    iModelMatrix.name = "iModelMatrix" ;
    iModelMatrix.type = HdwProgVarType::Mat4 ;
    iModelMatrix.value.mat4 = ModelMatrix ;
    program->setVariable ( iModelMatrix ) ;
    
    // Configure the node's material .
    
    MaterialHolder material = node->getMaterial() ;
    
    if ( !material.isInvalid() )
    {
        material->configureProgram(program);
        material->bindTextures();
    }
    
    else if ( hasFeature(RendererFeature::LoadDefaultMaterial) && !iDefaultMaterial.isInvalid() )
    {
        iDefaultMaterial.lock() ->configureProgram(program);
        iDefaultMaterial.lock() ->bindTextures();
    }
    
    _setNode(node);
}

void Renderer::postNode ( const RenderNodeHolder & node , const CameraUser& camera , const HardwareProgramUser& program ) const
{
    if ( node.isInvalid() ) return ;
    if ( node->hasChildren() )
    {
        
        if ( camera.isInvalid() || program.isInvalid() )
            return ;
        
        for ( const RenderNodeHolder& child : node->getChildren() )
        {
            if ( child.isInvalid() )
                continue ;
            
            
            if ( child->isRenderable() && child->isVisible(camera) )
            {
                setNode ( child , program ) ;
                _drawNodeMesh ( child->getMesh() , program ) ;
            }
            
            postNode ( child , camera , program ) ;
        }
    }
    
    _postNode(node, camera, program);
}

void Renderer::installManagers ()
{
    GreAutolock ;
    
    iInstalled = true ;
    MeshManagerHolder meshmanager = iCreateMeshManager () ;
    
    if ( meshmanager.isInvalid() )
    {
        iInstalled = false ;
#ifdef GreIsDebugMode
        GreDebugPretty () << "No Mesh Manager can be installed." << Gre::gendl ;
#endif
    }
    else
    {
        if ( ResourceManager::Get().getMeshManager().isInvalid() )
        {
            ResourceManager::Get().setMeshManager ( meshmanager ) ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Mesh Manager installed." << Gre::gendl ;
#endif
        }
        else
        {
            iInstalled = false ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Mesh Manager already present and cannot be installed." << Gre::gendl ;
#endif
        }
    }
    
    TextureManagerHolder texturemanager = iCreateTextureManager () ;
    
    if ( texturemanager.isInvalid() )
    {
        iInstalled = false ;
#ifdef GreIsDebugMode
        GreDebugPretty () << "No Texture Manager can be installed." << Gre::gendl ;
#endif
    }
    else
    {
        if ( ResourceManager::Get().getTextureManager().isInvalid() )
        {
            ResourceManager::Get().setTextureManager ( texturemanager ) ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Texture Manager installed." << Gre::gendl ;
#endif
        }
        else
        {
            iInstalled = false ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Texture Manager already present and cannot be installed." << Gre::gendl ;
#endif
        }
    }
    
    HardwareProgramManagerHolder pmanager = iCreateProgramManager () ;
    
    if ( pmanager.isInvalid() )
    {
        iInstalled = false ;
#ifdef GreIsDebugMode
        GreDebugPretty () << "No Program Manager can be installed." << Gre::gendl ;
#endif
    }
    else
    {
        if ( ResourceManager::Get().getHardwareProgramManager().isInvalid() )
        {
            ResourceManager::Get().setHardwareProgramManager ( pmanager ) ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Program Manager installed." << Gre::gendl ;
#endif
        }
        else
        {
            iInstalled = false ;
#ifdef GreIsDebugMode
            GreDebugPretty () << "Program Manager already present and cannot be installed." << Gre::gendl ;
#endif
        }
    }
}

void Renderer::unload ( )
{
    GreAutolock ;
    
    iDefaultProgram.clear() ;
    iDefaultTexturedProgram.clear() ;
    iDefaultOrthoProjectionMatrix = Matrix4 (0) ;
    iDefaultViewMatrix = Matrix4 (0) ;
    iDefaultModelMatrix = Matrix4 (0) ;
    iFeatures.clear() ;
    
    if ( iInstalled )
    {
        ResourceManager::Get() .setMeshManager ( MeshManagerHolder(nullptr) ) ;
        ResourceManager::Get() .setTextureManager ( TextureManagerHolder(nullptr) ) ;
        ResourceManager::Get() .setHardwareProgramManager ( HardwareProgramManagerHolder(nullptr) ) ;
        iInstalled = false ;
    }
}

bool Renderer::isInstalled () const
{
    GreAutolock ; return iInstalled ;
}

bool Renderer::hasFeature(const RendererFeature& feature) const
{
    GreAutolock ;
    
    for ( auto& f : iFeatures )
    {
        if ( f == feature )
            return true;
    }
    
    return false;
}

void Renderer::setFeature ( const RendererFeature& feature )
{
    if ( !hasFeature(feature) )
    {
        GreAutolock ;
        iFeatures.push_back(feature);
    }
}

void Renderer::removeFeature ( const RendererFeature& feature )
{
    GreAutolock ;
    
    for ( auto it = iFeatures.begin() ; it != iFeatures.end() ; it++ )
    {
        if ( (*it) == feature )
        {
            iFeatures.erase(it);
            break;
        }
    }
}

const std::vector < RendererFeature > & Renderer::getFeatures () const
{
    GreAutolock ; return iFeatures;
}

// ---------------------------------------------------------------------------------------------------

RendererLoader::RendererLoader()
{
    
}

RendererLoader::~RendererLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

RendererManager::RendererManager( const std::string& name )
: SpecializedResourceManager<Gre::Renderer, Gre::RendererLoader>(name)
{
    
}

RendererManager::~RendererManager() noexcept ( false )
{
    
}

RendererUser RendererManager::load(const std::string &name, const Gre::RendererOptions &options)
{
    GreAutolock ;
    
    if ( !name.empty() )
    {
        RendererUser tmp = findFirst(name);
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Renderer Resource '" << name << "' already loaded." << Gre::gendl;
#endif
            return RendererUser ( nullptr );
        }
        
        for ( auto it = iLoaders.getLoaders().begin(); it != iLoaders.getLoaders().end(); it++ )
        {
            auto loader = it->second;
            
            if ( loader )
            {
                if ( loader->isCompatible(options) )
                {
                    RendererHolder rholder = loader->load(name, options);
                    
                    if ( rholder.isInvalid() )
                    {
#ifdef GreIsDebugMode
                        GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by loader '" << it->first << "'." << Gre::gendl;
#endif
                    }
                    
                    else
                    {
                        addListener( EventProceederUser(rholder) ) ;
                        iHolders.push_back(rholder);
                        return RendererUser ( rholder );
                    }
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by any installed Loader." << Gre::gendl;
#endif
        return RendererUser ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return RendererUser ( nullptr );
    }
}

GreEndNamespace
