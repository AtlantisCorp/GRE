//
//  OpenGlRendererClass.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlRenderer.h"

OpenGlRenderer::OpenGlRenderer (const std::string& name)
: RendererResource(name)
{
    _mClearColor[0] = 1.0f;
    _mClearColor[1] = 1.0f;
    _mClearColor[2] = 1.0f;
    _mClearColor[3] = 0.0f;
    _mClearDepth    = 1.0f;
    _mVao = 0;
    
    HardwareProgramManagerLoader* hdwLoader = ResourceManager::Get().getHardwareProgramManagerLoaderFactory().get("OpenGlHdwProgramManagerLoader");
    if(hdwLoader)
    {
        _mProgramManager = HardwareProgramManager(ResourceManager::Get().loadResourceWith(hdwLoader, Resource::Type::HwdProgManager, "OpenGlProgManager"));
    }
    
#ifdef GreIsDebugMode
    else
    {
        _mProgramManager = HardwareProgramManager::Null;
        GreDebugPretty() << "No OpenGlHdwProgramManagerLoader could be created." << std::endl;
        throw RendererNoProgramManagerException("No OpenGlHdwProgramManagerLoader could be created.");
    }
#endif
}

OpenGlRenderer::~OpenGlRenderer ()
{
    if(_mVao != 0)
    {
#   ifdef GreIsDebugMode
        GreDebugPretty() << "Deleted Vao (id=" << _mVao << ")." << std::endl;
#   endif
        glGlobalContext->getGl().DeleteVertexArrays(1, &_mVao);
    }
}

const void* OpenGlRenderer::getCustomData(const std::string &dataname) const
{
    if(dataname == "OpenGlVendor") {
        return &glGlobalContext->getGl().Vendor;
    }
    else if(dataname == "OpenGlRenderer") {
        return &glGlobalContext->getGl().Renderer;
    }
    else {
#ifdef GreIsDebugMode
        GreDebugPretty() << "No data called '" << dataname << "' in OpenGlRenderer." << std::endl;
#endif
        return nullptr;
    }
}

bool OpenGlRenderer::hasExtension(const std::string& ext) const
{
    return glGlobalContext->getGl().IsExtensionSupported(ext);
}

void OpenGlRenderer::setClearColor (const Color& color)
{
    _mClearColor[0] = color.getRed();
    _mClearColor[1] = color.getGreen();
    _mClearColor[2] = color.getBlue();
    _mClearColor[3] = color.getAlpha();
    glGlobalContext->getGl().ClearColor(_mClearColor[0], _mClearColor[1], _mClearColor[2], _mClearColor[3]);
}

void OpenGlRenderer::setClearDepth (float depth)
{
    _mClearDepth = depth;
    glGlobalContext->getGl().ClearDepth(_mClearDepth);
}

bool OpenGlRenderer::isCoreProfile() const
{
    return glGlobalContext->getGl().VersionMajor > 2;
}

void OpenGlRenderer::_preRenderCurrentScene()
{
    if(isCoreProfile())
    {
        if(_mVao == 0)
        {
            glGlobalContext->getGl().GenVertexArrays(1, &_mVao);
#ifdef GreIsDebugMode
            GreDebugPretty() << "Vertex Array created (id=" << _mVao << ")." << std::endl;
#endif
        }
        
        glGlobalContext->getGl().BindVertexArray(_mVao);
    
        // First we set up the Viewport.
        Surface sz = _mCurrentViewport.getSurface();
        GLsizei width = sz.width;
        GLsizei height = sz.height;
        glGlobalContext->getGl().Viewport(0, 0, width, height);
        
        glGlobalContext->getGl().Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
        glGlobalContext->getGl().ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glGlobalContext->getGl().Enable(GL_TEXTURE_2D);
        glGlobalContext->getGl().Disable(GL_LIGHTING);
    }
    
    else
    {
        // This stuff is for none - OpenGl Core profile.
        // It is deprecated. Please don't use, but you can of course
        // make it better if you want. But that's deprecated.
        
        // [TODO] Handle resizing in a separate method.
        Surface sz = _mCurrentViewport.getSurface();
        GLsizei width = sz.width;
        GLsizei height = sz.height;
        
        // Set the viewport.
        glGlobalContext->getGl().Viewport(0, 0, width, height);
        
        // Set the Projection Matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Calculate aspect ratio of the window
        gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
        
        // Reset ModelView Matrix.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // --------------------------------------------
        
        glGlobalContext->getGl().Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
        glGlobalContext->getGl().Enable(GL_TEXTURE_2D);
        glGlobalContext->getGl().Disable(GL_LIGHTING);
    }
}

