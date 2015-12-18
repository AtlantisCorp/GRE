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
private:
    
    GLfloat _mClearColor [4];
    GLfloat _mClearDepth;
    GLint   _gl_major;
    GLint   _gl_minor;
    std::vector<std::string> _extensions; ///< @brief Supported extensions.
    
    bool _suportVbo;
    
public:
    
    OpenGlRenderer (const std::string& name);
    ~OpenGlRenderer ();
    
    bool hasExtension(const std::string& ext) const;
    void setClearColor (const Color& color);
    void setClearDepth (float depth);
    
protected:
    void _preRender();
    void _render ();
    void _postRender();
    
public:
    
    void translate(float x, float y, float z);
    void rotate(float angle, float x, float y, float z);
    
    void drawTriangle(float sz, const Color& color1, const Color& color2, const Color& color3);
    void drawQuad(float sz, const Color& color1, const Color& color2, const Color& color3, const Color& color4);
    void draw(const Mesh& mesh);
    
    void prepareMaterial(const Material& mat);
    void prepare(const Camera& camera);
    void renderExample ();
    
    HardwareVertexBuffer createVertexBuffer();
    HardwareIndexBuffer createIndexBuffer(PrimitiveType ptype, StorageType stype);
    Texture createTexture(const std::string& name, const std::string& file);
    
private:
    
    PFNGLGETSTRINGPROC   _glGetString;
    PFNGLGETSTRINGIPROC  _glGetStringi;
    PFNGLGETINTEGERVPROC _glGetIntegerv;
    
    void initializeFunctions();
    void setGlVersion();
    void initExtensions();
};

#endif
