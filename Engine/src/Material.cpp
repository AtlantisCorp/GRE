//////////////////////////////////////////////////////////////////////
//
//  Material.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
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
#include "ResourceManager.h"

GreBeginNamespace

Material::Material(const std::string& name) : Gre::Renderable(name)
, iEmission(Color(1.0f, 1.0f, 1.0f, 1.0f))
{
    //////////////////////////////////////////////////////////////////////
    // Loads default color values.

    iAmbient = { 1.0f , 1.0f , 1.0f } ;
    iDiffuse = { 1.0f , 1.0f , 1.0f } ;
    iSpecular = { 1.0f , 1.0f , 1.0f } ;
    iShininess = 32.0f ;

    //////////////////////////////////////////////////////////////////////
    // Initializes every textures to null , making the map complete. Notes
    // no texturing is enabled. When enabling texturing, 'clearTextures()'
    // should set every textures to the default blank one.

    for ( int i = (int) TechniqueParam::Texture0 ; i < (int) TechniqueParam::Texture9 ; ++i )
        iTextures [(TechniqueParam)i] = nullptr ;

    iTextures [TechniqueParam::MaterialTexAmbient] = nullptr ;
    iTextures [TechniqueParam::MaterialTexDiffuse] = nullptr ;
    iTextures [TechniqueParam::MaterialTexSpecular] = nullptr ;
    iTextures [TechniqueParam::MaterialTexNormal] = nullptr ;
    iTextures [TechniqueParam::LightTexture0] = nullptr ;
    iTextures [TechniqueParam::LightTexture1] = nullptr ;
    iTextures [TechniqueParam::LightTexture2] = nullptr ;
    iTextures [TechniqueParam::LightTexture3] = nullptr ;

    iUseTextures = false ;
    iEmissive = false ;
}

Material::~Material() noexcept ( false )
{

}

void Material::setTexture ( const TechniqueParam & param , const TextureHolder & tex )
{
    GreAutolock ;

    if ( !tex.isInvalid() )
    {
        iTextures [param] = tex ;
    }
}

