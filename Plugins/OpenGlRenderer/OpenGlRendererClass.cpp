//
//  OpenGlRendererClass.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlRenderer.h"

int FindFirstOccurence(const std::string& in, char what)
{
    if(in.empty())
        return -1;
    
    int i = 0;
    while(in[i] != '\0' && in[i] != what)
        ++i;
    
    if(in[i] == '\0' || in[i] == what)
        return i;
    return -1;
}

OpenGlRenderer::OpenGlRenderer (const std::string& name)
: RendererResource(name)
{
    _mClearColor[0] = 1.0f;
    _mClearColor[1] = 1.0f;
    _mClearColor[2] = 1.0f;
    _mClearColor[3] = 0.0f;
    _mClearDepth    = 1.0f;
    
    initializeFunctions();
    setGlVersion();
    initExtensions();
    
    _suportVbo = hasExtension("GL_ARB_vertex_buffer_object") || _gl_major >= 3;
    if(!_suportVbo) {
        GreDebugPretty() << "Warning : Current Hardware does not support Vertex Buffer Objects ! This can be followed by very bad performances." << std::endl;
    }
}

OpenGlRenderer::~OpenGlRenderer ()
{
    
}

void OpenGlRenderer::initializeFunctions()
{
    _glGetStringi  = (PFNGLGETSTRINGIPROC)  GlGetProcAddress("glGetStringi");
    _glGetIntegerv = (PFNGLGETINTEGERVPROC) GlGetProcAddress("glGetIntegerv");
    _glGetString   = (PFNGLGETSTRINGPROC)   GlGetProcAddress("glGetString");
}

void OpenGlRenderer::setGlVersion()
{
    if(!_glGetString) {
        GreDebugPretty() << "Couldn't find glGetString entry point." << std::endl;
        throw RendererInvalidApi();
    }
    
    char* GL_version  = (char*) _glGetString(GL_VERSION);
    char* GL_vendor   = (char*) _glGetString(GL_VENDOR);
    char* GL_renderer = (char*) _glGetString(GL_RENDERER);
    
    int vpoint = FindFirstOccurence(std::string(GL_version), '.');
    if(vpoint >= 0)
    {
        _gl_major = GL_version[vpoint-1] - '0';
        _gl_minor = GL_version[vpoint+1] - '0';
    }
    else
        throw RendererInvalidVersion();
    
    GreDebugPretty() << "Version  : " << GL_version << std::endl;
    GreDebugPretty() << "Vendor   : " << GL_vendor << std::endl;
    GreDebugPretty() << "Renderer : " << GL_renderer << std::endl;
}

void OpenGlRenderer::initExtensions()
{
    if(_gl_major >= 3)
    {
        if(!_glGetStringi || !_glGetIntegerv) {
            GreDebugPretty() << "Couldn't find glGetStringi or glGetIntegerv entry points." << std::endl;
            return;
        }
        
        GLint extnum = 0;
        _glGetIntegerv(GL_NUM_EXTENSIONS, &extnum);
        
        for(GLuint cext = 0; cext < extnum; ++cext) {
            _extensions.push_back(std::string((char*) _glGetStringi(GL_EXTENSIONS, cext)));
        }
    }
    else
    {
        if(!_glGetString) {
            GreDebugPretty() << "Couldn't find glGetString entry point." << std::endl;
            return;
        }
        
        char* exts = (char*) _glGetString(GL_EXTENSIONS);
        _extensions = split(std::string(exts), ' ');
    }
    
    GreDebugPretty() << "Num Ext  : " << _extensions.size() << std::endl;
}

bool OpenGlRenderer::hasExtension(const std::string& ext) const
{
    return std::find(_extensions.begin(), _extensions.end(), ext) != _extensions.end();
}

void OpenGlRenderer::setClearColor (const Color& color)
{
    _mClearColor[0] = color.getRed();
    _mClearColor[1] = color.getGreen();
    _mClearColor[2] = color.getBlue();
    _mClearColor[3] = color.getAlpha();
    glClearColor(_mClearColor[0], _mClearColor[1], _mClearColor[2], _mClearColor[3]);
}

void OpenGlRenderer::setClearDepth (float depth)
{
    _mClearDepth = depth;
    glClearDepth(_mClearDepth);
}

void OpenGlRenderer::_preRender()
{
    // [TODO] Handle resizing in a separate method.
    WindowSize sz = _window.getWindowSize();
    GLsizei width = sz.first;
    GLsizei height = sz.second;
    
    // Set the viewport.
    glViewport(0, 0, width, height);
    
    // Set the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Calculate aspect ratio of the window
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    
    // Reset ModelView Matrix.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // --------------------------------------------
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    glEnable(GL_TEXTURE_2D);
}

void OpenGlRenderer::_render ()
{
    // Here we draw the scene.
    Camera& camera = _mScene.getCamera();
    prepare(camera);
    
    // Now we draw each nodes.
    for(auto node : _mScene.getNodesByFilter(Node::Filter::FarthestToNearest))
    {
        pushMatrix(MatrixType::ModelView);
        
        transform(node);
        draw(node.getMesh());
        
        popMatrix(MatrixType::ModelView);
    }
}

void OpenGlRenderer::_postRender()
{
    glFlush();
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

void OpenGlRenderer::draw(const Mesh& mesh)
{
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

void OpenGlRenderer::renderExample ()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDepthFunc(GL_ALWAYS);
    
    glClearColor(_mClearColor[0], _mClearColor[1], _mClearColor[2], _mClearColor[3]);
    glClearDepth(_mClearDepth);
    glClear(GL_COLOR_BUFFER_BIT);
    
    WindowSize sz = _window.getWindowSize();
    
    glViewport(0,0,sz.first,sz.second);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(float)sz.first-1,(float)sz.second-1,0,-1,1);
    
    int x1,y1,x2,y2;
    int r,g,b;
    
    x1=rand()%sz.first;
    y1=rand()%sz.second;
    x2=rand()%sz.first;
    y2=rand()%sz.second;
    r=rand()%256;
    g=rand()%256;
    b=rand()%256;
    
    glColor3ub(r,g,b);
    glBegin(GL_LINES);
    glVertex2i(x1,y1);
    glVertex2i(x2,y2);
    glEnd();
    glFlush();
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

void OpenGlRenderer::prepare(const Camera &camera)
{
    glMatrixMode(GL_PROJECTION);
    
    Matrix4 m = camera.getMatrix();
    glLoadMatrixf(&(m[0][0]));
    
    glMatrixMode(GL_MODELVIEW);
}

