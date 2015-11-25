//
//  Clonable.h
//  GRE
//
//  Created by Jacques Tronconi on 23/11/2015.
//
//

#ifndef GRE_Clonable_h
#define GRE_Clonable_h

#include "Version.h"

GRE_BEGIN_NAMESPACE

/// @brief A class that defines a basic Clonable object.
/// Clonable objects should overwrite the clone() method in order
/// to create a new object.
template<class T>
class Clonable
{
public:
    
    virtual ~Clonable() { }
    virtual T* clone () const = 0;
};

GRE_END_NAMESPACE
#endif