void OpenGlRenderer::_renderCurrentScene()
{
    RendererResource::_renderCurrentScene();
}

void OpenGlRenderer::_postRenderCurrentScene()
{
    glGlobalContext->getGl().Flush();
    glGlobalContext->getGl().BindVertexArray(0);
}

void OpenGlRenderer::translate(float x, float y, float z)
{
    glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
}

void OpenGlRenderer::rotate(float angle, float x, float y, float z)
{
    glRotatef(angle, x, y, z);
}

void OpenGlRenderer::drawTriangle(float sz, const Color& color1, const Color& color2, const Color& color3)
{
    glBegin(GL_TRIANGLES);								// Drawing Using Triangles
    glColor4fv(color1.getData());
    glVertex3f( 0.0f, sz, 0.0f);					// Top
    glColor4fv(color2.getData());
    glVertex3f(-sz,  -sz, 0.0f);					// Bottom Left
    glColor4fv(color3.getData());
    glVertex3f( sz,  -sz, 0.0f);					// Bottom Right
    glEnd();											// Finished Drawing The Triangle
}

void OpenGlRenderer::drawQuad(float sz, const Color& color1, const Color& color2, const Color& color3, const Color& color4)
{
    glBegin(GL_QUADS);                                  // Draw A Quad
    glColor3f(color1.getRed(), 0.0f, 0.0f);
    glVertex3f(-sz,  sz, 0.0f);                     // Top Left
    glColor3f(color2.getRed(), 0.0f, 0.0f);
    glVertex3f( sz,  sz, 0.0f);                     // Top Right
    glColor3f(color3.getRed(), 0.0f, 0.0f);
    glVertex3f( sz, -sz, 0.0f);                     // Bottom Right
    glColor3f(color4.getRed(), 0.0f, 0.0f);
    glVertex3f(-sz, -sz, 0.0f);                     // Bottom Left
    glEnd();
}

void OpenGlRenderer::draw_legacy(const Mesh& mesh)
{
    // All of this stuff is DEPRECATED
    
    if(mesh.getType() == MeshPrivate::Type::Buffered)
    {
        auto vbuf = mesh.getVertexBuffer();
        vbuf.bind();
        
        if(vbuf.isDirty())
        {
            vbuf.update();
        }
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (char*) NULL);
        
        if(vbuf.isColorActivated())
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, sizeof(Vertex), (char*) (sizeof(Vector3) + sizeof(Vector2)));
        }
        
        for(auto indexbuf : mesh.getIndexBufferBatch().batchs)
        {
            indexbuf.bind();
            
            if(indexbuf.isDirty())
            {
                indexbuf.update();
            }
            
            prepareMaterial(indexbuf.getMaterial());
            if(indexbuf.getMaterial().texture.isBinded())
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (char*) (sizeof(Vector3)));
            }
            
            GLenum mode = OpenGlUtils::PrimitiveTypeToGl(indexbuf.getPrimitiveType());
            GLenum stype = OpenGlUtils::StorageTypeToGl(indexbuf.getStorageType());
            GLsizei sz = (GLsizei) indexbuf.count();
            glDrawElements(mode, sz, stype, 0);
            
            if(indexbuf.getMaterial().texture.isBinded())
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                indexbuf.getMaterial().texture.unbind();
            }
            
            indexbuf.unbind();
        }
        
        if(vbuf.isColorActivated())
            glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        vbuf.unbind();
    }
}

