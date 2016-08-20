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

GreBeginNamespace

typedef std::chrono::high_resolution_clock Clock;

RendererPrivate::RendererPrivate (const std::string& name)
: Resource(name), iLastRenderContext(nullptr), iProgramManager(nullptr)
{
    
}

RendererPrivate::~RendererPrivate() noexcept ( false )
{
    
}

void RendererPrivate::drawRenderTarget(const Gre::RenderTarget &rendertarget)
{
    RenderTargetHolder holder = rendertarget.lock();
   
    if( !holder )
    {
#ifndef GreIsDebugMode
        GreDebugPretty() << "Drawing invalid RenderTarget." << std::endl;
#endif
    }
    
    else
    {
        if(holder->holdsRenderContext())
        {
            // If RenderTarget has a RenderContext, this means we should leave the last RenderContext
            // and replace it by this one.
            iLastRenderContext = holder->getRenderContext();
        }
        
        if( iLastRenderContext )
        {
            iLastRenderContext->bind();
            
            // At least the RenderContext is binded. Now, we should try to see if we have a special framebuffer
            // to draw to.
            
            if(holder->holdsFramebuffer())
            {
                // Replace the null special framebuffer by this one.
                iFrameContext.SpecialFramebuffer = holder->getFramebuffer();
            }
            
            else
            {
                // Ensure the special framebuffer is null.
                iFrameContext.SpecialFramebuffer = RenderFramebufferHolder(nullptr);
            }
            
            // Now we should have the Scene we should draw with this RenderTarget.
            SceneManagerHolder smholder = holder->getSelectedScene().lock();
            
            if( smholder )
            {
                // Sets the SceneManager to the FrameContext.
                iFrameContext.RenderedScene = smholder;
                
                // Now Render the selected SceneManager.
                drawSceneManager(iFrameContext.RenderedScene);
                
                // After rendering the SceneManager, just unbind the RenderContext.
                iLastRenderContext->unbind();
            }
            
            else
            {
                // If we don't have any SceneManager to render, abort.
#ifdef GreIsDebugMode
                GreDebugPretty() << "No SceneManager selected for RenderTarget '" << holder->getName() << "'." << std::endl;
#endif
                
                iLastRenderContext->unbind();
                return;
            }
        }
        
        else
        {
            // If we don't have any RenderContext, abort.
#ifdef GreIsDebugMode
            GreDebugPretty() << "No RenderContext selected for RenderTarget '" << holder->getName() << "'." << std::endl;
#endif
            return;
        }
    }
}

void RendererPrivate::drawSceneManager(const Gre::SceneManager &scenemanager)
{
    // Ensure everything is alright.
    if( iLastRenderContext && iFrameContext.RenderedScene == scenemanager.lock() )
    {
        PassHolderList passlist = iFrameContext.RenderedScene->getActivePasses();
        
        if( passlist.empty() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No Pass selected to render '" << iFrameContext.RenderedScene->getName() << "'." << std::endl;
#endif
            return;
        }
        
        // We have to draw every Pass objects. First, prepare the Framebuffer objects.
        RenderFramebufferHolderList fbolist = getFramebuffers((int)passlist.size());
        
        if( fbolist.size() == 0 )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Coudn't get " << passlist.size() << " Framebuffers." << std::endl;
#endif
            return;
        }
        
        // TODO : Handle multiple viewports. For now we only use the default viewport from the RenderContext.
        Viewport view = iLastRenderContext->getDefaultViewport();
        iFrameContext.RenderedViewport = view;
        
        // Set the FBO's size to those from the Pass object, in percent. (As default value is '1.0f', one not
        // using this option can have Framebuffers filling the whole Viewport.
        
        // For now, we always use Texture objects as Framebuffers. We render to texture for each Pass.
        // Rendering of each Pass is done by rendering the Scene for each Pass to a texture, then using multitexturing
        // in order to blend each Pass.
        // If the number of Texture that can be used at the same time is too high, we should do the last pass
        // with other Framebuffers (i.e. if limit is 32 and we have 60 Pass, we can render the 32 first to a texture,
        // then use this texture + the 28 others to draw the final result).
        
        for(auto fbo : fbolist)
        {
            if( fbo )
            {
                if( fbo->getAttachementSurface(RenderFramebufferAttachement::Color) != view.getSurface() )
                {
                    TextureHolder texholder = createEmptyTexture(view.getSurface().height, view.getSurface().width);
                    
                    if( !texholder )
                    {
#ifdef GreIsDebugMode
                        GreDebugPretty() << "Can't create Texture for Framebuffer object '" << fbo->getName() << "'." << std::endl;
#endif
                        return;
                    }
                    
                    fbo->setAttachement(RenderFramebufferAttachement::Color, texholder);
                }
            }
        }
        
        // After updating the Framebuffers to be prepared, we can draw the pass list.
        drawPassList(passlist, fbolist);
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Something occured bad." << std::endl;
    }
