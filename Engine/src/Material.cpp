//////////////////////////////////////////////////////////////////////
//
//  Material.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 26/11/2015.
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

#include "Material.h"

GreBeginNamespace

Material::Material(const std::string& name)
: Gre::Resource(ResourceIdentifier::New() , name)
, iEmission(Color(1.0f, 1.0f, 1.0f, 1.0f))
, iAmbient(Color(1.0f, 1.0f, 1.0f, 1.0f))
, iDiffuse(Color(1.0f, 1.0f, 1.0f, 1.0f))
, iSpecular(Color(1.0f, 1.0f, 1.0f, 1.0f))
, iShininess(32.0f)
, iNormalMap(nullptr) , iNormalMapEnabled(false)
, iSpecularTexture(nullptr) , iSpecularTextureEnabled(false)
{
    
}

Material::~Material() noexcept ( false )
{
    
}

void Material::setTexture(const Gre::TextureUser &tex)
{
    if ( iTextures.size() )
    {
        iTextures.at(0) = tex;
    }
    
    else
    {
        iTextures.push_back(tex);
    }
}

void Material::setTexture(const Gre::TextureUser &tex, size_t index)
{
    if ( iTextures.size() > index )
    {
        iTextures.at(index) = tex;
    }
    
    else
    {
        for ( size_t i = iTextures.size(); i < index; ++i )
        {
            iTextures.push_back(TextureUser(nullptr));
        }
        
        iTextures.push_back(tex);
    }
}

const TextureUser Material::getTexture() const
{
    if ( iTextures.size() )
    {
        return iTextures.at(0);
    }
    
    else
    {
        return TextureUser(nullptr);
    }
}

const TextureUser Material::getTexture(size_t index) const
{
    if ( iTextures.size() > index )
    {
        return iTextures.at(index);
    }
    
    else
    {
        return TextureUser(nullptr);
    }
}

const std::vector< TextureUser > & Material::getTextures() const
{
    return iTextures;
}

void Material::clearTexture()
{
    clearTexture(0);
}

void Material::clearTexture(size_t index)
{
    if ( iTextures.size() > index )
    {
        TextureHolder holder = iTextures.at(index).lock() ;
        if ( !holder.isInvalid() ) holder->clear() ;
    }
}

void Material::clearTextures()
{
    for ( auto i = 0 ; i < iTextures.size() ; ++i ) clearTexture(i) ;
    iTextures.clear() ;
}

bool Material::hasTexture() const
{
    return iTextures.size() > 0;
}

bool Material::hasMultitexture() const
{
    return iTextures.size() > 1;
}

const Color& Material::getAmbient() const
{
    return iAmbient;
}

void Material::setAmbient(const Color &color)
{
    iAmbient = color;
}

const Color& Material::getDiffuse() const
{
    return iDiffuse;
}

void Material::setDiffuse(const Color &color)
{
    iDiffuse = color;
}

const Color& Material::getSpecular() const
{
    return iSpecular;
}

void Material::setSpecular(const Color &color)
{
    iSpecular = color;
}

const Color& Material::getEmission() const
{
    return iEmission;
}

void Material::setEmission(const Color &color)
{
    iEmission = color;
}

float Material::getShininess() const
{
    return iShininess;
}

void Material::setShininess(float f)
{
    iShininess = f;
}