void OpenGlRenderer::draw(const Mesh& mesh, const HardwareProgram& activProgram)
{
    if(!isCoreProfile())
    {
        draw_legacy(mesh);
    }
    
    if(mesh.getType() == MeshPrivate::Type::Buffered)
    {
        bool isPosAttribEnabled = false;
        GLint posAttribLoc = -1;
        
        bool isColorAttribEnabled = false;
        GLint colorAttribLoc = -1;
        
        bool isTexCoordAttribEnabled = false;
        GLint texCoordAttribLoc = -1;
        
        auto vbuf = mesh.getVertexBuffer();
        vbuf.bind();
        
        if(vbuf.isDirty())
        {
            vbuf.update();
        }

        posAttribLoc = activProgram.getAttribLocation("Position");
#ifdef GreIsDebugMode
        if(posAttribLoc == -1)
        {
            GreDebugPretty() << "No Attrib 'Position' in shader. Please add it." << std::endl;
            return;
        }
#endif
        
        glGlobalContext->getGl().VertexAttribPointer(posAttribLoc, 3, GL_FLOAT, false, sizeof(Vertex), (char*) NULL);
        glGlobalContext->getGl().EnableVertexAttribArray(posAttribLoc);
        isPosAttribEnabled = true;
    
        
        if(vbuf.isColorActivated())
        {
            colorAttribLoc = activProgram.getAttribLocation("Color");
#ifdef GreIsDebugMode
            if(colorAttribLoc == -1)
            {
                GreDebugPretty() << "No Attrib 'Color' in shader. Please add it." << std::endl;
                isColorAttribEnabled = false;
            }
            else
            {
#endif
                
                glGlobalContext->getGl().VertexAttribPointer(colorAttribLoc, 4, GL_FLOAT, false, sizeof(Vertex), (char*) (sizeof(Vector3) + sizeof(Vector2)));
                glGlobalContext->getGl().EnableVertexAttribArray(colorAttribLoc);
                isColorAttribEnabled = true;
                
#ifdef GreIsDebugMode
            }
#endif
        }
        
        // [Experimental Texture support]
        if(vbuf.isTexCoordActivated())
        {
            texCoordAttribLoc = activProgram.getAttribLocation("TexCoord");
#ifdef GreIsDebugMode
            if(texCoordAttribLoc == -1)
            {
                GreDebugPretty() << "No Attrib 'TexCoord' in shader. Please add it." << std::endl;
                isTexCoordAttribEnabled = false;
            }
            else
            {
#endif
        
                glGlobalContext->getGl().VertexAttribPointer(texCoordAttribLoc, 2, GL_FLOAT, true, sizeof(Vertex), (char*) sizeof(Vector3));
                glGlobalContext->getGl().EnableVertexAttribArray(texCoordAttribLoc);
                isTexCoordAttribEnabled = true;
                
#ifdef GreIsDebugMode
            }
#endif
        }
        
        for(auto indexbuf : mesh.getIndexBufferBatch().batchs)
        {
            indexbuf.bind();
            if(indexbuf.isDirty())
            {
                indexbuf.update();
            }
        
            if(indexbuf.getMaterial().hasTexture())
            {
                glGlobalContext->getGl().ActiveTexture(GL_TEXTURE0);
                indexbuf.getMaterial().texture.bind();
            }
            
            prepareMaterial(indexbuf.getMaterial());
            
            GLenum mode = OpenGlUtils::PrimitiveTypeToGl(indexbuf.getPrimitiveType());
            GLenum stype = OpenGlUtils::StorageTypeToGl(indexbuf.getStorageType());
            GLsizei sz = (GLsizei) indexbuf.count();
            glDrawElements(mode, sz, stype, 0);
        
            if(indexbuf.getMaterial().hasTexture())
            {
                glGlobalContext->getGl().ActiveTexture(GL_TEXTURE0);
                indexbuf.getMaterial().texture.unbind();
            }
            
            indexbuf.unbind();
        }
        
        if(vbuf.isTexCoordActivated())
            glGlobalContext->getGl().DisableVertexAttribArray(texCoordAttribLoc);
        
        if(vbuf.isColorActivated())
            glGlobalContext->getGl().DisableVertexAttribArray(colorAttribLoc);
        
        glGlobalContext->getGl().DisableVertexAttribArray(posAttribLoc);
        
        vbuf.unbind();
    }
}