#endif
}

void RendererPrivate::drawPassList(const PassHolderList& passlist, RenderFramebufferHolderList& fbolist)
{
    if( passlist.empty() || fbolist.empty() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "No Pass or problem with the Framebuffer's list." << std::endl;
#endif
        return;
    }
    
    if( fbolist.size() < passlist.size() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Not enough RenderFramebuffer's to draw each Pass. (" << fbolist.size() << ":" << passlist.size() << ")." << std::endl;
#endif
        return;
    }
    
    auto itfbo = fbolist.begin();
    for( auto passit = passlist.begin(); passit != passlist.end(); passit++ )
    {
        auto passholder = *passit;
        auto fboholder = *itfbo;
        
        // Draw Pass with its Framebuffer.
        drawPassWithFramebuffer(passholder, fboholder);
        itfbo++;
    }
    
    // After drawing every Pass into a Framebuffer, we should blend the Framebuffers.
    drawFramebufferList(fbolist);
}

void RendererPrivate::drawPassWithFramebuffer(const PassHolder& passholder, RenderFramebufferHolder& fboholder)
{
    // Ensure everything is okay
    if( !passholder || !fboholder )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "No pass or framebuffer to draw." << std::endl;
#endif
        return;
    }
    
    fboholder->bind();
    
    // In order to set Shaders, we must check that this one has a correct location output (at least 0).
    HardwareProgramHolder program = passholder->getHardwareProgram().lock();
    
    if( !program->hasOutputLocation(0) )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Incorrect output location for program '" << program->getName() << "'." << std::endl;
        GreDebugPretty() << "There should be at least an output for location '0' in your fragment shader." << std::endl;
#endif
    }
    
    else
    {
        // NOTES : In this phasis, the Camera object is not prepared to the Program. Why ? Because this is done
        // when updating the SceneManager and updating the HardwareProgramManager. The SceneManager will update
        // its current Camera, which will changes its position. The HardwareProgramManager will updates its Programs
        // with the global values "ProjectionMatrix" and "ViewMatrix" from the Camera.
        
        // Program has correct location, now bind it and configure the Camera.
        program->bind();
        
        // Now we are drawing to the correct Framebuffer, using the correct HardwareProgram. We still have to
        // get the Objects to draw.
        SceneNodeList nodes = iFrameContext.RenderedScene->getNodesFrom(iFrameContext.RenderedScene->getCurrentCamera());
        
        // The SceneNode's objects should be rendered now to the binded Framebuffer, accordingly to the given
        // HardwareProgram.
        drawSceneNodeList(nodes, program);
        
        program->unbind();
    }
    
    fboholder->unbind();
}

void RendererPrivate::drawSceneNodeList(SceneNodeList& nodes, HardwareProgramHolder& program)
{
    if( !nodes.empty() && program )
    {
        // We iterates to every Node's from the list given by the SceneManager to draw the
        // objects. Those Objects are 'placed' thanks to the 'ModelMatrix' uniform.
        // Notes the ViewMatrix and ProjectionMatrix are set by the Camera during Rendering.
        
        for(auto node : nodes)
        {
            Mesh mesh = node.getRenderable();
            
            if( mesh.isExpired() )
            {
                continue;
            }
            
            // Updates the Model Matrix.
            
            program->setUniformMat4("ModelMatrix", node.getModelMatrix());
            
            // Draw the Mesh normally.
            
            HardwareProgram tmpprog ( program );
            drawSimpleMesh(mesh, tmpprog);
        }
    }
}

