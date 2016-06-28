////////////////////////////////////////////////////
//  File    : ReferenceCounter.h
//  Project : GRE
//
//  Created by Jacques Tronconi on 06/05/2016.
//  
//
////////////////////////////////////////////////////

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
    std::atomic<int> iHolderCount;
    
    /// @brief Stores User's + Holder's count. When it reach 0, this Counter must be deleted.
    std::atomic<int> iUserCount;
};

GreEndNamespace

#endif
