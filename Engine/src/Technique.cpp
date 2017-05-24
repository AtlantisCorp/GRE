//////////////////////////////////////////////////////////////////////
//
//  Technique.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 08/02/2017.
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

#include "Technique.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "TechniqueFilePreprocessor.h"
#include "TechniqueFileParser.h"

GreBeginNamespace

// -----------------------------------------------------------------------------

TechniqueLightingMode TechniqueLightingModeFromString ( const std::string & mode )
{
    if ( mode == "PerLight" ) return TechniqueLightingMode::PerLight ;
    if ( mode == "AllLights" ) return TechniqueLightingMode::AllLights ;
    return TechniqueLightingMode::None ;
}

TechniqueParam TechniqueParamFromString ( const std::string & p )
{
    if ( p == "ModelMatrix" ) return TechniqueParam::ModelMatrix ;
    if ( p == "ViewMatrix" ) return TechniqueParam::ViewMatrix ;
    if ( p == "ProjectionMatrix" ) return TechniqueParam::ProjectionMatrix ;
    if ( p == "ProjectionViewMatrix" ) return TechniqueParam::ProjectionViewMatrix ;
    if ( p == "NormalMatrix" ) return TechniqueParam::NormalMatrix ;
    if ( p == "NormalMatrix3" ) return TechniqueParam::NormalMatrix3 ;

    if ( p == "CameraPosition" ) return TechniqueParam::CameraPosition ;
    if ( p == "CameraDirection" ) return TechniqueParam::CameraDirection ;

    if ( p == "ViewportWidth" ) return TechniqueParam::ViewportWidth ;
    if ( p == "ViewportHeight" ) return TechniqueParam::ViewportHeight ;
    if ( p == "ViewportLeft" ) return TechniqueParam::ViewportLeft ;
    if ( p == "ViewportTop" ) return TechniqueParam::ViewportTop ;

    if ( p == "ClearColor" ) return TechniqueParam::ClearColor ;
    if ( p == "ClearDepth" ) return TechniqueParam::ClearDepth ;

    if ( p == "Light0" ) return TechniqueParam::Light0 ;
    if ( p == "Light1" ) return TechniqueParam::Light1 ;
    if ( p == "Light2" ) return TechniqueParam::Light2 ;
    if ( p == "Light3" ) return TechniqueParam::Light3 ;
    if ( p == "Light4" ) return TechniqueParam::Light4 ;
    if ( p == "Light5" ) return TechniqueParam::Light5 ;
    if ( p == "Light6" ) return TechniqueParam::Light6 ;
    if ( p == "Light7" ) return TechniqueParam::Light7 ;
    if ( p == "Light8" ) return TechniqueParam::Light8 ;
    if ( p == "Light9" ) return TechniqueParam::Light9 ;

    if ( p == "LightAmbient" ) return TechniqueParam::LightAmbient ;
    if ( p == "LightDiffuse" ) return TechniqueParam::LightDiffuse ;
    if ( p == "LightSpecular" ) return TechniqueParam::LightSpecular ;
    if ( p == "LightPosition" ) return TechniqueParam::LightPosition ;
    if ( p == "LightDirection" ) return TechniqueParam::LightDirection ;
    if ( p == "LightAttCst" ) return TechniqueParam::LightAttCst ;
    if ( p == "LightAttLine" ) return TechniqueParam::LightAttLine ;
    if ( p == "LightAttQuad" ) return TechniqueParam::LightAttQuad ;
    if ( p == "LightSpotAngle" ) return TechniqueParam::LightSpotAngle ;
    if ( p == "LightSpotExposition" ) return TechniqueParam::LightSpotExposition ;
    if ( p == "LightShadowMatrix" ) return TechniqueParam::LightShadowMatrix ;
    if ( p == "LightTexture0" ) return TechniqueParam::LightTexture0 ;
    if ( p == "LightTexture1" ) return TechniqueParam::LightTexture1 ;
    if ( p == "LightTexture2" ) return TechniqueParam::LightTexture2 ;
    if ( p == "LightTexture3" ) return TechniqueParam::LightTexture3 ;

    if ( p == "Texture0" ) return TechniqueParam::Texture0 ;
    if ( p == "Texture1" ) return TechniqueParam::Texture1 ;
    if ( p == "Texture3" ) return TechniqueParam::Texture3 ;
    if ( p == "Texture4" ) return TechniqueParam::Texture4 ;
    if ( p == "Texture5" ) return TechniqueParam::Texture5 ;
    if ( p == "Texture6" ) return TechniqueParam::Texture6 ;
    if ( p == "Texture7" ) return TechniqueParam::Texture7 ;
    if ( p == "Texture8" ) return TechniqueParam::Texture8 ;
    if ( p == "Texture9" ) return TechniqueParam::Texture9 ;

    if ( p == "MaterialAmbient" ) return TechniqueParam::MaterialAmbient ;
    if ( p == "MaterialDiffuse" ) return TechniqueParam::MaterialDiffuse ;
    if ( p == "MaterialSpecular" ) return TechniqueParam::MaterialSpecular ;
    if ( p == "MaterialTexAmbient" ) return TechniqueParam::MaterialTexAmbient ;
    if ( p == "MaterialTexDiffuse" ) return TechniqueParam::MaterialTexDiffuse ;
    if ( p == "MaterialTexSpecular" ) return TechniqueParam::MaterialTexSpecular ;
    if ( p == "MaterialTexNormal" ) return TechniqueParam::MaterialTexNormal ;
    if ( p == "MaterialShininess" ) return TechniqueParam::MaterialShininess ;

    //////////////////////////////////////////////////////////////////////
    // We should never go here but well , that could be.
    return (TechniqueParam) 0 ;
}