void RendererPrivate::drawFramebufferList(RenderFramebufferHolderList& fbolist)
{
    if( !fbolist.empty() )
    {
        // Get the HardwareProgram to use.
        
        HardwareProgram multitexturing = getHardwareProgramManager().getProgram("Shader/Multitexturing");
        
        if ( multitexturing.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "'Shader/Multitexturing' Shader Program was not found in database." << std::endl;
#endif
            return;
        }
        
        // Bind the HardwareProgram in order to use it.
        
        multitexturing.bind();
        
        // The 'Gre/Multitexturing' HardwareProgram has the uniform 'uTextureCount' which specify
        // the number of textures binded. The textures must be binded from 0 to this number minus 1.
        
        HardwareProgramVariable textureCountVar;
        textureCountVar.name = "uTextureCount";
        textureCountVar.type = HdwProgVarType::Uint32;
        textureCountVar.value.uint32 = (uint32_t) fbolist.size();
        multitexturing.setUniform(textureCountVar);
        
        // Next, we bind every Textures to the multitexturing samplers objects.
        // Notes if the Texture object has a HardwareSampler, it will also bind it. The Texture object is
        // binded from the first to the last.
        
        // The Sampler's name in the Shader is as follow :
        // 'u' + Sampler's type to string + The Array index.
        
        int i = 0;
        for ( auto it = fbolist.begin(); it != fbolist.end(); it++ )
        {
            
            auto fbo = (*it);
            
            if ( fbo.isInvalid() )
            {
                continue;
            }
            
            // Use the Program to activate the Texture Unit.
            
            multitexturing.bindTextureUnit(i);
            
            // Bind the Framebuffer Texture.
            
            Texture iTexture = fbo->getTextureAttachement(RenderFramebufferAttachement::Color);
            
            if ( iTexture.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Failed accessing Framebuffer Color Texture ('" << i << "')." << std::endl;
#endif
                continue;
            }
            
            iTexture.bind();
            
            // Check if the Texture has a Sampler to bind.
            
            if ( iTexture.hasHardwareSamplerActivated() )
            {
                // This will bind the HardwareSampler to the same texture unit the Texture was binded to.
                iTexture.getHardwareSampler().bind(i);
            }
            
            // Create the uniform and set it to the shader.
            
            HardwareProgramVariable textureVar;
            textureVar.name = std::string("u") + TextureTypeToString(iTexture.getType());
            textureVar.isArrayElement = true;
            textureVar.elementNumber = i;
            textureVar.type = TextureTypeToHdwProgType(iTexture.getType());
            textureVar.value.textureunit = i;
            multitexturing.setUniform(textureVar);
    
            // Increment the Texture Unit counter.
            
            i += 1;
        }
        
        // Now we get the Plane's Mesh.
        
        Mesh myplane = iMeshManager.createRectangle(iFrameContext.RenderedViewport.getSurface());
        
        if ( myplane.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "'Mesh/Rectangle' was not found in database." << std::endl;
#endif
        }
        
        else
        {
            // Draw the Plane.
            
            drawSimpleMesh(myplane, multitexturing);
        }
            
        // Unbind the HardwareProgram and the Texture's units.
        
        multitexturing.unbind();
        
        i = 0;
        for(auto it = fbolist.begin(); it != fbolist.end(); it++)
        {
            // Unbind the Texture unit.
            
            Texture iTexture = (*it)->getTextureAttachement(RenderFramebufferAttachement::Color);
            
            if ( iTexture.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Failed accessing Framebuffer Color Texture ('" << i << "')." << std::endl;
#endif
                continue;
            }
            
            iTexture.unbind();
            
            // Check if the Texture has a Sampler to unbind.
            
            if ( iTexture.hasHardwareSamplerActivated() )
            {
                // This will unbind the HardwareSampler to the same texture unit the Texture was binded to.
                iTexture.getHardwareSampler().unbind(i);
            }
            
            // Increment the Texture Unit counter.
            
            i += 1;
        }
    }
}

