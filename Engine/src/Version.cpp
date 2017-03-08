//////////////////////////////////////////////////////////////////////
//
//  Version.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 23/03/2016.
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

#include "Version.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


extern "C" std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

// ---------------------------------------------------------------------------------------------------

GreBeginNamespace

std::string DebugListNumeroted(const StringList& list)
{
    std::string ret;
    size_t itnum = 1;
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

// ---------------------------------------------------------------------------------------------------

GreConstructorException::GreConstructorException(const char* constructor)
: iConstructor(constructor)
{
    
}

GreConstructorException::GreConstructorException(const char* constructor, const char* error)
: iConstructor(constructor)
, iError(error)
{
    
}

const char* GreConstructorException::what() const throw()
{
    return (iConstructor + " : " + iError).c_str();
}

// ---------------------------------------------------------------------------------------------------

GreIndexException::GreIndexException(const char* emitter, size_t idx, size_t max)
: iEmitter(emitter)
, iIndex(idx)
, iMaxIndex(max)
, iWhat(iEmitter + ":IndexException:" + std::to_string(idx) + "/" + std::to_string(max))
{
    
}

GreIndexException::~GreIndexException()
{
    
}

const char* GreIndexException::what() const throw()
{
    return iWhat.c_str();
}

GreEndNamespace