void OpenGlRenderer::renderFrameBuffers(std::queue<FrameBuffer> &fboQueue)
{
    // For each FrameBuffer, we render a plane with the Viewport size (i.e. the FrameBuffer
    // size). Those Framebuffers shoud have been given by the Pass's objects. Those FrameBuffer
    // only goals should be the final Rendered image on Screen. Custom FrameBuffers which do not
    // match the Viewport Size should not be used here.
    
    if(_mPlaneFboMesh.expired())
    {
        // This Vertices Positions corresponds to a Plane which have blanck color and
        // corresponding Texture Coord.
        float planeMeshVertexPosition [] =
        {// == V ==  ==T== ====C=====
            0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 1, 0, 0, 0, 0, 1,
            1, 1, 0, 1, 1, 0, 0, 0, 1,
            0, 1, 0, 0, 1, 0, 0, 0, 1
        };
        
        unsigned int planeMeshIndice [] =
        {
            0, 1, 2,
            2, 3, 0
        };
        
        // Create the Plane mesh.
        HardwareVertexBuffer vbuf = createVertexBuffer();
        vbuf.add(VertexBatchFromRaw(planeMeshVertexPosition, 4));
        vbuf.activateColor(false);
        
        HardwareIndexBuffer ibuf = createIndexBuffer(PrimitiveType::Triangles, StorageType::UnsignedInt);
        ibuf.add(IndexedFaceBatchFromRaw(planeMeshIndice, 2, 3));
        HardwareIndexBufferBatch ibufs;
        ibufs.batchs.push_back(ibuf);
        
        _mPlaneFboMesh = createMeshFromBuffers("_RendererPlaneFbo", vbuf, ibufs);
    }
    
    while (!fboQueue.empty())
    {
        FrameBuffer fbo = fboQueue.front();
        fboQueue.pop();
        
        // Just in case the fbo has not been unbinded.
        fbo.unbind();
        
        // Prepare the textures and renders it.
        // Function FrameBuffer::getDefaultAccessibleTexture() should returns
        // the default texture to render, i.e. Attachement::Color.
        _mPlaneFboMesh.getIndexBufferBatch().batchs.at(0).getMaterial().texture = fbo.getDefaultTextureAccessible();
        
        // We must have the valid fbohdwProgram.
        if(_mfboHdwProgram.expired())
        {
            // Create the fboHdwProgram
            _mfboHdwProgram = RendererResource::createHardwareProgram("shader-program-passthrough-fbo", "shaders/ogl-vertex-fbo.vs", "shaders/ogl-fragment-fbo.fs");
        }
        
        // Now we can render the Mesh.
        _mfboHdwProgram.bind();
        draw(_mPlaneFboMesh, _mfboHdwProgram);
        _mfboHdwProgram.unbind();
    }
    
    // After all this, every framebuffers have been renderer on the screen.
}

void OpenGlRenderer::prepare(PassPrivate *privPass)
{
    if(privPass)
    {
        Variant& vaoVariant = privPass->getVariantData(0);
        
        if(vaoVariant.isNull()) {
            GLuint vaopass = 0;
            glGlobalContext->getGl().GenVertexArrays(1, &vaopass);
            vaoVariant.reset(Variant::Policy::Integer, &vaopass);
        }
        
        glGlobalContext->getGl().BindVertexArray(vaoVariant.toInteger());
    }
}

void OpenGlRenderer::prepareMaterial(const Material& mat)
{
    if(mat.texture.isInvalid())
    {
        
    }
    
    else
    {
        mat.texture.bind();
    }
}

HardwareVertexBuffer OpenGlRenderer::createVertexBuffer()
{
    std::string hwdname = ResourceManager::Get().getNameGenerator().generateName("HardwareVertexBuffer");
    HardwareVertexBuffer oglbuffer = HardwareVertexBuffer(ResourceManager::Get().addResource(Resource::Type::HwdBuffer, hwdname, std::make_shared<OpenGlVertexBuffer>()));
    return oglbuffer;
}

HardwareIndexBuffer OpenGlRenderer::createIndexBuffer(PrimitiveType ptype, StorageType stype)
{
    std::string hwdname = ResourceManager::Get().getNameGenerator().generateName("HardwareIndexBuffer");
    HardwareIndexBuffer oglbuffer = HardwareIndexBuffer(ResourceManager::Get().addResource(Resource::Type::HwdBuffer, hwdname, std::make_shared<OpenGlIndexBuffer>(ptype, stype)));
    return oglbuffer;
}

Texture OpenGlRenderer::createTexture(const std::string& name, const std::string& file)
{
    auto loader = ResourceManager::Get().getImageLoaderFactory().findBestLoader(file);
    if(loader == nullptr)
        return Texture::Null;
    
    std::string imgname = ResourceManager::Get().getNameGenerator().generateName(name + "-img");
    Image img = Image(ResourceManager::Get().loadResourceWith(loader, Resource::Type::Image, imgname, file));
    if(img.expired())
        return Texture::Null;
    
    std::string texname = ResourceManager::Get().getNameGenerator().generateName(name);
    Texture texture = Texture(ResourceManager::Get().addResource(Resource::Type::Texture, texname, std::make_shared<OpenGlTexture>(texname, img)));
    return texture;
}

Texture OpenGlRenderer::createTexture(const std::string& name) const
{
    std::string texName = ResourceManager::Get().getNameGenerator().generateName(name);
    Texture texture = Texture(ResourceManager::Get().addResource(Resource::Type::Texture, texName, std::make_shared<OpenGlTexture>(texName)));
    return texture;
}

