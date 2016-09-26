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
    iTextureManager.setRenderer(this);
}

RendererPrivate::~RendererPrivate() noexcept ( false )
{
    stop();
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
        holder->lockGuard();
        
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
            RenderSceneHolder smholder = holder->getSelectedScene().lock();
            
            if( smholder )
            {
                // Sets the RenderScene to the FrameContext.
                iFrameContext.RenderedScene = smholder;
                
                // We don't need to keep the locking on the RenderTarget's mutex, because next step
                // has nothing to do with it. So just unlock it and relock it when we need it.
                
                holder->unlockGuard();
                
                // Now Render the selected RenderScene.
                drawRenderScene(iFrameContext.RenderedScene);
                
                holder->lockGuard();
            }
            
            else
            {
                // If we don't have any RenderScene to render, abort.
#ifdef GreIsDebugMode
               // GreDebugPretty() << "No RenderScene selected for RenderTarget '" << holder->getName() << "'." << std::endl;
#endif
            }
            
            // The RenderScene has been drew to the Buffer, now call the 'RenderTarget::draw' function
            // to redraw the surface.
            //
            // Notes ( 26.09.2016 ) : Normally this function should be avoided. This is the WindowManager's job
            // to update the rendering surface, not the Renderer. Alternatively we could swap buffers , but forcing
            // a Surface rendering is surely not the best way to draw our RenderTarget.
            
//          holder->draw();
            
            // After rendering the RenderScene, just unbind the RenderContext.
            iLastRenderContext->unbind();
        }
        
        else
        {
            // If we don't have any RenderContext, abort.
#ifdef GreIsDebugMode
            GreDebugPretty() << "No RenderContext selected for RenderTarget '" << holder->getName() << "'." << std::endl;
#endif
        }
        
        holder->unlockGuard();
    }
}

void RendererPrivate::drawRenderScene(const Gre::RenderScene &scenemanager)
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
        // Program has correct location, now bind it and configure the Camera.
        program->bind();
        
        // NOTES : The Camera is given by the CameraManager as 'CameraManager::getCurrentCamera()'. But, if you have to
        // render multiple cameras, you can do it in rendering in multiple RenderFramebuffer then using those Texture
        // to your final program.
        // The Camera will updates the 'uProjectionMatrix' and 'uViewMatrix' variables from the HardwareProgram. Please
        // be sure your HardwareProgram has those 2 variables.
        
        CameraHolder camera ( nullptr );
        
        // Now we are drawing to the correct Framebuffer, using the correct HardwareProgram. We still have to
        // get the Objects to draw.
        RenderNodeHolderList nodes = iFrameContext.RenderedScene->findNodes(camera, DefaultRenderSceneFilter());
        
        // The RenderNode's objects should be rendered now to the binded Framebuffer, accordingly to the given
        // HardwareProgram.
        drawRenderNodeList(nodes, program);
        
        program->unbind();
    }
    
    fboholder->unbind();
}

void RendererPrivate::drawRenderNodeList(RenderNodeHolderList& nodes, HardwareProgramHolder& program)
{
    if( !nodes.empty() && program )
    {
        // We iterates to every Node's from the list given by the RenderScene to draw the
        // objects. Those Objects are 'placed' thanks to the 'ModelMatrix' uniform.
        // Notes the ViewMatrix and ProjectionMatrix are set by the Camera during Rendering.
        
        for(RenderNodeHolder& node : nodes)
        {
            Mesh mesh = node->getMesh();
            
            if( mesh.isExpired() )
            {
                continue;
            }
            
            // Updates the Model Matrix.
            
            program->setUniformMat4("ModelMatrix", node->getModelMatrix());
            
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
                program->bindMaterial(material);
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
                program->unbindMaterial(material);
            }
        }
    }
}

HardwareVertexBufferHolder RendererPrivate::createVertexBuffer()
{
    GreDebugFunctionNotImplemented();
    return HardwareVertexBufferHolder ( nullptr );
}

