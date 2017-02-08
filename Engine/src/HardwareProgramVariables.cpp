//////////////////////////////////////////////////////////////////////
//
//  HardwareProgramVariables.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 11/01/2016.
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

#include "HardwareProgramVariables.h"

GreBeginNamespace

HardwareProgramVariables::HardwareProgramVariables()
{
    
}

HardwareProgramVariables::~HardwareProgramVariables()
{
    
}

void HardwareProgramVariables::add(const Gre::HardwareProgramVariable &var)
{
    push_back(var);
}

HardwareProgramVariable& HardwareProgramVariables::get(const std::string &name)
{
    for(auto it = begin(); it != end(); it++)
    {
        if((*it).name == name)
            return *it;
    }
    
    return HardwareProgramVariable::Null;
}

const HardwareProgramVariable& HardwareProgramVariables::get(const std::string &name) const
{
    for(auto it = begin(); it != end(); it++)
    {
        if((*it).name == name)
            return *it;
    }
    
    return HardwareProgramVariable::Null;
}

bool HardwareProgramVariables::has(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( auto it = begin(); it != end(); it++ )
        {
            if ( (*it).name == name )
            {
                return true;
            }
        }
        
        return false;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Invalid 'name' given." << Gre::gendl;
#endif
        return false;
    }
}

void HardwareProgramVariables::remove(const std::string &name)
{
    for(auto it = begin(); it != end(); it++)
    {
        if((*it).name == name)
            erase(it);
    }
}

HardwareProgramVariables HardwareProgramVariables::Empty = HardwareProgramVariables();

GreEndNamespace
