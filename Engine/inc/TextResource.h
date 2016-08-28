//
//  TextResource.h
//  GResource
//
//  Created by Jacques Tronconi on 28/10/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//
/*
#ifndef GResource_TextResource_h
#define GResource_TextResource_h

#include "Resource.h"
#include "FileLoader.h"

GreBeginNamespace

class DLL_PUBLIC TextResource : public Resource
{
public:
    
    POOLED(Pools::Resource)
    
    TextResource (const std::string& name, const std::string& file, const std::string& data);
    
protected:
    
    const void* _getData() const;
    
private:
    
    std::string _file;
    std::string _data;
};

class DLL_PUBLIC TextLoader : public FileLoader
{
public:
    
    POOLED(Pools::Loader)
    
    TextLoader ();
    
    bool isTypeSupported (Resource::Type type) const;
    
    Resource* load (Resource::Type type, const std::string& name, const std::string& file) const;
    
    ResourceLoader* clone() const;
};


GreEndNamespace
#endif
*/