void RendererPrivate::drawSimpleMesh(Mesh& mesh, HardwareProgram& prog)
{
    if( mesh.isInvalid() )
    {
        return;
    }
    
    // Here we provided every informations necessary to draw the Mesh. The Program,
    // the VertexBuffer and the IndexBuffer.
    
    // If one of the Vertex/IndexBuffer is not found, it is replaced by one of its
    // Software clones, if it has one.
    // If no IndexBuffer is found, we just draw the VertexBuffer.
    
    HardwareProgramHolder program = prog.lock();
    HardwareVertexBuffer vbuf (nullptr);
    HardwareIndexBuffer ibuf (nullptr);
    
    if ( mesh.useHardwareBuffers() && !mesh.getVertexBuffer().isInvalid() )
    {
        vbuf = mesh.getVertexBuffer();
    }
    else
    {
        vbuf = mesh.getSoftwareVertexBuffer();
    }
    
    if ( mesh.useHardwareBuffers() && !mesh.getIndexBuffer().isInvalid() )
    {
        ibuf = mesh.getIndexBuffer();
    }
    else
    {
        ibuf = mesh.getSoftwareIndexBuffer();
    }
    
    if ( vbuf.isInvalid() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "No VertexBuffer found in Mesh '" << mesh.getName() << "'." << std::endl;
#endif
        return;
    }
    
    if ( ibuf.isInvalid() )
    {
        drawVertexBuffer( vbuf, program );
    }
    
    else
    {
        drawIndexBuffer( ibuf, vbuf, program );
    }
}

void RendererPrivate::drawVertexBuffer(const HardwareVertexBuffer& vbuf, const HardwareProgramHolder& program)
{
    // First, ask the HardwareProgram to binds every attributes in the VertexDescriptor.
    // Name from the VertexDescriptor are the same used in the Program.
    
    program->bindAttribsVertex(vbuf.getVertexDescriptor(), vbuf.getData());
    
    // Then, we bind the Material::Default and draw the VertexBuffer as Triangles.
    
    drawVertexBufferPrivate(vbuf, program);
}

void RendererPrivate::drawIndexBuffer(const HardwareIndexBuffer& ibuf, const HardwareVertexBuffer& vbuf, const HardwareProgramHolder& program)
{
    // First, ask the HardwareProgram to binds every attributes in the VertexDescriptor.
    // Name from the VertexDescriptor are the same used in the Program.
    
    program->bindAttribsVertex(vbuf.getVertexDescriptor(), vbuf.getData());
    
    // Then, draw the IndexBatchs in the Index Buffer. The IndexBuffer is constitued by
    // IndexBatch's, which contains Material objects. Notes for some API, the Indexes in
    // one HardwareIndexBuffer should have the same type.
    
    HardwareIndexBufferHolder bufholder = ibuf.lock();
    
    if ( !bufholder.isInvalid() )
    {
        bufholder->bind();
        
        for ( const IndexBatch& indexbatch : ibuf.getIndexBatches() )
        {
            const Material& material = indexbatch.getDescriptor().getMaterial();
            
            if ( !material.isInvalid() )
            {
                bindMaterial(material);
            }
            
#ifdef GreIsDebugMode
            else
            {
                GreDebugPretty() << "No material in IndexBatch for HardwareIndexBuffer '" << bufholder->getName() << "'." << std::endl;
            }
#endif
            
            drawIndexBufferPrivate(ibuf, vbuf, program);
            
            if ( !material.isInvalid() )
            {
                unbindMaterial(material);
            }
        }
    }
}

HardwareVertexBuffer RendererPrivate::createVertexBuffer()
{
    GreDebugFunctionNotImplemented();
    return HardwareVertexBuffer::Null;
}

HardwareIndexBuffer RendererPrivate::createIndexBuffer()
{
    GreDebugFunctionNotImplemented();
    return HardwareIndexBuffer::Null;
}

