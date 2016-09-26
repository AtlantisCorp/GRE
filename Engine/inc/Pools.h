//////////////////////////////////////////////////////////////////////
//
//  Pools.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 28/10/2015.
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

#ifndef GResource_Pools_h
#define GResource_Pools_h

#include "Version.h"
#include "Clonable.h"

GreBeginNamespace

/// @brief Enumerate every Pools available in the Engine.
enum class Pools
{
    Default   = 0x1,    ///< @brief The default one.
    Resource  = 0x2,    ///< @brief Reserved to Resource objects.
    Manager   = 0x3,    ///< @brief Reserved to Manager objects.
    Loader    = 0x4,    ///< @brief Reserved to Loader objects.
    Factory   = 0x5,    ///< @brief Reserved to Factory objects.
    Event     = 0x6,    ///< @brief Reserved to Event-related objects.
    HdwBuffer = 0x7,    ///< @brief Reserved to HardwareBuffer objects.
    Scene     = 0x8,    ///< @brief Reserved to Scene's related objects.
    Render    = 0x9     ///< @brief Reserved to Render's related objects.
};

#ifndef GreExtraResourceHolder

/// @brief Declares an object as part of the pool system.
/// A Pooled object is created and destroyed using the Pool
/// system. You can select the right pool using this macro.
/// @note You must set every subclasses as Pooled too, in order
/// to correctly overwrite the new/delete operators.
#define POOLED(pooltype) \
void* operator new (size_t sz) { \
if(Pool< pooltype > ::Get().canAttach(sz) == false) throw std::bad_alloc(); \
void* ptr = malloc (sz) ; \
Pool< pooltype > :: Get().attach(ptr, sz); \
return ptr; } \
void  operator delete (void* p) noexcept { \
if(p) free (p); \
Pool< pooltype > :: Get().detach(p); }

#else

#define POOLED(pooltype) \
    void* operator new ( size_t sz ) { \
        if ( Pool< pooltype > ::Get().canAttach(sz) == false ) throw std::bad_alloc(); \
        void* ptr = malloc ( sz + sizeof(int32_t) ); \
        *( (int*) ptr) = 0xBAD007; \
        return (void*) ( ( (char*) ptr ) + sizeof(int32_t) ); \
    }

#endif

/// @class Pool
/// @brief A Basic memory pool.
template < Pools pooltype >
class DLL_PUBLIC Pool
{
public:
    
    /// @brief Return the Pool corresponding to the template argument.
    static Pool& Get () {
        static Pool pool;
        return pool;
    }
    
    /// @brief Add memory to this pool.
    /// @note If an address already is in the database, it is
    /// overwritten and is considered as a reallocation.
    /// @note This function should be used only by Pooled objects.
    /// When memory is attached, the pooled object should always
    /// check if the total memory of the pool has been reached.
    /// If maximum memory is reached, behaviour is application-dependent.
    void attach(void* p, size_t sz)
    {
        if(_ptrs.find(p) != _ptrs.end()) {
            _totalsz -= _ptrs[p];
        }
        
        _ptrs[p] = sz;
        _totalsz += sz;
    }
    
    /// @brief Remove memory from this pool.
    void detach(void* p)
    {
        auto it = _ptrs.find(p);
        if(it != _ptrs.end()) {
            _totalsz -= it->second;
            _ptrs.erase(it);
        }
    }
    
    /// @brief Returns true if allocating an object to this pool
    /// is possible, giving its size.
    bool canAttach(size_t sz)
    {
        return _totalsz + sz <= _maxsz;
    }
    
    /// @brief Returns the size actually taken by objects in this pool.
    unsigned getCurrentSize () const
    {
        return _totalsz;
    }
    
    /// @brief Set the maximum size of this pool.
    void setMaximumSize(size_t szinbytes)
    {
        _maxsz = szinbytes;
    }
    
    /// @brief Returns the maximum size this pool can reach.
    unsigned getMaximumSize() const
    {
        return _maxsz;
    }
    
private:
    
    Pool () : _totalsz(0), _maxsz(100000) { }
    
    std::map<void*, size_t> _ptrs;
    unsigned                _totalsz;
    unsigned                _maxsz;
};

GreEndNamespace
#endif
