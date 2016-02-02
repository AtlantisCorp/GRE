//
//  OpenGlRenderer.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_OpenGlRenderer_h
#define GRE_OpenGlRenderer_h

#include "OpenGlIncludes.h"
#include "OpenGlVertexBuffer.h"
#include "OpenGlIndexBuffer.h"
#include "OpenGlTexture.h"

class OpenGlRenderer : public RendererResource
{
public:
    
    OpenGlRenderer (const std::string& name);
    ~OpenGlRenderer ();
    
    bool hasExtension(const std::string& ext) const;
    void setClearColor (const Color& color);
    void setClearDepth (float depth);
    
protected:
    void _preRender();
    void _postRender();
    
public:
    
    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    
    void drawTriangle(float sz, const Color& color1, const Color& color2, const Color& color3);
    void drawQuad(float sz, const Color& color1, const Color& color2, const Color& color3, const Color& color4);
    void draw(const Mesh& mesh, const HardwareProgram& activProgram);
    
private:
    void draw_legacy(const Mesh& mesh);
    
public:
    
    void prepareMaterial(const Material& mat);
    void prepare(const Camera& camera);
    void renderExample ();
    
    HardwareVertexBuffer createVertexBuffer();
    HardwareIndexBuffer createIndexBuffer(PrimitiveType ptype, StorageType stype);
    Texture createTexture(const std::string& name, const std::string& file);
    
    bool isCoreProfile() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Constructs a HardwareProgram using given Vertex Shader,
    /// IndexShader.
    /// @note You can set every value to HardwareShader::PassThrough if you
    /// just want default Program.
    //////////////////////////////////////////////////////////////////////
    HardwareProgram createHardwareProgram(const std::string& name, const HardwareShader& vertexShader, const HardwareShader& fragmentShader);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns expected data for user.
    //////////////////////////////////////////////////////////////////////
    const void* getCustomData(const std::string& dataname) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Prepare the Pass object to be drew.
    //////////////////////////////////////////////////////////////////////
    void prepare(PassPrivate* privPass);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Renders Framebuffer's in the Queue to the screen (default
    /// frame buffer), and clear the queue.
    //////////////////////////////////////////////////////////////////////
    void renderFrameBuffers(std::queue<FrameBuffer>& fboQueue);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a blank Texture object and returns it.
    //////////////////////////////////////////////////////////////////////
    Texture createTexture(const std::string& name) const;
    
private:
    
    GLfloat _mClearColor [4];
    GLfloat _mClearDepth;
    
    bool _suportVbo;
    
    /// @brief Global Vertex Array Object used by the Engine.
    GLuint _mVao;
    
    void setGlVersion();
    void initExtensions();
};

#endif