HdwProgVarType HdwProgVarTypeFromTextureType ( const TextureType & type )
{
    if ( type == TextureType::Texture1D ) return HdwProgVarType::Sampler1D ;
    else if ( type == TextureType::Texture2D ) return HdwProgVarType::Sampler2D ;
    else if ( type == TextureType::Texture3D ) return HdwProgVarType::Sampler3D ;
    else if ( type == TextureType::CubeMap ) return HdwProgVarType::SamplerCube ;

    return HdwProgVarType::None ;
}

// -----------------------------------------------------------------------------

Technique::Technique ( const std::string & name )
: Gre::Resource(name)
{
    iLightingMode = TechniqueLightingMode::AllLights ;
    iCurrentLight = -1 ;
    iSelfRendered = false ;

    // Loads always the null framebuffer.

    auto framebuffers = ResourceManager::Get () -> getFramebufferManager () ;

    if ( !framebuffers.isInvalid() )
    iFramebuffer = framebuffers -> getNull() ;
}

Technique::~Technique() noexcept ( false )
{

}

const HardwareProgramHolder & Technique::getHardwareProgram () const
{
    GreAutolock ; return iProgram ;
}

void Technique::setHardwareProgram ( const HardwareProgramHolder& program )
{
    GreAutolock ; iProgram = program ;
}

TechniqueLightingMode Technique::getLightingMode () const
{
    GreAutolock ; return iLightingMode ;
}

void Technique::setLightingMode(const Gre::TechniqueLightingMode &lightingmode)
{
    GreAutolock ; iLightingMode = lightingmode ;
}

const RenderFramebufferHolder & Technique::getFramebuffer() const
{
    GreAutolock ; return iFramebuffer ;
}

void Technique::setFramebuffer(const RenderFramebufferHolder &framebuffer)
{
    GreAutolock ; iFramebuffer = framebuffer ;
}

void Technique::setAlias ( const TechniqueParam & param , const std::string & alias )
{
    GreAutolock ; iAliases[param] = alias ;
}

std::string Technique::getAlias ( const TechniqueParam & param ) const
{
    GreAutolock ; auto it = iAliases.find(param) ;

    if ( it != iAliases.end() ) return it->second ;
    return std::string () ;
}