void RendererPrivate::loadMesh(Mesh& mesh, bool deleteCache)
{
    // Here we wants to create GPU - stored HardwareBuffer.
    
    MeshHolder meshholder = mesh.lock();
    
    bool deleteVertexBuffer = false;
    bool deleteIndexBuffer = false;
    
    if( !meshholder->getSoftwareVertexBuffer().isInvalid() )
    {
        SoftwareVertexBuffer softvbuf = meshholder->getSoftwareVertexBuffer();
        
        if ( softvbuf.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No SoftwareVertexBuffer in Mesh '" << meshholder->getName() << "'." << std::endl;
#endif
        }
        
        else
        {
            HardwareVertexBuffer gpuvbuf = createVertexBufferWithSize(meshholder->getName() + "/HdwVertexBuffer" , softvbuf.getSize());
            
            if ( gpuvbuf.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Can't create HardwareVertexBuffer for Mesh '" << meshholder->getName() << "'." << std::endl;
#endif
            }
            
            else
            {
                gpuvbuf.setVertexDescriptor(softvbuf.getVertexDescriptor());
                gpuvbuf.addData(softvbuf.getData(), softvbuf.getSize());
                meshholder->setVertexBuffer(gpuvbuf);
                
                if ( deleteCache )
                {
                    deleteVertexBuffer = true;
                }
            }
        }
    }
    
    if( !meshholder->getSoftwareIndexBuffer().isInvalid() )
    {
        SoftwareIndexBuffer softibuf = meshholder->getSoftwareIndexBuffer();
        
        if ( !softibuf.isInvalid() )
        {
            HardwareIndexBuffer gpuibuf = createIndexBuffer();
            
            if ( gpuibuf.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Can't create SoftwareIndexBuffer for Mesh '" << meshholder->getName() << "'." << std::endl;
#endif
            }
            
            else
            {
                // As SoftwareIndexBuffer stores the 'default' IndexBatch in the Batch's vector,
                // we can iterates through every IndexBatch.
                
                // Handles for IndexDescriptor should be done by the Hardware Index Buffer itself,
                // in ::addIndexBatch.
                
                for ( auto batch : softibuf.getIndexBatchVector() )
                {
                    gpuibuf.addIndexBatch(batch);
                }
                
                meshholder->setIndexBuffer(gpuibuf);
                
                if ( deleteCache )
                {
                    deleteIndexBuffer = true;
                }
            }
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No SoftwareIndexBuffer in Mesh '" << meshholder->getName() << "'." << std::endl;
#endif
        }
    }
    
    // Deleting cache should be done only when we are sure everything is loaded to the
    // GPU , so things must be done properly before deleting the cache.
    
    if(deleteCache)
    {
        meshholder->onNextEvent(EventType::Update , [&] (const Event& e) {
            
            if ( deleteVertexBuffer )
            {
                meshholder->getSoftwareVertexBuffer().reset();
            }
            
            if ( deleteIndexBuffer )
            {
                meshholder->getSoftwareIndexBuffer().reset();
            }
        });
    }
}

TextureHolder RendererPrivate::createEmptyTexture(int width, int height)
{
    GreDebugFunctionNotImplemented();
    return TextureHolder(nullptr);
}

Texture RendererPrivate::createTexture(const std::string &name, const std::string &file)
{
    GreDebugFunctionNotImplemented();
    return Texture::Null;
}

HardwareProgram RendererPrivate::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    if( iHardwareProgramManager )
    {
        HardwareProgram program = iHardwareProgramManager->createProgram(name, vertexShader, fragmentShader);
        if( program.isExpired() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Can't create HardwareProgram '" << name << "'." << std::endl;
#endif
        }
        
        return program;
    }
    
    return HardwareProgram(nullptr);
}

HardwareProgram RendererPrivate::createHardwareProgram(const std::string &name, const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    if( iHardwareProgramManager )
    {
        HardwareProgram program = iHardwareProgramManager->createProgram(name, vertexShaderPath, fragmentShaderPath);
        if( program.isExpired() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Can't create HardwareProgram '" << name << "'." << std::endl;
#endif
        }
        
        return program;
    }
    
    return HardwareProgram(nullptr);
}

