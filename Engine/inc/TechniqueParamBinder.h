//////////////////////////////////////////////////////////////////////
//
//  TechniqueParamBinder.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 24/03/2017.
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

#ifndef GRE_TECHNIQUEPARAMBINDER_H
#define GRE_TECHNIQUEPARAMBINDER_H

#include "HardwareProgram.h"
#include "Technique.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Interface to bind named , aliased or custom parameters to
/// any technique.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TechniqueParamBinder : virtual public Lockable
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TechniqueParamBinder () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TechniqueParamBinder () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void addNamedParameter (const std::string & name , const HdwProgVarType & type , const RealProgramVariable & value);

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramVariable & getNamedParameter ( const std::string & name ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramVariable & getNamedParameter ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual void addAliasedParameter (const TechniqueParam & alias , const HdwProgVarType & type , const RealProgramVariable & value);

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramVariable & getAliasedParameter ( const TechniqueParam & alias ) const ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramVariable & getAliasedParameter ( const TechniqueParam & alias ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Lets the binder binds parameters to the technique in order
    /// to use it. When binding , the default function binds every named
    /// and aliased parameters.
    ///
    /// @note An object that use other binder should call their use function
    /// inside this one. Also , overwriting this function should always call
    /// the 'TechniqueParamBinder::use()' default function.
    //////////////////////////////////////////////////////////////////////
    virtual void use ( const TechniqueHolder & technique ) const ;

protected:

    /// @brief List of parameters for the technique , by names. Notes that only the
    /// 'name' , 'value' and 'type' fields are used.
    std::map < std::string , HardwareProgramVariable > iNamedParams ;

    /// @brief List of parameters for the technique , by alias. Notes that only the
    /// 'value' and 'type' fields are used.
    std::map < TechniqueParam , HardwareProgramVariable > iAliasedParams ;
};

GreEndNamespace

#endif
