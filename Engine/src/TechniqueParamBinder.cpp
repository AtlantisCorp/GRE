//////////////////////////////////////////////////////////////////////
//
//  TechniqueParamBinder.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 29/03/2017.
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

#include "TechniqueParamBinder.h"

GreBeginNamespace

TechniqueParamBinder::TechniqueParamBinder ()
{

}

TechniqueParamBinder::~TechniqueParamBinder()
{

}

void TechniqueParamBinder::addNamedParameter(const std::string &name, const Gre::HdwProgVarType &type, const Gre::RealProgramVariable &value)
{
    GreAutolock ;

    iNamedParams[name].type = type ;
    iNamedParams[name].value = value ;
}

const HardwareProgramVariable & TechniqueParamBinder::getNamedParameter(const std::string &name) const
{
    GreAutolock ;

    auto it = iNamedParams.find(name) ;
    if ( it != iNamedParams.end() ) return it->second ;
    return HardwareProgramVariable::Null ;
}

HardwareProgramVariable & TechniqueParamBinder::getNamedParameter(const std::string &name)
{
    GreAutolock ; return iNamedParams [name] ;
}

void TechniqueParamBinder::addAliasedParameter(const Gre::TechniqueParam &alias, const Gre::HdwProgVarType &type, const Gre::RealProgramVariable &value)
{
    GreAutolock ;

    iAliasedParams[alias].type = type ;
    iAliasedParams[alias].value = value ;
}

const HardwareProgramVariable & TechniqueParamBinder::getAliasedParameter(const Gre::TechniqueParam &alias) const
{
    GreAutolock ;

    auto it = iAliasedParams.find(alias) ;
    if ( it != iAliasedParams.end() ) return it->second ;
    return HardwareProgramVariable::Null ;
}

HardwareProgramVariable & TechniqueParamBinder::getAliasedParameter(const Gre::TechniqueParam &alias)
{
    GreAutolock ; return iAliasedParams[alias] ;
}

void TechniqueParamBinder::use(const TechniqueHolder &technique) const
{
    GreAutolock ;

    if ( technique.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Binds the aliased variables primary.

    for ( auto it : iAliasedParams )
    technique -> setAliasedParameterValue(it.first, it.second.type, it.second.value) ;

    //////////////////////////////////////////////////////////////////////
    // Binds the named variables now. This is done after the aliases because
    // a user could make an alias and then want to overwrite its value using
    // a direct name for the shader.

    for ( auto it : iNamedParams )
    technique -> setNamedParameterValue(it.first, it.second.type, it.second.value) ;
}

void TechniqueParamBinder::clear ()
{
    GreAutolock ;

    iAliasedParams.clear () ;
    iNamedParams.clear () ;
}

GreEndNamespace
