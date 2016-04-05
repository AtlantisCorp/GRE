//
//  RenderContextInfo.cpp
//  GRE
//
//  Created by Jacques Tronconi on 12/02/2016.
//
//

#include "RenderContextInfo.h"

GreBeginNamespace

RenderContextInfo::RenderContextInfo()
{
    
}

RenderContextInfo::~RenderContextInfo()
{
    
}

void RenderContextInfo::add(const std::string &entryName, const Variant &entryValue)
{
    _mDictionnary[entryName] = entryValue;
}

std::map<std::string, Variant>& RenderContextInfo::getEntries()
{
    return _mDictionnary;
}

const std::map<std::string, Variant>& RenderContextInfo::getEntries() const
{
    return _mDictionnary;
}

bool RenderContextInfo::hasEntry(const std::string &entryName) const
{
    return _mDictionnary.find(entryName) != _mDictionnary.end();
}

Variant& RenderContextInfo::getValue(const std::string& entryName)
{
    return _mDictionnary.at(entryName);
}

const Variant& RenderContextInfo::getValue(const std::string& entryName) const
{
    return _mDictionnary.at(entryName);
}

RenderContextInfo RenderContextInfo::Null = RenderContextInfo();

GreEndNamespace
