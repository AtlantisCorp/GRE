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

GRE_BEGIN_NAMESPACE

/// @brief A Generic Loader for files.
/// It determines the function to load files.
class DLL_PUBLIC FileLoader : public ResourceLoader
{
public:
    
    FileLoader();
    FileLoader(const FileLoader&);
    
    virtual ~FileLoader();
    
    /// @brief Load a file.
    virtual Resource* load (Resource::Type type, const std::string& name, const std::string& file) const;
    
    /// @brief Clone the ResourceLoader object.
    virtual ResourceLoader* clone() const;
};

typedef ResourceLoaderFactory<FileLoader> FileLoaderFactory;

GRE_END_NAMESPACE

#endif
