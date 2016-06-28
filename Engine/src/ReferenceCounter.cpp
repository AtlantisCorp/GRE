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
    iHolderCount = iHolderCount + 1;
    iUserCount = iUserCount + 1;
    return iHolderCount;
}

int ReferenceCounter::unhold()
{
    iHolderCount = iHolderCount - 1;
    iUserCount = iUserCount - 1;
    return iHolderCount;
}

int ReferenceCounter::use()
{
    iUserCount = iUserCount + 1;
    return iUserCount;
}

int ReferenceCounter::unuse()
{
    iUserCount = iUserCount - 1;
    return iUserCount;
}

int ReferenceCounter::getHolderCount() const
{
    return iHolderCount;
}

int ReferenceCounter::getUserCount() const
{
    return iUserCount;
}

GreEndNamespace