void OpenGlRenderer::prepare(const Camera &camera)
{
    if(isCoreProfile())
    {
        // We load the perspective matrix into the ShaderManager.
        Matrix4 projectionMatrix = camera.getProjection();
        _mProgramManager.setSdkUniformMat4("ProjectionMatrix", projectionMatrix);
        
        // The View Matrix.
        Matrix4 viewMatrix = camera.getView();
        _mProgramManager.setSdkUniformMat4("ViewMatrix", viewMatrix);
    }
    
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&(camera.getProjection()[0][0]));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMultMatrixf(&(camera.getView()[0][0]));
    }
}

HardwareProgram OpenGlRenderer::createHardwareProgram(const std::string& name, const Gre::HardwareShader &vertexShader, const Gre::HardwareShader &fragmentShader)
{
    HardwareShader vshader = vertexShader;
    if(vshader == HardwareShader::VertexPassThrough)
    {
        vshader = _mProgramManager.loadShader(ShaderType::Vertex, "passthrough-vs", "shaders/ogl-vertex-ps.vs");
#ifdef GreIsDebugMode
        if(vshader.expired())
        {
            GreDebugPretty() << "No passthrough vertex shader detected ! Should be in 'shaders/ogl-vertex-ps.vs'." << std::endl;
        }
#endif
    }
    
    HardwareShader fshader = fragmentShader;
    if(fshader == HardwareShader::FragmentPassThrough)
    {
        fshader = _mProgramManager.loadShader(ShaderType::Fragment, "passthrough-fs", "shaders/ogl-fragment-ps.fs");
#ifdef GreIsDebugMode
        if(vshader.expired())
        {
            GreDebugPretty() << "No passthrough fragment shader detected ! Should be in 'shaders/ogl-fragment-ps.fs'." << std::endl;
        }
#endif
    }
    
    if(vshader == HardwareShader::Null || fshader == HardwareShader::Null)
    {
        GreDebugPretty() << "Unable to create HardwareProgram '" << name << "'." << std::endl;
        if(!vshader.expired())
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No fragment shader !" << std::endl;
#endif
            _mProgramManager.unloadShader(vshader);
        }
        if(!fshader.expired())
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No vertex shader !" << std::endl;
#endif
            _mProgramManager.unloadShader(fshader);
        }
    }
    
    return _mProgramManager.createHardwareProgram(name, vshader, fshader);
}

RenderContext OpenGlRenderer::createRenderContext(const std::string &name, const RenderContextInfo &info, Renderer caller)
{
    auto it = _mRenderContextsByName.find(name);
    if(it == _mRenderContextsByName.end())
    {
        std::shared_ptr<OpenGlRenderContext> newCtxt (std::make_shared<OpenGlRenderContext>(name, info, caller));
        _mRenderContextsByName[name] = newCtxt;
        
        RenderContext retValue = RenderContext(ResourceUser(newCtxt));
        
        return retValue;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Sorry, name '" << name << "' for RenderContext is already taken. Please choose another one." << std::endl;
#endif
        return RenderContext::Null;
    }
}

RenderContext OpenGlRenderer::findRenderContextFromPrivate(RenderContextPrivate *renderCtxtPriv)
{
    if(!renderCtxtPriv)
    {
        return RenderContext::Null;
    }
    
    OpenGlRenderContext* oglCtxtPtr = reinterpret_cast<OpenGlRenderContext*>(renderCtxtPriv);
    if(oglCtxtPtr)
    {
        auto it = _mRenderContextsByName.find(oglCtxtPtr->getName());
        if(it == _mRenderContextsByName.end())
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "RenderContext named '" << oglCtxtPtr->getName() << "' is not present in RenderContext database. Ensure you created it using OpenGlRenderer::createRenderContext()." << std::endl;
#endif
            return RenderContext::Null;
        }
        
        if((*it).second.get() == oglCtxtPtr)
        {
            return RenderContext(ResourceUser(it->second));
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "RenderContext named '" << oglCtxtPtr->getName() << "' does not point to the same RenderContext as the one in the database. Ensure you created it using OpenGlRenderer::createRenderContext()." << std::endl;
#endif
            return RenderContext::Null;
        }
    }
    
#ifdef GreIsDebugMode
    else
    {
        GreDebugPretty() << "Given RenderContext pointer is invalid, reinterpret_cast() failed." << std::endl;
        return RenderContext::Null;
    }
#endif
}

void OpenGlRenderer::onCurrentContextChanged(Gre::RenderContextPrivate *renderCtxt)
{
    RendererResource::onCurrentContextChanged(renderCtxt);
    if(!getCurrentContext().expired())
    {
        glGlobalContext = (_mRenderContextsByName[renderCtxt->getName()]);
    }
    
    else
    {
        glGlobalContext = nullptr;
    }
}