HardwareIndexBufferHolder RendererPrivate::createIndexBuffer()
{
    GreDebugFunctionNotImplemented();
    return HardwareIndexBufferHolder ( nullptr );
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
            HardwareVertexBufferHolder gpuvbuf = createVertexBufferWithSize(meshholder->getName() + "/HdwVertexBuffer" , softvbuf.getSize());
            
            if ( gpuvbuf.isInvalid() )
            {
#ifdef GreIsDebugMode
                GreDebugPretty() << "Can't create HardwareVertexBuffer for Mesh '" << meshholder->getName() << "'." << std::endl;
#endif
            }
            
            else
            {
                gpuvbuf->setVertexDescriptor(softvbuf.getVertexDescriptor());
                gpuvbuf->addData(softvbuf.getData(), softvbuf.getSize());
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
            HardwareIndexBufferHolder gpuibuf = createIndexBuffer();
            
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
                
                for ( auto batch : softibuf.getIndexBatches() )
                {
                    gpuibuf->addIndexBatch(batch);
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
        meshholder->onNextEvent(EventType::Update , [meshholder, deleteVertexBuffer, deleteIndexBuffer] (const Event& e)
        {
            MeshHolder holder (meshholder);
            if ( deleteVertexBuffer )
            {
                holder->getSoftwareVertexBufferHolder().reset();
            }
            
            if ( deleteIndexBuffer )
            {
                holder->getSoftwareIndexBufferHolder().reset();
            }
        });
    }
}

TextureHolder RendererPrivate::createTexture(const std::string &name)
{
    if ( !name.empty() )
    {
        // Check if Texture already exists.
        
        if ( iTextureManager.isLoaded(name) )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Texture Resource '" << name << "' already loaded." << std::endl;
#endif
            return TextureHolder ( nullptr );
        }
        
        // Load the Texture using private function.
        
        TextureHolder ret = iCreateTexturePrivate ( name );
        
        if ( ret.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Texture Resource '" << name << "' couldn't be loaded." << std::endl;
#endif
            return TextureHolder ( nullptr );
        }
        
        return ret;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return TextureHolder ( nullptr );
    }
}

TextureHolder RendererPrivate::createTexture(const std::string &name, const std::string &file)
{
    if ( !name.empty() )
    {
        // Try to load the Texture using TextureManager::load() . This will call RendererPrivate::createTexture(name).
        
        Texture loaded = iTextureManager.load(name, file);
        TextureHolder ret ( loaded.lock() );
        
        if ( ret.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Texture Resource '" << name << "' can't be loaded." << std::endl;
#endif
            return TextureHolder ( nullptr );
        }
        
        // Once loaded, the Texture Objet will be sent by the TextureManager UpdateEvent . Texture::onUpdateEvent() should
        // check that the SoftwarePixelBuffer is loaded but the HardwareBuffer is not loaded, so it should create it.
        
        return ret;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is invalid." << std::endl;
#endif
        return TextureHolder ( nullptr );
    }
}

TextureHolder RendererPrivate::createEmptyTexture(int width, int height)
{
    static unsigned EmptyTextureCount = 0;
    
    // Check if width and height are valid.
    
    if ( width > 0 && height > 0 )
    {
        std::string tname = std::string("EmptyTexture#") + std::to_string(EmptyTextureCount) + "[" + std::to_string(width) + "/" + std::to_string(height) + "]";
        
        // Creates a new Texture, and changes its Surface.
        
        TextureHolder tex = iCreateTexturePrivate(tname);
        tex->setSurface({0, 0, width, height});
        
        // Load it to the TextureManager.
        
        iTextureManager.load(tex);
        return tex;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'width' or 'height' is invalid." << std::endl;
#endif
        return TextureHolder ( nullptr );
    }
}

TextureManager& RendererPrivate::getTextureManager()
{
    return iTextureManager;
}

const TextureManager& RendererPrivate::getTextureManager() const
{
    return iTextureManager;
}

HardwareProgramManager RendererPrivate::getHardwareProgramManager()
{
    return HardwareProgramManager ( iProgramManager );
}

const HardwareProgramManager RendererPrivate::getHardwareProgramManager() const
{
    return HardwareProgramManager ( iProgramManager );
}

