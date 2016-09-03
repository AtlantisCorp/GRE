//////////////////////////////////////////////////////////////////////
//
//  HardwareProgram.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/01/2016.
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

#include "HardwareProgram.h"

GreBeginNamespace

HardwareProgramPrivate::HardwareProgramPrivate(const std::string& name, const HardwareShader& vertexShader, const HardwareShader& fragmentShader)
: Resource(name), iVertexShader(vertexShader), iFragmentShader(fragmentShader), iVariables(), iNeedsCompilation(true), iNeedsVarUpdate(false)
{
    
}

HardwareProgramPrivate::~HardwareProgramPrivate()
{
    
}

void HardwareProgramPrivate::bind() const
{
#ifdef GreIsDebugMode
    if(!isCompiled())
    {
        GreDebugPretty() << "Can't bind HardwareProgram '" << getName() << "' because not compiled." << std::endl;
    }
#endif
}

void HardwareProgramPrivate::unbind() const
{
    
}

void HardwareProgramPrivate::attachShader(const HardwareShader& hwdShader)
{
    if(isCompiled())
    {
        // If program is already ready, we can't change it so we must clear it.
        // We store the Current Shader in order not to loose them.
        
        HardwareShader tmpVertex = iVertexShader;
        HardwareShader tmpFragment = iFragmentShader;
        
        reset();
        
        iVertexShader = tmpVertex;
        iFragmentShader = tmpFragment;
    }
    
    if(hwdShader.getType() == ShaderType::Vertex)
    {
        iVertexShader = hwdShader;
    }
    
    if(hwdShader.getType() == ShaderType::Fragment)
    {
        iFragmentShader = hwdShader;
    }
}

void HardwareProgramPrivate::finalize()
{
    iNeedsCompilation = true;
}

bool HardwareProgramPrivate::isReady() const
{
    return isCompiled();
}

HardwareProgramVariables HardwareProgramPrivate::getVariables()
{
    return iVariables;
}

void HardwareProgramPrivate::setUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    HardwareProgramVariable var;
    var.name = name;
    var.type = HdwProgVarType::Mat4;
    var.value.mat4 = mat4;
    iVariables.add(var);
    iNeedsVarUpdate = true;
}

void HardwareProgramPrivate::setUniform(const Gre::HardwareProgramVariable &var)
{
    iVariables.add(var);
    iNeedsVarUpdate = true;
}

void HardwareProgramPrivate::unsetUniform(const std::string &name)
{
    iVariables.remove(name);
}

int HardwareProgramPrivate::getAttribLocation(const std::string& name) const
{
    return -1;
}

bool HardwareProgramPrivate::isCompiled() const
{
    return iNeedsCompilation;
}

void HardwareProgramPrivate::reset()
{
    iVertexShader.reset();
    iFragmentShader.reset();
    iVariables.clear();
    iNeedsCompilation = true;
    iNeedsVarUpdate = false;
}

bool HardwareProgramPrivate::hasOutputLocation(uint16_t location) const
{
    return false;
}

void HardwareProgramPrivate::bindTextureUnit(int) const
{
    
}

void HardwareProgramPrivate::setCompiled(bool flag)
{
    iNeedsCompilation = flag;
}

void HardwareProgramPrivate::bindAttribsVertex(const VertexDescriptor &descriptor, const char* data) const
{
    if ( iNeedsCompilation )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "HardwareProgram '" << getName() << "' is not compiled." << std::endl;
#endif
        return;
    }
    
    if ( descriptor.getSize() )
    {
        VertexComponents components = descriptor.getComponents();
        
        for ( auto comp : components )
        {
            if ( VertexComponentTypeGetSize(comp) )
            {
                int index = getAttribLocation(comp);
                
                if ( index >= 0 )
                {
                    setAttribData(index, descriptor.getStride(comp), data + descriptor.getComponentLocation(comp) );
                }
                
                else
                {
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Attribute '" << VertexComponentTypeToString(comp) << "' not found in HardwareProgram '" << getName() << "'." << std::endl;
#endif
                }
            }
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Descriptor given is empty." << std::endl;
#endif
    }
}

int HardwareProgramPrivate::getAttribLocation(const VertexComponentType& component) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
    return -1;
}

void HardwareProgramPrivate::setAttribData(int index, size_t stride, const char *data) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
}

void HardwareProgramPrivate::bindAttributeLocation(const Gre::VertexComponentType &component, int index)
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
}

