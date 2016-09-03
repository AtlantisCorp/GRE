//////////////////////////////////////////////////////////////////////
//
//  Variant.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 25/01/2016.
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

#ifndef GRE_Variant_h
#define GRE_Variant_h

#include "Pools.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief A basic Variant object, which can contains one subobject
/// supported by this class.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Variant
{
public:
    
    enum class Policy
    {
        None    = 0,    ///< @brief Nothing is stored.
        Integer = 1,    ///< @brief Stores an Integer.
        Version = 2,    ///< @brief Stores a Version.
        Boolean = 3     ///< @brief Type 'bool'.
    };
    
    Variant(Policy policy = Policy::None, void* object = nullptr);
    Variant(const Variant& rhs);
    
    Variant(int object);
    Variant(const Version& object);
    Variant(bool object);
    
    ~Variant();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Object.
    //////////////////////////////////////////////////////////////////////
    void reset(Policy policy, const void* object);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Object.
    //////////////////////////////////////////////////////////////////////
    void reset(int object);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Object.
    //////////////////////////////////////////////////////////////////////
    void reset(const Version& object);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Sets a new Object.
    //////////////////////////////////////////////////////////////////////
    void reset(bool object);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Clears the Variant and set it to Policy::None.
    //////////////////////////////////////////////////////////////////////
    void clear();
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Variant object as an Integer, if one.
    //////////////////////////////////////////////////////////////////////
    int& toInteger();
    const int& toInteger() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Variant object as a Version, if one.
    //////////////////////////////////////////////////////////////////////
    Version& toVersion();
    const Version& toVersion() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the Variant object as a bool, if one.
    //////////////////////////////////////////////////////////////////////
    bool& toBool();
    const bool& toBool() const;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if policy is Policy::None or Object is nullptr.
    //////////////////////////////////////////////////////////////////////
    bool isNull() const;
    
    /// @brief A Null Variant object.
    static Variant Null;
    
private:
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Copy an object depending on its policy and returns its
    /// pointer.
    //////////////////////////////////////////////////////////////////////
    static void* CopyFromPolicy(Policy policy, const void* object);
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Deletes an object depending on its policy.
    //////////////////////////////////////////////////////////////////////
    static void DeleteFromPolicy(Policy policy, void* object);
    
private:
    
    /// @brief The Policy to be applied to this Variant.
    Policy _mPolicy;
    
    /// @brief The internal object holded by this Variant.
    void*  _mObject;
};

typedef GreExceptionWithText VariantBadCast;

GreEndNamespace

#endif