void Technique::setAliasedParameterStructValue (const TechniqueParam & alias1 ,
                                                const TechniqueParam & alias2 ,
                                                const HdwProgVarType & type ,
                                                const RealProgramVariable & value ) const
{
    GreAutolock ;

    if ( iProgram.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Checks both aliases. If both are empty , don't send . If one is empty ,
    // sends to second.

    std::string name1 = getAlias ( alias1 ) ;
    std::string name2 = getAlias ( alias2 ) ;

    if ( name1.empty() )
    {
        setAliasedParameterValue(alias2, type, value) ;
        return ;
    }

    //////////////////////////////////////////////////////////////////////
    // If program is bound , sends the value to the shader object.

    if ( !iProgram->binded() )
    return ;

    iProgram -> setUniform ( name1 + "." + name2 , type , value ) ;
}

void Technique::setAliasedParameterValue (const TechniqueParam & name ,
                                          const HdwProgVarType & type ,
                                          const RealProgramVariable & value) const
{
    GreAutolock ;

    if ( iProgram.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Checks for an eventual 'GlobSet' action.

    auto globset = findGlobSets ( name ) ;

    for ( auto globsetit : globset )
    ResourceManager::Get() -> getTechniqueManager () -> setGlobalValue ( globsetit , value ) ;

    //////////////////////////////////////////////////////////////////////
    // Now see if the program has the given parameter registered.

    if ( iProgram->binded() )
    {
        auto alias = getAlias (name) ;
        if ( alias.empty() ) return ;

        iProgram -> setUniform ( alias , type , value ) ;
    }
}

void Technique::setNamedParameterValue (const std::string & name ,
                                        const HdwProgVarType & type ,
                                        const RealProgramVariable & value) const
{
    GreAutolock ;

    if ( iProgram.isInvalid() )
        return ;

    if ( iProgram->binded() && !name.empty() ) {
        iProgram -> setUniform ( name , type , value ) ;
    }
}

void Technique::bind () const
{
    GreAutolock ;

    if ( !iFramebuffer.isInvalid() )
    iFramebuffer -> bind () ;

    //////////////////////////////////////////////////////////////////////
    // If possible , binds the HardwareProgram associated.

    if ( !iProgram.isInvalid() )
    {
        iProgram -> use () ;

        //////////////////////////////////////////////////////////////////////
        // If bound , we must check the globals associated with this technique.

        if ( iProgram -> binded() )
        {
            auto techmanager = ResourceManager::Get() -> getTechniqueManager() ;

            for ( auto globalias : iGlobALiases )
            {
                auto global = techmanager -> getGlobal ( globalias.first ) ;
                iProgram -> setUniform ( globalias.second , global.type , global.value ) ;
            }
        }
    }
}

void Technique::unbind () const
{
    GreAutolock ;

    if ( !iProgram.isInvalid() )
    iProgram -> unuse () ;

    if ( !iFramebuffer.isInvalid() )
    iFramebuffer -> unbind () ;
}

TechniqueParam Technique::getNextLightAlias () const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Computes the alias depending on current light counter.

    TechniqueParam alias = (TechniqueParam) ((int)TechniqueParam::Light0 + iCurrentLight + 1) ;
    iCurrentLight ++ ;

    return alias ;
}

TechniqueParam Technique::getCurrentLightAlias () const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // If the alias is '-1' , we always return the first light.

    if ( iCurrentLight < 0 )
    return TechniqueParam::Light0 ;

    return (TechniqueParam) ((int)TechniqueParam::Light0 + iCurrentLight) ;
}

void Technique::resetLights () const
{
    GreAutolock ; iCurrentLight = -1 ;
}

