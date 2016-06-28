//////////////////////////////////////////////////////////////////////
//
//  HardwareShader.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 09/01/2016.
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

#include "HardwareShader.h"

GreBeginNamespace

HardwareShaderPrivate::HardwareShaderPrivate(const std::string& name, const ShaderType& type)
: Resource(name), iType(type), iSource(), iCompiled(false)
{
    
}

HardwareShaderPrivate::HardwareShaderPrivate(const std::string& name, const ShaderType& type, const std::string& text)
: Resource(name), iType(type), iSource(text), iCompiled(false)
{

}

HardwareShaderPrivate::~HardwareShaderPrivate()
{
    
}

ShaderType HardwareShaderPrivate::getType() const
{
    return iType;
}

void HardwareShaderPrivate::setSource(const std::string &source)
{
    iSource = source;
    iCompiled = false;
}

const std::string& HardwareShaderPrivate::getSource() const
{
    return iSource;
}

bool HardwareShaderPrivate::compile() const
{
    return iCompiled;
}

bool HardwareShaderPrivate::isCompiled() const
{
    return iCompiled;
}

void HardwareShaderPrivate::clear()
{
    iSource.clear();
    iCompiled = false;
}

// ---------------------------------------------------------------------------------------------------

HardwareShader::HardwareShader(const HardwareShaderPrivate* pointer)
: SpecializedResourceUser<HardwareShaderPrivate>(pointer)
{
    
}

HardwareShader::HardwareShader(const HardwareShaderHolder& holder)
: SpecializedResourceUser<HardwareShaderPrivate>(holder)
{
    
}

HardwareShader::HardwareShader(const HardwareShader& user)
: SpecializedResourceUser<HardwareShaderPrivate>(user)
{
    
}

HardwareShader::~HardwareShader()
{
    
}

ShaderType HardwareShader::getType() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getType();
    return ShaderType::Null;
}

void HardwareShader::setSource(const std::string &source)
{
    auto ptr = lock();
    if(ptr)
        ptr->setSource(source);
}

const std::string HardwareShader::getSource() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->getSource();
    return std::string();
}

bool HardwareShader::compile() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->compile();
    return false;
}

bool HardwareShader::isCompiled() const
{
    auto ptr = lock();
    if(ptr)
        return ptr->isCompiled();
    return false;
}

void HardwareShader::clear()
{
    auto ptr = lock();
    if(ptr)
        ptr->clear();
}

// ---------------------------------------------------------------------------------------------------

HardwareShaderLoader::HardwareShaderLoader()
{
    
}

HardwareShaderLoader::~HardwareShaderLoader()
{
    
}

bool HardwareShaderLoader::isTypeSupported(Resource::Type type) const
{
    return type == Resource::Type::HdwShader;
}

Resource* HardwareShaderLoader::load(Resource::Type type, const std::string &name, const std::string& filepath, const Gre::ShaderType &stype) const
{
    GreDebugFunctionNotImplemented();
    return nullptr;
}

HardwareShader HardwareShader::Null = HardwareShader(nullptr);
HardwareShader HardwareShader::VertexPassThrough = HardwareShader(nullptr);
HardwareShader HardwareShader::FragmentPassThrough = HardwareShader(nullptr);

GreEndNamespace
