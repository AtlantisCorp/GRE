//////////////////////////////////////////////////////////////////////
//
//  LoopBehaviours.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 21/02/2016.
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

#ifndef GRE_LoopBehaviours_h
#define GRE_LoopBehaviours_h

#include "Pools.h"

GreBeginNamespace

/// @brief A Behaviour function.
typedef std::function<void (void)> LoopBehaviour;

//////////////////////////////////////////////////////////////////////
/// @brief A Helper class to store Behaviour's functions.
/// Use this class to make any other class able to have custom behaviours
/// on some event, or action.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC LoopBehaviours
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    LoopBehaviours();
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ~LoopBehaviours();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a behaviour.
    //////////////////////////////////////////////////////////////////////
    void add(LoopBehaviour behaviour);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears every behaviours.
    //////////////////////////////////////////////////////////////////////
    void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Calls every behaviours.
    //////////////////////////////////////////////////////////////////////
    void call();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every behaviours stored.
    //////////////////////////////////////////////////////////////////////
    const std::vector<LoopBehaviour>& get() const;
    
private:
    
    /// @brief Holds every Behaviours.
    std::vector<LoopBehaviour> _mBehaviours;
};

GreEndNamespace

#endif
