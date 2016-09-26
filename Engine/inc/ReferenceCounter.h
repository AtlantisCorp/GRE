//////////////////////////////////////////////////////////////////////
//
//  ReferenceCounter.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 06/05/2016.
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

#ifndef GRE_ReferenceCounter_h
#define GRE_ReferenceCounter_h

#include "Pools.h"

GreBeginNamespace

////////////////////////////////////////////////////////////////////////
/// @class ReferenceCounter
/// @brief A Counter that is used by Resource, ResourceHolder and
/// ResourceUser.
///
/// It has been designed to be shared between those 3 classes. The behaviour
/// should be as followed :
/// - When iHolderCount reach 0, the Resource Object should be destroyed or
/// commit suicide.
/// - When iUserCount reach 0, the ReferenceCounter Object should be destroyed
/// by the Object that made him reach 0.
/// - A Resource count as iUserCount, and not iHolderCount. When the Resource
/// initialize the Counter, it should use the ReferenceCounter::use() function
/// to store itself. Then, when destroying the Resource (the iHolderCount reach
/// 0), it should call ReferenceCounter::unuse() to unstore itself.
////////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ReferenceCounter
{
public:
    
    POOLED(Pools::Resource)
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Creates a new Reference Counter object and intialize all fields
    /// to 0.
    ////////////////////////////////////////////////////////////////////////
    ReferenceCounter();
    
    ~ReferenceCounter() noexcept(false);
    
public:
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Increase by one iHolderCount and iUserCount. This is used
    /// by ResourceHolder.
    /// @return Current iHolderCount.
    ////////////////////////////////////////////////////////////////////////
    int hold();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Decrease by one iHolderCount and iUserCount. This is used by
    /// ResourceHolder.
    /// @return Current iHolderCount.
    ////////////////////////////////////////////////////////////////////////
    int unhold();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Increase by one iUserCount. This is used by ResourceUser.
    /// @return Current iUserCount.
    ////////////////////////////////////////////////////////////////////////
    int use();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Decrease by one iUserCount. This is used by ResourceUser.
    /// @return Current iUserCount.
    ////////////////////////////////////////////////////////////////////////
    int unuse();
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the iHolderCount.
    ////////////////////////////////////////////////////////////////////////
    int getHolderCount() const;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns the iUserCount.
    ////////////////////////////////////////////////////////////////////////
    int getUserCount() const;
    
private:
    
    /// @brief Stores Holder's count. When it reach 0, the Object must be deleted.
    int iHolderCount;
    
    /// @brief Stores User's + Holder's count. When it reach 0, this Counter must be deleted.
    int iUserCount;
};

GreEndNamespace

#endif