const TextureHolder & Material::getTexture(const Gre::TechniqueParam &param) const
{
    GreAutolock ; return iTextures.at(param) ;
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

const TextureHolder & Material::getAmbientTexture() const
{
    GreAutolock ; return iTextures.at(TechniqueParam::MaterialTexAmbient) ;
}

void Material::setAmbientTexture(const TextureHolder &tex)
{
    GreAutolock ; iTextures[TechniqueParam::MaterialTexAmbient] = tex ;
}

const TextureHolder & Material::getDiffuseTexture() const
{
    GreAutolock ; return iTextures.at(TechniqueParam::MaterialTexDiffuse) ;
}

void Material::setDiffuseTexture(const TextureHolder &texture)
{
    GreAutolock ; iTextures[TechniqueParam::MaterialTexDiffuse] = texture ;
}

const TextureHolder & Material::getSpecularTexture() const
{
    GreAutolock ; return iTextures.at(TechniqueParam::MaterialDiffuse) ;
}

void Material::setSpecularTexture(const TextureHolder &texture)
{
    GreAutolock ; iTextures[TechniqueParam::MaterialTexSpecular] = texture ;
}

const TextureHolder & Material::getNormalTexture() const
{
    GreAutolock ; return iTextures.at(TechniqueParam::MaterialTexNormal) ;
}

void Material::setNormalTexture(const TextureHolder &texture)
{
    GreAutolock ; iTextures[TechniqueParam::MaterialTexNormal] = texture ;
}

void Material::use(const TechniqueHolder &technique) const
{
    GreAutolock ;

    if ( !technique.isInvalid() )
    {
        TechniqueParam alias ;
        
        //////////////////////////////////////////////////////////////////////
        // Binds the parameters differently if this material is an emissive
        // or normal one. (Emissive is for light use)
        
        if ( emissive() )
        {
            alias = technique -> getNextLightAlias() ;
            technique -> setAliasedParameterStructValue(alias, TechniqueParam::LightAmbient, HdwProgVarType::Float3, iAmbient.toFloat3());
            technique -> setAliasedParameterStructValue(alias, TechniqueParam::LightDiffuse, HdwProgVarType::Float3, iDiffuse.toFloat3());
            technique -> setAliasedParameterStructValue(alias, TechniqueParam::LightSpecular, HdwProgVarType::Float3, iSpecular.toFloat3());
        }
        else
        {
            alias = TechniqueParam::None ;
            technique -> setAliasedParameterStructValue(alias, TechniqueParam::MaterialAmbient, HdwProgVarType::Float3, iAmbient.toFloat3());
            technique -> setAliasedParameterStructValue(alias, TechniqueParam::MaterialDiffuse, HdwProgVarType::Float3, iDiffuse.toFloat3());
            technique -> setAliasedParameterStructValue(alias, TechniqueParam::MaterialSpecular, HdwProgVarType::Float3, iSpecular.toFloat3());
            technique -> setAliasedParameterStructValue(alias, TechniqueParam::MaterialShininess, HdwProgVarType::Float1, iShininess);
        }

        //////////////////////////////////////////////////////////////////////
        // As textures can be directly bound to LightTexturei by the user ,
        // we don't need to check for emissive or not material. However , we should
        // use the two alias functions to bind those textures.
        
        if ( iUseTextures )
        {
            for ( auto it : iTextures )
            if ( !it.second.isInvalid() )
            technique -> setAliasedTextureStruct(alias, it.first, it.second) ;
        }
    }
}

void Material::setUseTextures ( bool value )
{
    GreAutolock ;

    if ( !iUseTextures && value )
    clearTextures () ;

    iUseTextures = value ;
}

bool Material::usesTextures () const
{
    GreAutolock ; return iUseTextures ;
}

void Material::clearTextures ()
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // For every Textures that can be bound to a Technique , sets the default
    // blank texture provided by the TextureManager.

    TextureManagerHolder textmanager = ResourceManager::Get() -> getTextureManager () ;

    if ( textmanager.isInvalid() )
    {
        GreDebug ( "[WARN] Material '" ) << getName() << "' can't clear Textures because no Texture Manager is installed." << gendl ;
        return ;
    }

    TextureHolder blanktext = textmanager -> getDefaultTexture () ;

    if ( blanktext.isInvalid() )
    {
        GreDebug ( "[WARN] Material '" ) << getName() << "' can't clear Textures because no Default Texture is loaded." << gendl ;
        return ;
    }

    for ( auto & it : iTextures )
    it.second = blanktext ;
}

bool Material::emissive () const
{
    GreAutolock ; return iEmissive ;
}

void Material::setEmissive ( bool value )
{
    GreAutolock ; iEmissive = value ;
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
: SpecializedResourceManager<Gre::Material, Gre::MaterialLoader>()
{

}

MaterialManager::~MaterialManager() noexcept ( false )
{

}

MaterialHolder MaterialManager::loadBlank(const std::string &name)
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks input values.

    if ( name.empty() )
    return MaterialHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Checks if material doesn't exists.

    MaterialHolder material = get ( name ) ;

    if ( material.isInvalid() )
    return loadHolder ( MaterialHolder ( new Material (name) ) ) ;

    material -> clear () ;
    return material ;
}

MaterialHolder MaterialManager::get(const std::string &name)
{
    GreAutolock ; return findFirstHolder ( name ) ;
}

MaterialHolder MaterialManager::loadHolder ( const MaterialHolder & material )
{
    GreAutolock ;

    if ( material.isInvalid() )
    return MaterialHolder ( nullptr ) ;

    if ( findHolder(material->getIdentifier()).isInvalid() )
    iHolders.add(material);

    return material ;
}

GreEndNamespace