void HardwareProgramPrivate::bindMaterial(const Gre::Material &material) const
{
    if ( !material.isInvalid() )
    {
        // First, we see if the Material has Texture to bind.
        
        if ( material.hasTexture() && !material.hasMultitexture() )
        {
            const Texture& tex = material.getTexture();
            
            if ( !tex.isInvalid() )
            {
                std::string texname = std::string("uTexture0");
                int texloc = getUniformLocation(texname);
                
                if ( texloc >= 0 )
                {
                    // Remember that the Texture's type should be handled by the Shader itself. Here we just
                    // communicate the Texture Unit to the sampler variable.
                    
                    HardwareProgramVariable texunit;
                    texunit.location = texloc;
                    texunit.type = HdwProgVarType::Uint32;
                    texunit.value.uint32 = 0;
                    setUniformVariable(texunit);
                    
                    // As 'setUniformVariable' should not wait an update to set the Uniform , we should bind
                    // the Texture here and communicate the Texture object our Texture Unit.
                    
                    tex.bindWithTextureUnit(0);
                }
            }
        }
        
        if ( material.hasMultitexture() )
        {
            auto texvec = material.getTextures();
            int loc = 0;
            
            for ( auto& tex : texvec )
            {
                if ( !tex.isInvalid() )
                {
                    std::string texname = std::string("uTexture") + std::to_string(loc);
                    int texloc = getUniformLocation(texname);
                    
                    if ( texloc >= 0 )
                    {
                        // Remember that the Texture's type should be handled by the Shader itself. Here we just
                        // communicate the Texture Unit to the sampler variable.
                        
                        HardwareProgramVariable texunit;
                        texunit.location = texloc;
                        texunit.type = HdwProgVarType::Uint32;
                        texunit.value.uint32 = loc;
                        setUniformVariable(texunit);
                        
                        // As 'setUniformVariable' should not wait an update to set the Uniform , we should bind
                        // the Texture here and communicate the Texture object our Texture Unit.
                        
                        tex.bindWithTextureUnit(loc);
                    }
                }
                
                loc++;
            }
        }
        
        // Then, bind the Material Properties.
        
        const Color& ambient = material.getAmbient();
        int ambientloc = getUniformLocation("uAmbient");
        
        if ( ambientloc >= 0 )
        {
            HardwareProgramVariable ambientvar;
            ambientvar.location = ambientloc;
            ambientvar.type = HdwProgVarType::Vec4;
            ambientvar.value.vec4 = Vector4 (ambient.getRed(), ambient.getGreen(), ambient.getBlue(), ambient.getAlpha());
            setUniformVariable(ambientvar);
        }
        
        const Color& diffuse = material.getDiffuse();
        int diffuseloc = getUniformLocation("uDiffuse");
        
        if ( diffuseloc >= 0 )
        {
            HardwareProgramVariable diffusevar;
            diffusevar.location = diffuseloc;
            diffusevar.type = HdwProgVarType::Vec4;
            diffusevar.value.vec4 = Vector4 (diffuse.getRed(), diffuse.getGreen(), diffuse.getBlue(), diffuse.getAlpha());
            setUniformVariable(diffusevar);
        }
        
        const Color& specular = material.getSpecular();
        int specularloc = getUniformLocation("uSpecular");
        
        if ( specularloc >= 0 )
        {
            HardwareProgramVariable specularvar;
            specularvar.location = specularloc;
            specularvar.type = HdwProgVarType::Vec4;
            specularvar.value.vec4 = Vector4 (specular.getRed(), specular.getGreen(), specular.getBlue(), specular.getAlpha());
            setUniformVariable(specularvar);
        }
        
        const Color& emission = material.getEmission();
        int emissionloc = getUniformLocation("uEmission");
        
        if ( emissionloc >= 0 )
        {
            HardwareProgramVariable emissionvar;
            emissionvar.location = emissionloc;
            emissionvar.type = HdwProgVarType::Vec4;
            emissionvar.value.vec4 = Vector4 (emission.getRed(), emission.getGreen(), emission.getBlue(), emission.getAlpha());
            setUniformVariable(emissionvar);
        }
        
        float shininess = material.getShininess();
        int shininessloc = getUniformLocation("uShininess");
        
        if ( shininessloc >= 0 )
        {
            HardwareProgramVariable shininessvar;
            shininessvar.location = shininessloc;
            shininessvar.type = HdwProgVarType::Float1;
            shininessvar.value.float1 = shininess;
            setUniformVariable(shininessvar);
        }
    }
}

