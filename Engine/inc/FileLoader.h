//
//  FileLoader.h
//  GResource
//
//  Created by Jacques Tronconi on 02/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#ifndef GResource_FileLoader_h
#define GResource_FileLoader_h

#include "Resource.h"

class DLL_PUBLIC FileLoader : public ResourceLoader
{
public:
    
    FileLoader();
    FileLoader(const FileLoader&);
    
    virtual ~FileLoader();
    
    virtual Resource* load (Resource::Type type, const std::string& name, const std::string& file) const;
    virtual ResourceLoader* clone() const;
};

typedef ResourceLoaderFactory<FileLoader> FileLoaderFactory;

#endif