RenderFramebufferHolderList RendererPrivate::getFramebuffers(int sz)
{
    if( iFrameContext.Framebuffers.size() >= sz )
    {
        // We have enough Framebuffers, returns the first ones.
        RenderFramebufferHolderList fbolist;
        
        for(int i = 0; i < sz; i++)
        {
            fbolist.add( *(iFrameContext.Framebuffers.begin() + i) );
        }
        
        return fbolist;
    }
    
    else
    {
        // Push the first Framebuffers.
        RenderFramebufferHolderList fbolist;
        
        for(int i = 0; i < iFrameContext.Framebuffers.size(); i++)
        {
            fbolist.add( *(iFrameContext.Framebuffers.begin() + i) );
        }
        
        int numtocreate = sz - iFrameContext.Framebuffers.size();
        
        // Creates as Framebuffers as necessary.
        for(int i = 0; i < numtocreate; i++)
        {
            RenderFramebufferHolder fbo = createFramebuffer();
            iFrameContext.Framebuffers.add(fbo);
            fbolist.add(fbo);
        }
    }
}

RenderFramebufferHolder RendererPrivate::createFramebuffer() const
{
    return FramebufferHolder(nullptr);
    
    /*
    auto rloader = ResourceManager::Get().getFramebufferLoaderFactory().get("OpenGlFramebufferLoader");
    
    if(rloader)
    {
        FramebufferHolder holder = ResourceManager::Get().loadResourceWith<FramebufferHolder>(rloader, Resource::Type::Framebuffer, ResourceManager::Get().getNameGenerator().generateName("Framebuffer"));
        
        return holder;
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "No loader to create Framebuffer." << std::endl;
        throw GreNoLoaderFound("OpenGlFramebufferLoader");
    }
#endif
    
    return FramebufferHolder(nullptr);
    */
}

RenderContextHolder RendererPrivate::createRenderContext(const std::string&, const RenderContextInfo&, Renderer)
{
    GreDebugFunctionNotImplemented();
    return RenderContextHolder(nullptr);
}

// ---------------------------------------------------------------------------------------------------

Renderer::Renderer()
: ResourceUser(), _mRenderer()
{
    
}

Renderer::Renderer(Renderer&& movref)
: ResourceUser(movref), _mRenderer(std::move(movref._mRenderer))
{
    
}

Renderer::Renderer (const Renderer& renderer)
: ResourceUser(renderer.lock()), _mRenderer(renderer._mRenderer)
{
    
}

Renderer::Renderer (const ResourceUser& renderer)
: ResourceUser(renderer.lock()), _mRenderer(std::dynamic_pointer_cast<RendererPrivate>(renderer.lock()))
{
    
}

Renderer::~Renderer()
{
    
}

Renderer& Renderer::operator=(const ResourceUser &ruser)
{
    ResourceUser::_resource = ruser.lock();
    _mRenderer = std::dynamic_pointer_cast<RendererPrivate>(ruser.lock());
    return *this;
}

Renderer& Renderer::operator=(const Renderer &ruser)
{
    ResourceUser::_resource = ruser.lock();
    _mRenderer = ruser._mRenderer;
    return *this;
}

void Renderer::render()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->render();
}

void Renderer::setFramerate(float fps)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setFramerate(fps);
}

float Renderer::getCurrentFramerate() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getCurrentFramerate();
    
    return 0.0f;
}

ElapsedTime Renderer::getElapsedTime() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getElapsedTime();
    return ElapsedTime::zero();
}

void Renderer::resetElapsedTime()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->resetElapsedTime();
}

void Renderer::beginRender()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->beginRender();
}

void Renderer::endRender()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->endRender();
}

void Renderer::setClearColor(const Color& color)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setClearColor(color);
}

void Renderer::setClearDepth(float depth)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setClearDepth(depth);
}

void Renderer::setActive(bool active)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setActive(active);
}

bool Renderer::isActive() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->isActive();
    return false;
}

bool Renderer::isImmediate() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->isImmediate();
    return false;
}

void Renderer::setImmediateMode(bool mode)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->setImmediateMode(mode);
}

void Renderer::addImmediateAction(std::function<void ()> action)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->addImmediateAction(action);
}

void Renderer::resetImmediateActions()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->resetImmediateActions();
}

void Renderer::translate(float x, float y, float z)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->translate(x, y, z);
}

void Renderer::rotate(float angle, float x, float y, float z)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->rotate(angle, x, y, z);
}

void Renderer::drawTriangle(float sz, const Color& color1, const Color& color2, const Color& color3)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->drawTriangle(sz, color1, color2, color3);
}

