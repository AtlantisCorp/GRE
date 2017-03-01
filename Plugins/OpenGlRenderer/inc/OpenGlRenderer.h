//////////////////////////////////////////////////////////////////////
//
//  OpenGlRenderer.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/02/2017.
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

#ifndef OpenGlRenderer_h
#define OpenGlRenderer_h

#include <Renderer.h>

#ifdef __APPLE__
#   include <OpenGL/OpenGL.h>
#   include <OpenGL/gl3.h>
#else 
#   include <GL/gl3.h>
#endif

class OpenGlRenderer ;

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Texture.
//////////////////////////////////////////////////////////////////////
class OpenGlTexture : public Gre::Texture
{
public:
    
    POOLED ( Gre::Pools::Render )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlTexture (const std::string & name , const Gre::TextureType & type ,
                   const Gre::SoftwarePixelBufferHolderList & buffers) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlTexture () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'iGlTexture != 0'.
    //////////////////////////////////////////////////////////////////////
    virtual bool isGlTextureValid () const ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should bind the Texture to the correct target.
    //////////////////////////////////////////////////////////////////////
    virtual void _bind () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should unbind the Texture from its target.
    //////////////////////////////////////////////////////////////////////
    virtual void _unbind () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the GPU texture buffer to given Texture.
    //////////////////////////////////////////////////////////////////////
    virtual void _setBuffer ( ) const ;
    
    virtual void _setParameters ( GLenum target ) const ;
    virtual void _applySwizzling ( GLenum target , const Gre::SoftwarePixelBufferHolder& buffer ) const ;
    
protected:
    
    /// @brief We create only one iGlTexture id. When using cube map, the
    /// texture id does not vary.
    mutable GLuint iGlTexture ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Texture Manager.
//////////////////////////////////////////////////////////////////////
class OpenGlTextureCreator : public Gre::TextureInternalCreator
{
public:
    
