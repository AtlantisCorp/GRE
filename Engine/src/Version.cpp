//
//  Version.cpp
//  GRE
//
//  Created by Jacques Tronconi on 23/03/2016.
//
//

#include "Version.h"

GreBeginNamespace

std::string DebugListNumeroted(const StringList& list)
{
    std::string ret;
    int itnum = 1;
    for(auto item : list)
    {
        ret += std::to_string(itnum);
        ret += ". ";
        ret += item;
        if(itnum != list.size())
            ret += "\n";
        
        itnum++;
    }
    
    return ret;
}

int DebugGetNumber()
{
    std::string num;
    int ret;
    
    std::cin >> num;
    ret = atoi(num.c_str());
    return ret;
}

Surface Surface::Null = { 0, 0, 0, 0 };

/// @brief Common egality operator.
bool operator == (const Surface& lhs, const Surface& rhs)
{
    return lhs.top == rhs.top && lhs.left == rhs.left &&
    lhs.width == rhs.width && lhs.height == rhs.height;
}

/// @brief Common different operator.
bool operator != (const Surface& lhs, const Surface& rhs)
{
    return ! (lhs == rhs);
}

GreEndNamespace