void Technique::setAliasedTextureStruct (const TechniqueParam & alias1 ,
                                         const TechniqueParam & alias2 ,
                                         const TextureHolder & tex) const
{
    GreAutolock ;

    if ( iProgram.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Checks both aliases. If one is empty , just send to name2.

    std::string name1 = getAlias ( alias1 ) ;
    std::string name2 = getAlias ( alias2 ) ;

    if ( name1.empty() ) {
        setAliasedTexture(alias2, tex);
        return ;
    }

    //////////////////////////////////////////////////////////////////////
    // If program is bound , sends the value to the shader object.

    if ( !iProgram->binded() )
    return ;

    int unit = bindTexture ( tex ) ;
    HdwProgVarType textype = HdwProgVarTypeFromTextureType ( tex->getType() ) ;
    iProgram -> setUniform ( name1 + "." + name2 , textype , unit ) ;
}

void Technique::setAliasedTexture(const Gre::TechniqueParam &param, const TextureHolder &tex) const
{
    GreAutolock ;

    if ( iProgram.isInvalid() || tex.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // If this parameter is set in the framebuffer attachements aliases , we
    // prefer to set the framebuffer attachement instead.

    auto it = iAliasAttachements.find(param) ;

    if ( it != iAliasAttachements.end() && !iFramebuffer.isInvalid() )
    iFramebuffer -> setAttachment (it->second, tex) ;

    //////////////////////////////////////////////////////////////////////
    // If not , bind the alias to the program.

    else if ( iProgram->binded() )
    {
        auto alias = getAlias ( param ) ;
        if ( alias.empty() ) return ;

        int unit = bindTexture ( tex ) ;
        HdwProgVarType textype = HdwProgVarTypeFromTextureType ( tex->getType() ) ;
        iProgram -> setUniform(alias, textype, unit);
    }
}

void Technique::setAttribName(const Gre::VertexAttribAlias &alias, const std::string &name)
{
    GreAutolock ; iAttribAliases[alias] = name ;
}

const std::string Technique::getAttribName ( const VertexAttribAlias & alias ) const
{
    GreAutolock ;

    auto it = iAttribAliases.find(alias) ;
    if ( it != iAttribAliases.end() ) return it->second ;
    return std::string () ;
}

void Technique::resetTextures() const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // When resetting texture , we unbind every textures , activating their
    // texture unit. Notes the technique should be unbinded after using this
    // function , not before or the program will never be unbinded.

    if ( iProgram.isInvalid() )
    return ;

    iProgram -> use () ;

    while ( !iTextureUnits.empty() )
    {
        if ( !iTextureUnits.top().isInvalid() )
        {
            iProgram -> bindTextureUnit ( iTextureUnits.size() - 1 ) ;
            iTextureUnits.top() -> unbind () ;
            iTextureUnits.pop() ;
        }
    }
}

void Technique::setFramebufferAttachements ( const std::map < TechniqueParam , RenderFramebufferAttachement > & attachements )
{
    GreAutolock ; iAliasAttachements = attachements ;
}

bool Technique::isSelfRendered () const
{
    GreAutolock ; return iSelfRendered ;
}

void Technique::setSelfRendered ( bool value )
{
    GreAutolock ; iSelfRendered = value ;
}

void Technique::reset () const
{
    resetLights () ;
    resetTextures () ;
}

const Matrix4 Technique::getProjectionMatrix () const
{
    GreAutolock ;

    if ( iFramebuffer.isInvalid() )
    return Matrix4 ( 1.0f ) ;

    return iFramebuffer -> getProjection().get () ;
}

void Technique::addGlobSet ( const std::string & globname , const TechniqueParam & techparam )
{
    GreAutolock ; iGlobSets[globname] = techparam ;
}

const std::vector < std::string > Technique::findGlobSets ( const TechniqueParam & param ) const
{
    GreAutolock ; std::vector < std::string > ret ;

    for ( auto globset : iGlobSets )
    if ( globset.second == param )
    ret.push_back ( globset.first ) ;

    return ret ;
}

void Technique::addGlobAlias ( const std::string & globname , const std::string & progparam )
{
    GreAutolock ; iGlobALiases[globname] = progparam ;
}

int Technique::bindTexture ( const TextureHolder & texture ) const
{
    GreAutolock ;

    if ( iProgram.isInvalid() || texture.isInvalid() )
    return 0 ;

    //////////////////////////////////////////////////////////////////////
    // Binds the next texture unit , and bind the texture to it.

    int unit = iTextureUnits.size() ;

    iProgram -> bindTextureUnit ( unit ) ;
    texture -> bind () ;

    iTextureUnits.push ( texture ) ;
    return unit ;
}

void Technique::onUpdateEvent(const Gre::UpdateEvent &e)
{

}

// ---------------------------------------------------------------------------------------------------

TechniqueManager::TechniqueManager ( const std::string & name )
: ResourceManagerBase<Gre::Technique>(name)
{
    //////////////////////////////////////////////////////////////////////
    // Always load a Global named 'null'.
    addGlobal ( "null" , HdwProgVarType::None , (float) 0.0f ) ;
}

TechniqueManager::~TechniqueManager() noexcept ( false )
{

}

int TechniqueManager::loadFromBundles ( const std::vector < ResourceBundleHolder > & bundles )
{
    GreAutolock ;
    int ret = 0 ;

    for ( auto bundle : bundles ) {
        ret += loadFromBundle ( bundle ) ;
    }

    return ret ;
}

int TechniqueManager::loadFromBundle ( const ResourceBundleHolder & bundle )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // First , tries to get a list of files in this bundle.

    std::vector < std::string > files = bundle->getFilesList ( ResourceType::Effect ) ;

    if ( files.empty() )
        return 0 ;

    //////////////////////////////////////////////////////////////////////
    // Now , we have to try to load every files. Notes that the file list
    // is computed to be non-recursive , because some technique's files may
    // want to include private files that should not be loaded separately.

    int ret = 0 ;

    for ( auto file : files )
    {
        TechniqueHolderList tech = loadFromFile ( file ) ;
        ret += tech.size() ;
    }

    return ret ;
}