HardwareProgram RendererPrivate::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    if( iProgramManager )
    {
        HardwareProgram program = iProgramManager->createHardwareProgram(name, vertexShader, fragmentShader);
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
    if( iProgramManager )
    {
        HardwareProgram program = iProgramManager->createHardwareProgramFromFiles(name, vertexShaderPath, fragmentShaderPath);
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
    if( iFrameContext.Framebuffers.size() >= (size_t) sz )
    {
        // We have enough Framebuffers, returns the first ones.
        
        RenderFramebufferHolderList fbolist;
        
        int i = 0;
        for ( auto& framebuf : iFrameContext.Framebuffers )
        {
            fbolist.add( framebuf );
            
            if ( i >= sz )
            {
                break;
            }
            
            else
            {
                i++;
            }
        }
        
        return fbolist;
    }
    
    else
    {
        // Push the first Framebuffers.
        
        RenderFramebufferHolderList fbolist;
        
        for ( auto& framebuf : iFrameContext.Framebuffers )
        {
            fbolist.add( framebuf );
        }
        
        int numtocreate = (int) ( sz - iFrameContext.Framebuffers.size() );
        
        // Creates as Framebuffers as necessary.
        for ( int i = 0; i < numtocreate; i++ )
        {
            RenderFramebufferHolder fbo = createFramebuffer();
            iFrameContext.Framebuffers.add(fbo);
            fbolist.add(fbo);
        }
        
        return fbolist;
    }
}

RenderFramebufferHolder RendererPrivate::createFramebuffer() const
{
    return RenderFramebufferHolder(nullptr);
    
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

RenderContextHolder RendererPrivate::createRenderContext(const std::string &name, const Gre::RenderContextInfo &info)
{
    // Here we just call the private 'iCreateRenderContext()' method, and register the newly created context.
    
    RenderContextHolder newctxt = iCreateRenderContext(name, info);
    
    if ( !newctxt.isInvalid() )
    {
        iRenderContexts.add(newctxt);
        return newctxt;
    }
    
    return RenderContextHolder ( nullptr );
}

HardwareVertexBufferHolder RendererPrivate::createVertexBufferWithSize(const std::string &name, size_t sz) const
{
    // These function is just a helper to use ::iCreateVertexBuffer().
    return iCreateVertexBuffer(name, sz);
}

void RendererPrivate::clearRenderContexts()
{
    iRenderContexts.clear();
}

void RendererPrivate::registerTarget(const RenderTargetHolder &holder)
{
    if ( !holder.isInvalid() )
    {
        iTargets.add(holder);
    }
}

void RendererPrivate::launch()
{
    if ( iLaunchThread.joinable() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Launch thread for Renderer '" << getName() << "' has already been launched." << std::endl;
#endif
        return;
    }
    
    iLaunchThread = std::thread ( [] (RendererPrivate* renderer) {
        
        if ( renderer )
        {
            while ( !renderer->getLaunchMustStop() )
            {
                RenderTargetHolderList& targets = renderer->getRegisteredTargets();
                
                if ( !targets.empty() )
                {
                    for ( RenderTargetHolder& holder : targets )
                    {
                        if ( holder->isAvailableForDrawing() )
                        {
                            renderer->drawRenderTarget( RenderTarget(holder) );
                        }
                    }
                }
            }
        }
        
    } , this );
}

bool RendererPrivate::getLaunchMustStop() const
{
    return iLaunchMustStop;
}

RenderTargetHolderList& RendererPrivate::getRegisteredTargets()
{
    return iTargets;
}

const RenderTargetHolderList& RendererPrivate::getRegisteredTargets() const
{
    return iTargets;
}

void RendererPrivate::stop()
{
    if ( iLaunchThread.joinable() )
    {
        iLaunchMustStop = true;
        iLaunchThread.join();
    }
}

// ---------------------------------------------------------------------------------------------------

Renderer::Renderer(const RendererPrivate* pointer)
: Gre::ResourceUser(pointer)
, SpecializedResourceUser<Gre::RendererPrivate>(pointer)
{
    
}

Renderer::Renderer(const RendererHolder& holder)
: Gre::ResourceUser(holder)
, SpecializedResourceUser<Gre::RendererPrivate>(holder)
{
    
}

Renderer::Renderer(const Renderer& user)
: Gre::ResourceUser(user)
, SpecializedResourceUser<Gre::RendererPrivate>(user)
{
    
}

Renderer::~Renderer()
{
    
}

Renderer Renderer::Null = Renderer(nullptr);

// ---------------------------------------------------------------------------------------------------

RendererLoader::RendererLoader()
{
    
}

RendererLoader::~RendererLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

RendererManager::RendererManager()
{
    
}

RendererManager::~RendererManager()
{
    
}

Renderer RendererManager::load(const RendererHolder &renderer)
{
    if ( !renderer.isInvalid() )
    {
        Renderer tmp = get(renderer->getName());
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Renderer Resource '" << renderer->getName() << "' already loaded." << std::endl;
#endif
            return Renderer ( nullptr );
        }
        
        iRenderers.add(renderer);
        return Renderer ( renderer );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'renderer' is invalid." << std::endl;
#endif
        return Renderer ( nullptr );
    }
}

Renderer RendererManager::load(const std::string &name)
{
    if ( !name.empty() )
    {
        Renderer tmp = get(name);
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Renderer Resource '" << name << "' already loaded." << std::endl;
#endif
            return Renderer ( nullptr );
        }
        
        for ( auto it = iFactory.getLoaders().begin(); it != iFactory.getLoaders().end(); it++ )
        {
            auto loader = it->second;
            
            if ( loader )
            {
                RendererHolder rholder = loader->load(name);
                
                if ( rholder.isInvalid() )
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by loader '" << it->first << "'." << std::endl;
#endif
                }
                
                else
                {
                    iRenderers.add(rholder);
                    return Renderer ( rholder );
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by any installed Loader." << std::endl;
#endif
        return Renderer ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Renderer ( nullptr );
    }
}

Renderer RendererManager::load(const std::string &name, const Gre::RenderingApiDescriptor &apidescriptor)
{
    if ( !name.empty() )
    {
        Renderer tmp = get(name);
        
        if ( !tmp.isInvalid() )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Renderer Resource '" << name << "' already loaded." << std::endl;
#endif
            return Renderer ( nullptr );
        }
        
        for ( auto it = iFactory.getLoaders().begin(); it != iFactory.getLoaders().end(); it++ )
        {
            auto loader = it->second;
            
            if ( loader )
            {
                if ( loader->isCompatible(apidescriptor) )
                {
                    RendererHolder rholder = loader->load(name);
                    
                    if ( rholder.isInvalid() )
                    {
#ifdef GreIsDebugMode
                        GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by loader '" << it->first << "'." << std::endl;
#endif
                    }
                    
                    else
                    {
                        iRenderers.add(rholder);
                        return Renderer ( rholder );
                    }
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Renderer Resource '" << name << "' could not be loaded by any installed Loader." << std::endl;
#endif
        return Renderer ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Renderer ( nullptr );
    }
}

Renderer RendererManager::get(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( RendererHolder& holder : iRenderers )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Renderer ( holder );
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Renderer Resource '" << name << "' not found." << std::endl;
#endif
        return Renderer ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Renderer ( nullptr );
    }
}

const Renderer RendererManager::get(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( const RendererHolder& holder : iRenderers )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Renderer ( holder );
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Renderer Resource '" << name << "' not found." << std::endl;
#endif
        return Renderer ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Renderer ( nullptr );
    }
}

const RendererHolderList& RendererManager::getRenderers() const
{
    return iRenderers;
}

void RendererManager::remove(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( auto it = iRenderers.begin(); it != iRenderers.end(); it++ )
        {
            RendererHolder& holder = *it;
            
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    iRenderers.erase(it);
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Renderer Resource '" << name << "' not found." << std::endl;
#endif
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
    }
}

void RendererManager::clearRenderers()
{
    iRenderers.clear();
}

RendererLoaderFactory& RendererManager::getRendererLoaderFactory()
{
    return iFactory;
}

const RendererLoaderFactory& RendererManager::getRendererLoaderFactory() const
{
    return iFactory;
}

void RendererManager::clear()
{
    clearRenderers();
    iFactory.clear();
}

GreEndNamespace