void Material::configureProgram(const HardwareProgramHolder &program) const
{
    if ( !program.isInvalid() )
    {
        HardwareProgramVariable iAmbientColor ;
        iAmbientColor.name = "material.ambient" ;
        iAmbientColor.type = HdwProgVarType::Float4 ;
        iAmbientColor.value.f4 = iAmbient.toFloat4 () ;
        program->setVariable(iAmbientColor) ;
        
        HardwareProgramVariable iDiffuseColor ;
        iDiffuseColor.name = "material.diffuse" ;
        iDiffuseColor.type = HdwProgVarType::Float4 ;
        iDiffuseColor.value.f4 = iDiffuse.toFloat4 () ;
        program->setVariable(iDiffuseColor) ;
        
        HardwareProgramVariable iSpecularColor ;
        iSpecularColor.name = "material.specular" ;
        iSpecularColor.type = HdwProgVarType::Float4 ;
        iSpecularColor.value.f4 = iSpecular.toFloat4 () ;
        program->setVariable(iSpecularColor) ;
        
        HardwareProgramVariable iEmissionColor ;
        iEmissionColor.name = "material.emission" ;
        iEmissionColor.type = HdwProgVarType::Float4 ;
        iEmissionColor.value.f4 = iEmission.toFloat4 () ;
        program->setVariable(iEmissionColor) ;
        
        HardwareProgramVariable iShininessVar ;
        iShininessVar.name = "material.shininess" ;
        iShininessVar.type = HdwProgVarType::Float1 ;
        iShininessVar.value.f1 = iShininess ;
        program->setVariable(iShininessVar) ;
        
        // If we have multiple Color Textures , we should specify the number  of
        // Color Textures to the shader.
        
        HardwareProgramVariable iMultitextureBool ;
        iMultitextureBool.name = "iMultitexture" ;
        iMultitextureBool.type = HdwProgVarType::Int1 ;
        iMultitextureBool.value.i1 = iTextures.size() ;
        program->setVariable(iMultitextureBool) ;
        
        int textureunit = 0 ;
        
        for ( const TextureUser & texture : iTextures )
        {
            if ( !texture.isInvalid() )
            {
                const TextureHolder holder = texture.lock() ;
                
                program -> bindTextureUnit ( textureunit ) ;
                holder -> bind() ;
                
                HardwareProgramVariable iTexture ;
                iTexture.name = std::string ("iTexture") + std::to_string(textureunit) ;
                iTexture.type = HdwProgVarType::Int1 ;
                iTexture.value.i1 = textureunit ;
                program -> setVariable(iTexture) ;
                
                textureunit ++ ;
            }
        }
        
        // Specify the normal map if we have one.
        
        if ( iNormalMapEnabled )
        {
            HardwareProgramVariable NormalMapEnabled ;
            NormalMapEnabled.name = "material.normalmap_enabled" ;
            NormalMapEnabled.type = HdwProgVarType::Int1 ;
            NormalMapEnabled.value.i1 = 1 ;
            program -> setVariable(NormalMapEnabled) ;
            
            if ( !iNormalMap.isInvalid() )
            {
                const TextureHolder normalmap = iNormalMap.lock() ;
                
                program -> bindTextureUnit(textureunit) ;
                normalmap -> bind() ;
                
                NormalMapEnabled.name = "material.normalmap" ;
                NormalMapEnabled.type = HdwProgVarType::Int1 ;
                NormalMapEnabled.value.i1 = textureunit ;
                program -> setVariable(NormalMapEnabled) ;
                
                textureunit ++ ;
            }
        }
        
        //////////////////////////////////////////////////////////////////////
        // Specular Texture Enabling.
        
        if ( iSpecularTextureEnabled )
        {
            HardwareProgramVariable specular ;
            specular.name = "material.hasSpecularMap" ;
            specular.type = HdwProgVarType::Int1 ;
            specular.value.i1 = 1 ;
            program -> setVariable(specular) ;
            
            if ( !iSpecularTexture.isInvalid() )
            {
                specular.name = "material.specularMap" ;
                specular.type = HdwProgVarType::Int1 ;
                specular.value.i1 = textureunit ;
                program -> setVariable(specular) ;
                
                textureunit ++ ;
            }
        }
    }
}

const TextureUser & Material::getNormalMap() const
{
    GreAutolock ; return iNormalMap ;
}

void Material::setNormalMap(const TextureUser &texture)
{
    GreAutolock ; iNormalMap = texture ;
}

bool Material::isNormalMapEnabled() const
{
    GreAutolock ; return iNormalMapEnabled ;
}

void Material::setNormalMapEnabled(bool value)
{
    GreAutolock ; iNormalMapEnabled = value ;
}

const TextureUser & Material::getSpecularTexture() const
{
    GreAutolock ; return iSpecularTexture ;
}

void Material::setSpecularTexture(const TextureUser &texture)
{
    GreAutolock ; iSpecularTexture = texture ;
}

bool Material::isSpecularTextureEnabled() const
{
    GreAutolock ; return iSpecularTextureEnabled ;
}

