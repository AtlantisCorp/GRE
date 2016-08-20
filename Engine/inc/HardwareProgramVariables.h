//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramVariables.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 07/01/2016.
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

#ifndef GRE_HardwareProgramVariables_h
#define GRE_HardwareProgramVariables_h

#include "Pools.h"
#include "HardwareProgramVariable.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief List of HardwareProgramVariable.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC HardwareProgramVariables : public std::list<HardwareProgramVariable>
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    HardwareProgramVariables();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~HardwareProgramVariables();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a HardwareProgramVariable to the list.
    //////////////////////////////////////////////////////////////////////
    virtual void add(const HardwareProgramVariable& var);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a HardwareProgramVariable from the list.
    //////////////////////////////////////////////////////////////////////
    virtual HardwareProgramVariable& get(const std::string& name);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns a HardwareProgramVariable from the list.
    //////////////////////////////////////////////////////////////////////
    virtual const HardwareProgramVariable& get(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the given Variable name already exists.
    //////////////////////////////////////////////////////////////////////
    virtual bool has(const std::string& name) const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a HardwareProgramVariable from the list.
    //////////////////////////////////////////////////////////////////////
    virtual void remove(const std::string& name);
    
    /// @brief An Empty List.
    static HardwareProgramVariables Empty;
};

GreEndNamespace

#endif
