//
//  LoopBehaviours.cpp
//  GRE
//
//  Created by Jacques Tronconi on 21/02/2016.
//
//

#include "LoopBehaviours.h"

GreBeginNamespace

LoopBehaviours::LoopBehaviours()
{
    
}

LoopBehaviours::~LoopBehaviours()
{
    
}

void LoopBehaviours::add(LoopBehaviour behaviour)
{
    _mBehaviours.push_back(behaviour);
}

void LoopBehaviours::clear()
{
    _mBehaviours.clear();
}

void LoopBehaviours::call()
{
    for(LoopBehaviour fn : _mBehaviours)
        fn ();
}

const std::vector<LoopBehaviour>& LoopBehaviours::get() const
{
    return _mBehaviours;
}

GreEndNamespace
