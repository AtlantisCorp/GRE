////////////////////////////////////////////////////
//  File    : ReferenceCounter.cpp
//  Project : GRE
//
//  Created by Jacques Tronconi on 06/05/2016.
//  
//
////////////////////////////////////////////////////

#include "ReferenceCounter.h"

GreBeginNamespace

ReferenceCounter::ReferenceCounter()
{
    iHolderCount = 0;
    iUserCount = 0;
}

ReferenceCounter::~ReferenceCounter() noexcept(false)
{
    
}

int ReferenceCounter::hold()
{
    GreAutolock ;
    
    iHolderCount = iHolderCount + 1;
    iUserCount = iUserCount + 1;
    return iHolderCount;
}

int ReferenceCounter::unhold()
{
    GreAutolock ;
    
    iHolderCount = iHolderCount - 1;
    iUserCount = iUserCount - 1;
    return iHolderCount;
}

int ReferenceCounter::use()
{
    GreAutolock ;
    iUserCount = iUserCount + 1;
    return iUserCount;
}

int ReferenceCounter::unuse()
{
    GreAutolock ;
    iUserCount = iUserCount - 1;
    return iUserCount;
}

int ReferenceCounter::getHolderCount() const
{
    GreAutolock ; return iHolderCount;
}

int ReferenceCounter::getUserCount() const
{
    GreAutolock ; return iUserCount;
}

GreEndNamespace
