//
//  TextResource.cpp
//  GResource
//
//  Created by Jacques Tronconi on 02/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "TextResource.h"

GRE_BEGIN_NAMESPACE

TextResource::TextResource (const std::string& name, const std::string& file, const std::string& data) :
Resource(name),
_file (file),
_data(data)
{
    
}

const void* TextResource::_getData() const
{
    return (const void*) &_data;
}

TextLoader::TextLoader ()
{
    
}

bool TextLoader::isTypeSupported (Resource::Type type) const
{
    return type == Resource::Type::Text;
}

Resource* TextLoader::load (Resource::Type type, const std::string& name, const std::string& file) const
{
    std::ifstream ifs (file.c_str());
    
    ifs.seekg(0, ifs.end);
    std::streampos lenght = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    
    char buf[(int)(lenght+(std::streampos)1)];
    ifs.get(buf, lenght+(std::streampos)1);
    ifs.close();
    
    TextResource* ret = new TextResource(name, file, std::string(buf));
    
#ifdef DEBUG
    std::cout << "[TextLoader:" << file << "] Resource Loaded." << std::endl;
#endif
    
    return ret;
}

ResourceLoader* TextLoader::clone() const
{
    return new TextLoader();
}

GRE_END_NAMESPACE