TechniqueHolderList TechniqueManager::loadFromFile ( const std::string & path )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Gets the file's source.

    std::string src = Platform::GetFileSource ( path ) ;

    if ( src.empty() )
    {
#ifdef GreIsDebugMode
        GreDebug ( "[WARN] Can't get file's source '" ) << path << "'." << gendl ;
#endif
        return TechniqueHolderList () ;
    }

    //////////////////////////////////////////////////////////////////////
    // Preprocess the file using the TechniqueFilePreprocessor object. The
    // preprocessor is used to compute macros , and to define the file's
    // version . If no version is set , the default is this engine version.

    TechniqueFilePreprocessor preprocessor ;
    int version = preprocessor.process ( Platform::GetFileDirectory(path) , src ) ;

    //////////////////////////////////////////////////////////////////////
    // Next , depending on the Technique's file version , we use the correct
    // TechniqueFileParser.

    if ( version == 1 )
    {
        TechniqueFileParser parser ;
        TechniqueHolderList techniques = parser.process ( path , preprocessor.getResult() ) ;

        if ( !techniques.empty() )
        {
            for ( auto tech : techniques )
            {
                //////////////////////////////////////////////////////////////////////
                // Register every techniques loaded.

                if ( !tech.isInvalid() ) {
                    loadFromHolder ( tech ) ;
                }
            }
        }

        return techniques ;
    }

    return TechniqueHolderList () ;
}

TechniqueHolder TechniqueManager::loadFromHolder ( const TechniqueHolder & technique )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check this technique is not already loaded.

    if ( iHolders.find(technique->getIdentifier()) == iHolders.end() && !technique.isInvalid() )
    {
        iHolders.add ( technique ) ;
        addFilteredListener ( technique , { EventType::Update } ) ;
    }

    return technique ;
}

TechniqueHolder TechniqueManager::loadBlank ( const std::string & name )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Creates a new technique and registers it.

    return loadFromHolder ( new Technique ( name ) ) ;
}

TechniqueHolder TechniqueManager::get ( const std::string & name )
{
    return findFirstHolder ( name ) ;
}

const TechniqueHolder TechniqueManager::get ( const std::string & name ) const
{
    return findFirstHolder ( name ) ;
}

void TechniqueManager::addGlobal ( const std::string & name , const HdwProgVarType & type , const RealProgramVariable & value )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check if not already existing.

    auto checkit = iGlobalsByName.find ( name ) ;
    if ( checkit != iGlobalsByName.end() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Register the variable.

    HardwareProgramVariable var ;
    var.name = name ;
    var.type = type ;
    var.value = value ;
    iGlobalsByName [name] = var ;

    //////////////////////////////////////////////////////////////////////
    // Informs !

    GreDebug ( "[INFO] Added Global : '" ) << name << "'." << gendl ;
}

void TechniqueManager::setGlobalValue ( const std::string & name , const RealProgramVariable & value )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check existing.

    auto var = iGlobalsByName.find ( name ) ;
    if ( var == iGlobalsByName.end() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Sets new value.

    var->second.value = value ;
}

const HardwareProgramVariable & TechniqueManager::getGlobal ( const std::string & name ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check existency.

    auto globit = iGlobalsByName.find(name) ;

    if ( globit == iGlobalsByName.end() )
    return iGlobalsByName.at("null") ;

    else
    return globit->second ;
}

GreEndNamespace
