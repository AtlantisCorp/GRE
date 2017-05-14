//
//  Pools.cpp
//  GResource
//
//  Created by Jacques Tronconi on 05/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Pools.h"
GreBeginNamespace

Version GetLibVersion ()
{
    return { GreVersionMajor, GreVersionMinor, GreVersionBuild };
}

std::string computeMethodName(const std::string& function, const std::string& prettyFunction) {
    size_t locFunName = prettyFunction.find(function); //If the input is a constructor, it gets the beginning of the class name, not of the method. That's why later on we have to search for the first parenthesys
    size_t begin = prettyFunction.rfind(" ",locFunName) + 1;
    size_t end = prettyFunction.find("(",locFunName + function.length()); //Adding function.length() make this faster and also allows to handle operator parenthesys!
    if (prettyFunction[end + 1] == ')')
        return (prettyFunction.substr(begin,end - begin) + "()");
    else
        return (prettyFunction.substr(begin,end - begin) + "(...)");
}

std::recursive_mutex __globcoutmutex ;

std::ostream& GreDebugBase(const std::string& func)
{
    __globcoutmutex.lock();
    std::cout << "[" << func << "] ";
    return std::cout;
}

GreEndNamespace
