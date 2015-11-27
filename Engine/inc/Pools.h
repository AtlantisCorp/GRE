//
//  Pools.h
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef GResource_Pools_h
#define GResource_Pools_h

#include "Version.h"
#include "Clonable.h"

GRE_BEGIN_NAMESPACE

/// @brief Enumerate every Pools available in the Engine.
enum class DLL_PUBLIC Pools
{
    Default   = 0x1,    ///< @brief The default one.
    Resource  = 0x2,    ///< @brief Reserved to Resource objects.
    Manager   = 0x3,    ///< @brief Reserved to Manager objects.
    Loader    = 0x4,    ///< @brief Reserved to Loader objects.
    Factory   = 0x5,    ///< @brief Reserved to Factory objects.
    Event     = 0x6,    ///< @brief Reserved to Event-related objects.
    HwdBuffer = 0x7     ///< @brief Reserved to HardwareBuffer objects.
};

/// @brief Declares an object as part of the pool system.
/// A Pooled object is created and destroyed using the Pool
/// system. You can select the right pool using this macro.
/// @note You should set every subclasses as Pooled too, in order
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

GRE_END_NAMESPACE
#endif
