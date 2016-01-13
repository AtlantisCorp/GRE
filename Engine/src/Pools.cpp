//
//  Pools.cpp
//  GResource
//
//  Created by Jacques Tronconi on 05/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Pools.h"
GRE_BEGIN_NAMESPACE

Version GetLibVersion ()
{
    return { GRE_VERSION_MAJOR, GRE_VERSION_MINOR, GRE_VERSION_BUILD };
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

std::ostream& GreDebug(const std::string& func)
{
    std::cout << "[" << func << "] ";
    return std::cout;
}

Matrix4 MatrixUtils::Zero4 = Matrix4();
Matrix4 MatrixUtils::Identity = glm::mat4();

GRE_END_NAMESPACE