void Renderer::drawQuad(float sz, const Color& color1, const Color& color2, const Color& color3, const Color& color4)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->drawQuad(sz, color1, color2, color3, color4);
}

HardwareVertexBuffer Renderer::createVertexBuffer()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createVertexBuffer();
    return HardwareVertexBuffer::Null;
}

HardwareIndexBuffer Renderer::createIndexBuffer(PrimitiveType ptype, StorageType stype)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createIndexBuffer(ptype, stype);
    return HardwareIndexBuffer::Null;
}

Mesh Renderer::createMeshFromBuffers(const std::string &name, const HardwareVertexBuffer &vbuf, const HardwareIndexBufferBatch &ibufs)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createMeshFromBuffers(name, vbuf, ibufs);
    return Mesh::Null;
}

Texture Renderer::createTexture(const std::string &name, const std::string &file)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createTexture(name, file);
    return Texture::Null;
}

Camera Renderer::createCamera(const std::string &name)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createCamera(name);
    return Camera(nullptr);
}

void Renderer::draw(const Mesh &mesh, const HardwareProgram& activProgram)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->draw(mesh, activProgram);
}

void Renderer::prepare(const Camera& cam)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->prepare(cam);
}

Scene Renderer::loadSceneByName(const std::string& name, const std::string& sname)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->loadSceneByName(name, sname);
    return Scene::Null;
}

Scene Renderer::loadSceneByResource(Scene scene)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->loadSceneByResource(scene);
    return Scene::Null;
}

void Renderer::pushMatrix(MatrixType matrix)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->pushMatrix(matrix);
}

void Renderer::popMatrix(MatrixType matrix)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->popMatrix(matrix);
}

Scene& Renderer::getScene()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getScene();
    return Scene::Null;
}

const Scene& Renderer::getScene() const
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getScene();
    return Scene::Null;
}

void Renderer::transform(const Node &node)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->transform(node);
}

HardwareProgram Renderer::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createHardwareProgram(name, vertexShader, fragmentShader);
    return HardwareProgram::Null;
}

void Renderer::prepare(Gre::PassPrivate *privPass)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->prepare(privPass);
}

void Renderer::prepare(const FrameBuffer &fbo)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->prepare(fbo);
}

void Renderer::finish(const FrameBuffer &fbo)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->finish(fbo);
}

void Renderer::renderFrameBuffers(std::queue<FrameBuffer> &fboQueue)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->renderFrameBuffers(fboQueue);
}

RenderContext Renderer::createRenderContext(const std::string& name, const RenderContextInfo& info)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->createRenderContext(name, info, *this);
    return RenderContext::Null;
}

void Renderer::onCurrentContextChanged(Gre::RenderContextPrivate *renderCtxt)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->onCurrentContextChanged(renderCtxt);
}

void Renderer::addRenderTarget(const RenderTarget &renderTarget)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->addRenderTarget(renderTarget);
}

void Renderer::addRenderTarget(Gre::Window &window, bool autoCreateContext)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->addRenderTarget(window, *this, autoCreateContext);
}

void Renderer::selectDefaultScene(const Gre::Scene &defScene)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->selectDefaultScene(defScene);
}

Scene Renderer::getDefaultScene()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        return ptr->getDefaultScene();
    return Scene::Null;
}

void Renderer::render(Gre::RenderTarget &rtarget)
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->render(rtarget);
}

void Renderer::launchDrawingThread()
{
    auto ptr = _mRenderer.lock();
    if(ptr)
        ptr->launchDrawingThread();
}

Renderer Renderer::Null = Renderer();

// ---------------------------------------------------------------------------------------------------

RendererLoader::RendererLoader()
{
    
}

RendererLoader::~RendererLoader()
{
    
}

bool RendererLoader::isTypeSupported (Resource::Type type) const
{
    return type == Resource::Type::Renderer;
}

Resource* RendererLoader::load (Resource::Type type, const std::string& name) const
{
    return nullptr;
}

ResourceLoader* RendererLoader::clone() const
{
    return new RendererLoader();
}

// ---------------------------------------------------------------------------------------------------

RendererLoaderFactory::RendererLoaderFactory()
{
    
}

RendererLoaderFactory::~RendererLoaderFactory()
{
    
}

GreEndNamespace