void Material::setSpecularTextureEnabled(bool value)
{
    GreAutolock ; iSpecularTextureEnabled = value ;
}

// ---------------------------------------------------------------------------------------------------

MaterialLoader::MaterialLoader()
{
    
}

MaterialLoader::~MaterialLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

MaterialManager::MaterialManager(const std::string& name)
: Gre::Resource(name)
{
    // Always add a 'Default' Material object. This object can be used to create SoftwareIndexBuffer without
    // specific Materials.
    
    MaterialHolder holder ( new Material("Default") );
    
    if ( holder.isInvalid() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Can't load Material 'Default'." << Gre::gendl;
#endif
        throw GreConstructorException ( "MaterialManager" , "'Default' Material can't be loaded." );
    }
    
    load(holder);
}

MaterialManager::~MaterialManager()
{
    
}

MaterialHolder MaterialManager::create(const std::string &name)
{
    GreAutolock ;
    
    MaterialHolder value = MaterialHolder ( new Material(name) ) ;
    load ( value ) ;
    
    return value ;
}

MaterialVector MaterialManager::load(const std::string &filepath)
{
    if ( !filepath.empty() )
    {
        // We must find a MaterialLoader able to load this file.
        
        for ( auto it = iFactory.getLoaders().begin(); it != iFactory.getLoaders().end(); it++ )
        {
            MaterialLoader* loader = it->second.get();
            
            if ( loader )
            {
                if ( loader->isLoadable(filepath) )
                {
                    // Found a loader. Load the Materials.
                    
                    MaterialHolderList materials = loader->load(filepath);
                    
                    if ( !materials.empty() )
                    {
                        // Registers every Loader to the Manager and to the return Vector.
                        
                        MaterialVector ret;
                        
                        for ( MaterialHolder& holder : materials )
                        {
                            iMaterials.add(holder);
                            ret.push_back(MaterialUser(holder));
                        }
                        
                        return ret;
                    }
                    
                    else
                    {
#ifdef GreIsDebugMode
                        GreDebugPretty() << "File '" << filepath << "' doesn't seem to contain any Material." << Gre::gendl;
#endif
                        return MaterialVector();
                    }
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "File '" << filepath << "' is not loadable by any MaterialLoader." << Gre::gendl;
#endif
        return MaterialVector();
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'filepath' is empty." << Gre::gendl;
#endif
        return MaterialVector();
    }
}

MaterialUser MaterialManager::load(const MaterialHolder &material)
{
    if ( !material.isInvalid() )
    {
        MaterialUser mat = get(material->getName());
        
        if ( mat.isInvalid() )
        {
            iMaterials.add(material);
            return MaterialUser(material);
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Material '" << material->getName() << "' is already loaded in MaterialManager." << Gre::gendl;
#endif
            return MaterialUser(nullptr);
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'material' is invalid." << Gre::gendl;
#endif
        return MaterialUser(nullptr);
    }
}

MaterialUser MaterialManager::get(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( MaterialHolder& holder : iMaterials )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return MaterialUser(holder);
                }
            }
        }

        return MaterialUser(nullptr);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return MaterialUser(nullptr);
    }
}

const MaterialUser MaterialManager::get(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( const MaterialHolder& holder : iMaterials )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return MaterialUser(holder);
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Material '" << name << "' not found." << Gre::gendl;
#endif
        return MaterialUser(nullptr);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
        return MaterialUser(nullptr);
    }
}

void MaterialManager::remove(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( auto it = iMaterials.begin(); it != iMaterials.end(); it++ )
        {
            if ( !(*it).isInvalid() )
            {
                if ( (*it)->getName() == name )
                {
                    iMaterials.erase(it);
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Material '" << name << "' not found." << Gre::gendl;
#endif
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << Gre::gendl;
#endif
    }
}

void MaterialManager::clearMaterials()
{
    iMaterials.clear();
}

MaterialLoaderFactory& MaterialManager::getMaterialLoaderFactory()
{
    return iFactory;
}

const MaterialLoaderFactory& MaterialManager::getMaterialLoaderFactory() const
{
    return iFactory;
}

void MaterialManager::clear()
{
    clearMaterials();
    iFactory.clear();
}

GreEndNamespace