void HardwareProgramPrivate::unbindMaterial(const Gre::Material &material) const
{
    if ( !material.isInvalid() )
    {
        // Here, we just unbind the Texture's unit.
        
        if ( material.hasTexture() && !material.hasMultitexture() )
        {
            const Texture& tex = material.getTexture();
            
            if ( !tex.isInvalid() )
            {
                tex.activateTextureUnit(0);
                tex.unbind();
            }
        }
        
        if ( material.hasMultitexture() )
        {
            int texunit = 0;
            for ( const Texture& tex : material.getTextures() )
            {
                if ( !tex.isInvalid() )
                {
                    tex.activateTextureUnit(texunit);
                    tex.unbind();
                }
                
                texunit++;
            }
        }
    }
}

int HardwareProgramPrivate::getUniformLocation(const std::string &name) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
    return -1;
}

void HardwareProgramPrivate::setUniformVariable(const Gre::HardwareProgramVariable &var) const
{
#ifdef GreIsDebugMode
    GreDebugPretty() << "Not implemented." << std::endl;
#endif
}

// ---------------------------------------------------------------------------------------------------

HardwareProgram::HardwareProgram(const HardwareProgramPrivate* pointer)
: ResourceUser(pointer)
, SpecializedResourceUser<HardwareProgramPrivate>(pointer)
{
    
}

HardwareProgram::HardwareProgram(const HardwareProgramHolder& holder)
: ResourceUser(holder)
, SpecializedResourceUser<HardwareProgramPrivate>(holder)
{
    
}

HardwareProgram::HardwareProgram(const HardwareProgram& user)
: ResourceUser(user)
, SpecializedResourceUser<HardwareProgramPrivate>(user)
{
    
}

HardwareProgram::~HardwareProgram()
{
    
}

void HardwareProgram::bind() const
{
    auto ptr = lock();
    
    if(ptr)
        ptr->bind();
}

void HardwareProgram::unbind() const
{
    auto ptr = lock();
    
    if(ptr)
        ptr->unbind();
}

void HardwareProgram::finalize()
{
    auto ptr = lock();
    if(ptr)
        ptr->finalize();
}

bool HardwareProgram::isReady() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isReady();
    return false;
}

HardwareProgramVariables HardwareProgram::getVariables()
{
    auto ptr = lock();
    if(ptr)
        return ptr->getVariables();
    return HardwareProgramVariables::Empty;
}

void HardwareProgram::setUniformMat4(const std::string &name, const Matrix4 &mat4)
{
    auto ptr = lock();
    if(ptr)
        ptr->setUniformMat4(name, mat4);
}

void HardwareProgram::setUniform(const Gre::HardwareProgramVariable &var)
{
    auto ptr = lock();
    if(ptr)
        ptr->setUniform(var);
}

void HardwareProgram::unsetUniform(const std::string& name)
{
    auto ptr = lock();
    if(ptr)
        ptr->unsetUniform(name);
}

int HardwareProgram::getAttribLocation(const std::string& name) const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getAttribLocation(name);
    return -1;
}

bool HardwareProgram::isCompiled() const
{
    auto ptr = lock();
    if(ptr)
    {
        return ptr->isCompiled();
    }
    
    return false;
}

void HardwareProgram::reset()
{
    auto ptr = lock();
    if(ptr)
    {
        ptr->reset();
    }
}

bool HardwareProgram::hasOutputLocation(uint16_t location) const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->hasOutputLocation(location);
    return false;
}

void HardwareProgram::bindTextureUnit(int unit) const
{
    auto ptr = lock();
    if ( ptr )
        ptr->bindTextureUnit(unit);
}

void HardwareProgram::bindAttribsVertex(const Gre::VertexDescriptor &descriptor, const char* data) const
{
    auto ptr = lock();
    if ( ptr )
        ptr->bindAttribsVertex(descriptor, data);
}

int HardwareProgram::getAttribLocation(const Gre::VertexComponentType &component) const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getAttribLocation(component);
    return -1;
}

void HardwareProgram::setAttribData(int index, size_t stride, const char *data) const
{
    auto ptr = lock();
    if ( ptr )
        ptr->setAttribData(index, stride, data);
}

void HardwareProgram::bindAttributeLocation(const Gre::VertexComponentType &component, int index)
{
    auto ptr = lock();
    if ( ptr )
        ptr->bindAttributeLocation(component, index);
}

HardwareProgram HardwareProgram::Null = HardwareProgram(nullptr);

GreEndNamespace