    POOLED ( Gre::Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlTextureCreator ( const Gre::Renderer* renderer ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlTextureCreator () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads a Texture from a SoftwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::Texture* create (const std::string & name , const Gre::TextureType & type ,
                                   const Gre::SoftwarePixelBufferHolderList& buffer) const ;
    
protected:
    
    /// @brief
    const Gre::Renderer* iRenderer ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Shader.
//////////////////////////////////////////////////////////////////////
class OpenGlShader : public Gre::HardwareShader
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlShader ( const std::string & name , const Gre::ShaderType& type , const std::string & src ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlShader () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the errors logs when compiled.
    //////////////////////////////////////////////////////////////////////
    virtual const std::string& getErrorLog () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the gl shader ID .
    //////////////////////////////////////////////////////////////////////
    virtual GLuint getGlShader () const ;
    
protected:
    
    /// @brief
    std::string iErrorLog ;
    
    /// @brief
    GLuint iGlShader ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Program.
//////////////////////////////////////////////////////////////////////
class OpenGlProgram : public Gre::HardwareProgram
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlProgram (const std::string& name ,
                   const Gre::HardwareShaderUser& vertex ,
                   const Gre::HardwareShaderUser& fragment ,
                   bool cacheShaders = false ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlProgram () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new program instance.
    //////////////////////////////////////////////////////////////////////
    virtual void _create () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Binds the HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void _bind () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbinds the HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual void _unbind () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Attach the given shader to this Program.
    //////////////////////////////////////////////////////////////////////
    virtual void _attachShader ( const Gre::HardwareShaderUser& hwdShader, bool cacheShader = false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Finalize the Program.
    //////////////////////////////////////////////////////////////////////
    virtual void _finalize() ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Destroys the Program internal object.
    //////////////////////////////////////////////////////////////////////
    virtual void _deleteProgram () ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds an alias to given location.
    //////////////////////////////////////////////////////////////////////
    virtual void _setAttribLocation ( const std::string & name , int loc ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the location of a given attribute variable, or -1
    /// if this does not exists.
    //////////////////////////////////////////////////////////////////////
    virtual int getAttribLocation ( const std::string& name ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Activate the given Texture Unit.
    //////////////////////////////////////////////////////////////////////
    virtual void bindTextureUnit ( int unit ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Location for given uniform, or -1.
    //////////////////////////////////////////////////////////////////////
    virtual int getUniformLocation ( const std::string& name ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Manually sets a Variable.
    //////////////////////////////////////////////////////////////////////
    virtual void _setVariable ( const Gre::HardwareProgramVariable & var ) const ;
    
protected:
    
    /// @brief
    GLuint iGlProgram ;
    
    /// @brief Stores every encountered uniforms with their location.
    std::map < std::string , int > iUniformsByName ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl HardwareProgramManager.
//////////////////////////////////////////////////////////////////////
class OpenGlProgramManager : public Gre::HardwareProgramManager
{
public:
    
    POOLED ( Gre::Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlProgramManager ( const OpenGlRenderer* renderer , const std::string& name = "OpenGlProgramManager" ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlProgramManager () noexcept (false) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads the default program.
    //////////////////////////////////////////////////////////////////////
    virtual void _loadDefaultProgram () ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal method to create HardwareShader.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::HardwareShaderHolder iCreateHardwareShader ( const Gre::ShaderType& stype , const std::string& name , const std::string& source ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Internal method to create HardwareProgram.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::HardwareProgramHolder iCreateHardwareProgram ( const std::string& name , const Gre::HardwareShaderHolder& vshader , const Gre::HardwareShaderHolder& fshader ) const;
    
protected:
    
    /// @brief
    const OpenGlRenderer* iRenderer ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Index Buffer.
//////////////////////////////////////////////////////////////////////
class OpenGlHardwareIndexBuffer : public Gre::HardwareIndexBuffer
{
public:
    
    POOLED ( Gre::Pools::HdwBuffer )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlHardwareIndexBuffer ( const void* data , size_t sz , const Gre::IndexDescriptor& desc ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~OpenGlHardwareIndexBuffer () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the Hardware Buffer in order to use it.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the Hardware Buffer after it has been used.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds some data to this Buffer. Size is the total size in
    /// bytes, not the number of Vertex.
    //////////////////////////////////////////////////////////////////////
    virtual void addData(const char* vdata, size_t sz);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the data in this SoftwareVertexBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual const char* getData() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the data in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual void clearData();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the size of the buffer, in bytes.
    //////////////////////////////////////////////////////////////////////
    virtual size_t getSize() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of elements in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual size_t count() const;
    
protected:
    
    /// @brief
    GLuint iGlBuffer ;
    
    /// @brief
    size_t iSize ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Vertex Buffer.
//////////////////////////////////////////////////////////////////////
class OpenGlHardwareVertexBuffer : public Gre::HardwareVertexBuffer
{
public:
    
    POOLED ( Gre::Pools::HdwBuffer )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlHardwareVertexBuffer ( const void* data , size_t sz , const Gre::VertexDescriptor& desc ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlHardwareVertexBuffer () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Bind the Hardware Buffer in order to use it.
    //////////////////////////////////////////////////////////////////////
    virtual void bind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Unbind the Hardware Buffer after it has been used.
    //////////////////////////////////////////////////////////////////////
    virtual void unbind() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds some data to this Buffer. Size is the total size in
    /// bytes, not the number of Vertex.
    //////////////////////////////////////////////////////////////////////
    virtual void addData(const char* vdata, size_t sz);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the data in this SoftwareVertexBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual const char* getData() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the data in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual void clearData();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the size of the buffer, in bytes.
    //////////////////////////////////////////////////////////////////////
    virtual size_t getSize() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the number of elements in the buffer.
    //////////////////////////////////////////////////////////////////////
    virtual size_t count() const;
    
protected:
    
    /// @brief
    GLuint iGlBuffer ;
    
    /// @brief
    size_t iSize ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Gre::MeshManager.
///
/// As the MeshManager takes care of creating buffers for the meshes
/// (and so also for the MeshLoader) , the Renderer used is stored in
/// order to retrieve the associated RenderContext.
///
//////////////////////////////////////////////////////////////////////
class OpenGlMeshManager : public Gre::MeshManager
{
public:
    
    POOLED ( Gre::Pools::Manager )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlMeshManager ( const OpenGlRenderer* renderer ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlMeshManager () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareVertexBuffer with given data.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::HardwareVertexBufferHolder createVertexBuffer ( const void* data , size_t sz , const Gre::VertexDescriptor& desc ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareIndexBuffer with given data.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::HardwareIndexBufferHolder createIndexBuffer ( const void* data , size_t sz , const Gre::IndexDescriptor& desc ) const  ;
    
protected:
    
    /// @brief
    const OpenGlRenderer* iRenderer ;
};

//////////////////////////////////////////////////////////////////////
/// @brief OpenGl Gre::Renderer implementation.
//////////////////////////////////////////////////////////////////////
class OpenGlRenderer : public Gre::Renderer
{
public:
    
    POOLED ( Gre::Pools::Render )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlRenderer ( const std::string& name , const Gre::RendererOptions& options ) ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlRenderer () noexcept ( false ) ;
    
protected:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Should installs as many default features it can have.
    //////////////////////////////////////////////////////////////////////
    virtual void installDefaultFeatures () ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Sets the clearing color.
    ////////////////////////////////////////////////////////////////////////
    virtual void _setClearColor ( const Gre::Color& color ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Do some action before rendering.
    ////////////////////////////////////////////////////////////////////////
    virtual void _preRender () const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Do some action after rendering.
    ////////////////////////////////////////////////////////////////////////
    virtual void _postRender () const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current Viewport.
    //////////////////////////////////////////////////////////////////////
    virtual void _setViewport ( const Gre::Viewport& viewport ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current camera.
    //////////////////////////////////////////////////////////////////////
    virtual void _setCamera ( const Gre::Matrix4& projection , const Gre::Matrix4& view ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets the current node.
    //////////////////////////////////////////////////////////////////////
    virtual void _setNode ( const Gre::RenderNodeHolder& node ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Draws the Mesh using its buffers.
    //////////////////////////////////////////////////////////////////////
    virtual void _drawNodeMesh ( const Gre::MeshUser& mesh , const Gre::HardwareProgramHolder& program ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Post-drawing function.
    //////////////////////////////////////////////////////////////////////
    virtual void _postNode ( const Gre::RenderNodeHolder & node , const Gre::CameraUser& camera , const Gre::HardwareProgramHolder& program ) const ;
    
public:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a MeshManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::MeshManagerHolder iCreateMeshManager ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a HardwareProgramManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::HardwareProgramManagerHolder iCreateProgramManager ( ) const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a TextureManager for this Renderer.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::TextureInternalCreator* iCreateTextureCreator ( ) const ;
};

//////////////////////////////////////////////////////////////////////
/// @brief An OpenGlRenderer Loader.
//////////////////////////////////////////////////////////////////////
class OpenGlRendererLoader : public Gre::RendererLoader
{
public:
    
    POOLED ( Gre::Pools::Loader )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    OpenGlRendererLoader () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~OpenGlRendererLoader () noexcept ( false ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    ////////////////////////////////////////////////////////////////////////
    virtual Gre::ResourceLoader* clone() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns false.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns 'true' if the given RendererOptions is compatible
    /// with the Renderer loaded by this object.
    //////////////////////////////////////////////////////////////////////
    virtual bool isCompatible ( const Gre::RendererOptions& options ) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Renderer Object.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::RendererHolder load ( const std::string& name , const Gre::RendererOptions& options ) const;
};

#endif /* OpenGlRenderer_h */
