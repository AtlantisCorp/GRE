//
//  FileLoader.cpp
//  GResource
//
//  Created by Jacques Tronconi on 05/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "FileLoader.h"

GRE_BEGIN_NAMESPACE

FileLoader::FileLoader()
{
    
}

FileLoader::FileLoader(const FileLoader&)
{
    
}

FileLoader::~FileLoader()
{
    
}

Resource* FileLoader::load (Resource::Type type, const std::string& name, const std::string& file) const
{
    return nullptr;
}

ResourceLoader* FileLoader::clone() const
{
    return nullptr;
}

GRE_END_NAMESPACE
