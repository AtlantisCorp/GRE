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

MaterialPrivate::MaterialPrivate(const std::string& name)
: Gre::Resource(name)
, iAmbient(Color::Black)
, iDiffuse(Color::Black)
, iSpecular(Color::Black)
, iEmission(Color::Black)
, iShininess(0.0f)
{
    
}

MaterialPrivate::~MaterialPrivate()
{
    
}

void MaterialPrivate::setTexture(const Gre::Texture &tex)
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

void MaterialPrivate::setTexture(const Gre::Texture &tex, size_t index)
{
    if ( iTextures.size() > index )
    {
        iTextures.at(index) = tex;
    }
    
    else
    {
        for ( int i = (int) iTextures.size(); i < index; ++i )
        {
            iTextures.push_back(Texture(nullptr));
        }
        
        iTextures.push_back(tex);
    }
}

const Texture& MaterialPrivate::getTexture() const
{
    if ( iTextures.size() )
    {
        return iTextures.at(0);
    }
    
    else
    {
        return Texture::Null;
    }
}

const Texture& MaterialPrivate::getTexture(size_t index) const
{
    if ( iTextures.size() > index )
    {
        return iTextures.at(index);
    }
    
    else
    {
        return Texture::Null;
    }
}

const TextureVector& MaterialPrivate::getTextures() const
{
    return iTextures;
}

void MaterialPrivate::clearTexture()
{
    if ( iTextures.size() )
    {
        iTextures.at(0).reset();
    }
}

void MaterialPrivate::clearTexture(size_t index)
{
    if ( iTextures.size() > index )
    {
        iTextures.at(0).reset();
    }
}

void MaterialPrivate::clearTextures()
{
    iTextures.clear();
}

bool MaterialPrivate::hasTexture() const
{
    return iTextures.size() > 0;
}

bool MaterialPrivate::hasMultitexture() const
{
    return iTextures.size() > 1;
}

const Color& MaterialPrivate::getAmbient() const
{
    return iAmbient;
}

void MaterialPrivate::setAmbient(const Color &color)
{
    iAmbient = color;
}

const Color& MaterialPrivate::getDiffuse() const
{
    return iDiffuse;
}

void MaterialPrivate::setDiffuse(const Color &color)
{
    iDiffuse = color;
}

const Color& MaterialPrivate::getSpecular() const
{
    return iSpecular;
}

void MaterialPrivate::setSpecular(const Color &color)
{
    iSpecular = color;
}

const Color& MaterialPrivate::getEmission() const
{
    return iEmission;
}

void MaterialPrivate::setEmission(const Color &color)
{
    iEmission = color;
}

float MaterialPrivate::getShininess() const
{
    return iShininess;
}

void MaterialPrivate::setShininess(float f)
{
    iShininess = f;
}

// ---------------------------------------------------------------------------------------------------

Material::Material(const MaterialPrivate* pointer)
: Gre::ResourceUser(pointer)
, SpecializedResourceUser<Gre::MaterialPrivate>(pointer)
{
    
}

Material::Material(const MaterialHolder& holder)
: Gre::ResourceUser(holder)
, SpecializedResourceUser<Gre::MaterialPrivate>(holder)
{
    
}

Material::Material(const Material& user)
: Gre::ResourceUser(user)
, SpecializedResourceUser<Gre::MaterialPrivate>(user)
{
    
}

Material::~Material()
{
    
}

void Material::setTexture(const Gre::Texture &tex)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setTexture(tex);
}

void Material::setTexture(const Gre::Texture &tex, size_t index)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setTexture(tex, index);
}

const Texture& Material::getTexture() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getTexture();
    throw GreInvalidUserException("Material");
}

const Texture& Material::getTexture(size_t index) const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getTexture(index);
    throw GreInvalidUserException("Material");
}

const TextureVector& Material::getTextures() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getTextures();
    throw GreInvalidUserException("Material");
}

void Material::clearTexture()
{
    auto ptr = lock();
    if ( ptr )
        ptr->clearTexture();
}

void Material::clearTexture(size_t index)
{
    auto ptr = lock();
    if ( ptr )
        ptr->clearTexture(index);
}

void Material::clearTextures()
{
    auto ptr = lock();
    if ( ptr )
        ptr->clearTextures();
}

bool Material::hasTexture() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->hasTexture();
    throw GreInvalidUserException("Material");
}

bool Material::hasMultitexture() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->hasMultitexture();
    throw GreInvalidUserException("Material");
}

const Color& Material::getAmbient() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getAmbient();
    throw GreInvalidUserException("Material");
}

void Material::setAmbient(const Color &color)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setAmbient(color);
}

const Color& Material::getDiffuse() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getDiffuse();
    throw GreInvalidUserException("Material");
}

void Material::setDiffuse(const Color &color)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setDiffuse(color);
}

const Color& Material::getSpecular() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getSpecular();
    throw GreInvalidUserException("Material");
}

void Material::setSpecular(const Color &color)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setSpecular(color);
}

const Color& Material::getEmission() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getEmission();
    throw GreInvalidUserException("Material");
}

void Material::setEmission(const Color &color)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setEmission(color);
}

float Material::getShininess() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getShininess();
    throw GreInvalidUserException("Material");
}

void Material::setShininess(float f)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setShininess(f);
}

Material Material::Null = Material(nullptr);

// ---------------------------------------------------------------------------------------------------

MaterialLoader::MaterialLoader()
{
    
}

MaterialLoader::~MaterialLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

MaterialManager::MaterialManager()
{
    
}

MaterialManager::~MaterialManager()
{
    
}

MaterialVector MaterialManager::load(const std::string &filepath)
{
    if ( !filepath.empty() )
    {
        // We must find a MaterialLoader able to load this file.
        
        for ( auto it = iFactory.getLoaders().begin(); it != iFactory.getLoaders().end(); it++ )
        {
            auto loader = it->second;
            
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
                            ret.push_back(Material(holder));
                        }
                        
                        return ret;
                    }
                    
                    else
                    {
#ifdef GreIsDebugMode
                        GreDebugPretty() << "File '" << filepath << "' doesn't seem to contain any Material." << std::endl;
#endif
                        return MaterialVector();
                    }
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "File '" << filepath << "' is not loadable by any MaterialLoader." << std::endl;
#endif
        return MaterialVector();
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'filepath' is empty." << std::endl;
#endif
        return MaterialVector();
    }
}

Material MaterialManager::load(const MaterialHolder &material)
{
    if ( !material.isInvalid() )
    {
        Material mat = get(material->getName());
        
        if ( mat.isInvalid() )
        {
            iMaterials.add(material);
            return Material(material);
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Material '" << material->getName() << "' is already loaded in MaterialManager." << std::endl;
#endif
            return Material::Null;
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'material' is invalid." << std::endl;
#endif
        return Material::Null;
    }
}

Material MaterialManager::get(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( MaterialHolder& holder : iMaterials )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Material(holder);
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Material '" << name << "' not found." << std::endl;
#endif
        return Material::Null;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Material::Null;
    }
}

const Material MaterialManager::get(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( const MaterialHolder& holder : iMaterials )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Material(holder);
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Material '" << name << "' not found." << std::endl;
#endif
        return Material::Null;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
#endif
        return Material::Null;
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
        GreDebugPretty() << "Material '" << name << "' not found." << std::endl;
#endif
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' is empty." << std::endl;
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
