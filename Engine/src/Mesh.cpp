//////////////////////////////////////////////////////////////////////
//
//  Mesh.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 26/11/2015.
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

#include "Mesh.h"

GreBeginNamespace

MeshPrivate::MeshPrivate(const std::string& name)
: Gre::Resource(name), iSoftVertexBuffer(nullptr), iSoftIndexBuffer(nullptr), iHardVertexBuffer(nullptr), iHardIndexBuffer(nullptr), iSoftBuffersChanged(false), iHardBuffersChanged(false), iUseHardwareBuffers(false)
{
    
}

MeshPrivate::~MeshPrivate() noexcept(false)
{
    
}

const SoftwareVertexBuffer MeshPrivate::getSoftwareVertexBuffer() const
{
    return SoftwareVertexBuffer(iSoftVertexBuffer);
}

void MeshPrivate::setSoftwareVertexBuffer(const Gre::SoftwareVertexBuffer &softvertexbuffer)
{
    auto newbuf = softvertexbuffer.lock();
    if ( newbuf != iSoftVertexBuffer )
    {
        iSoftVertexBuffer = newbuf;
        iSoftBuffersChanged = true;
    }
}

const SoftwareIndexBuffer MeshPrivate::getSoftwareIndexBuffer() const
{
    return SoftwareIndexBuffer(iSoftIndexBuffer);
}

void MeshPrivate::setSoftwareIndexBuffer(const Gre::SoftwareIndexBuffer &softindexbuffer)
{
    auto newbuf = softindexbuffer.lock();
    if ( newbuf != iSoftIndexBuffer )
    {
        iSoftIndexBuffer = newbuf;
        iSoftBuffersChanged = true;
    }
}

const HardwareVertexBuffer MeshPrivate::getVertexBuffer() const
{
    return HardwareVertexBuffer(iHardVertexBuffer);
}

void MeshPrivate::setVertexBuffer(const HardwareVertexBuffer& vertexbuffer)
{
    auto newbuf = vertexbuffer.lock();
    if ( newbuf != iHardVertexBuffer )
    {
        iHardVertexBuffer = newbuf;
        iHardBuffersChanged = true;
    }
}

const HardwareIndexBuffer MeshPrivate::getIndexBuffer() const
{
    return HardwareIndexBuffer(iHardIndexBuffer);
}

void MeshPrivate::setIndexBuffer(const HardwareIndexBuffer& indexbuffer)
{
    auto newbuf = indexbuffer.lock();
    if ( newbuf != iHardIndexBuffer )
    {
        iHardIndexBuffer = newbuf;
        iHardBuffersChanged = true;
    }
}

bool MeshPrivate::useHardwareBuffers() const
{
    return iUseHardwareBuffers;
}

void MeshPrivate::setUseHardwareBuffers(bool b)
{
    iUseHardwareBuffers = b;
}

bool MeshPrivate::hasSoftwareBuffersChanged() const
{
    return iSoftBuffersChanged;
}

void MeshPrivate::setSoftwareBuffersChanged(bool b)
{
    iSoftBuffersChanged = b;
}

bool MeshPrivate::hasHardwareBuffersChanged() const
{
    return iHardBuffersChanged;
}

void MeshPrivate::setHardwareBuffersChanged(bool b)
{
    iHardBuffersChanged = b;
}

void MeshPrivate::clear()
{
    clearSoftwareBuffers();
    clearHardwareBuffers();
    
    iSoftBuffersChanged = false;
    iHardBuffersChanged = false;
    iUseHardwareBuffers = false;
}

void MeshPrivate::clearSoftwareBuffers()
{
    iSoftVertexBuffer.reset();
    iSoftIndexBuffer.reset();
    iSoftBuffersChanged = true;
}

void MeshPrivate::clearHardwareBuffers()
{
    iHardVertexBuffer.reset();
    iHardIndexBuffer.reset();
    iHardBuffersChanged = true;
}

// ---------------------------------------------------------------------------------------------------

Mesh::Mesh(const MeshPrivate* pointer)
: ResourceUser(pointer)
, SpecializedResourceUser(pointer)
{
    
}

Mesh::Mesh(const MeshHolder& holder)
: ResourceUser(holder)
, SpecializedResourceUser(holder)
{
    
}

Mesh::Mesh(const Mesh& user)
: ResourceUser(user)
, SpecializedResourceUser(user)
{
    
}

Mesh::~Mesh() noexcept(false)
{
    
}

const SoftwareVertexBuffer Mesh::getSoftwareVertexBuffer() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getSoftwareVertexBuffer();
    return SoftwareVertexBuffer::Null;
}

void Mesh::setSoftwareVertexBuffer(const SoftwareVertexBuffer& softvertexbuffer)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setSoftwareVertexBuffer(softvertexbuffer);
}

const SoftwareIndexBuffer Mesh::getSoftwareIndexBuffer() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getSoftwareIndexBuffer();
    return SoftwareIndexBuffer::Null;
}

void Mesh::setSoftwareIndexBuffer(const SoftwareIndexBuffer& softindexbuffer)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setSoftwareIndexBuffer(softindexbuffer);
}

const HardwareVertexBuffer Mesh::getVertexBuffer() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getVertexBuffer();
    return HardwareVertexBuffer::Null;
}

void Mesh::setVertexBuffer(const HardwareVertexBuffer& vertexbuffer)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setVertexBuffer(vertexbuffer);
}

const HardwareIndexBuffer Mesh::getIndexBuffer() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getIndexBuffer();
    return HardwareIndexBuffer::Null;
}

void Mesh::setIndexBuffer(const HardwareIndexBuffer& indexbuffer)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setIndexBuffer(indexbuffer);
}

bool Mesh::useHardwareBuffers() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->useHardwareBuffers();
    return false;
}

void Mesh::setUseHardwareBuffers(bool b)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setUseHardwareBuffers(b);
}

bool Mesh::hasSoftwareBuffersChanged() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->hasSoftwareBuffersChanged();
    return false;
}

void Mesh::setSoftwareBuffersChanged(bool b)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setSoftwareBuffersChanged(b);
}

bool Mesh::hasHardwareBuffersChanged() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->hasHardwareBuffersChanged();
    return false;
}

void Mesh::setHardwareBuffersChanged(bool b)
{
    auto ptr = lock();
    if ( ptr )
        ptr->setHardwareBuffersChanged(b);
}

void Mesh::clear()
{
    auto ptr = lock();
    if ( ptr )
        ptr->clear();
}

void Mesh::clearSoftwareBuffers()
{
    auto ptr = lock();
    if ( ptr )
        ptr->clearSoftwareBuffers();
}

void Mesh::clearHardwareBuffers()
{
    auto ptr = lock();
    if ( ptr )
        ptr->clearHardwareBuffers();
}

Mesh Mesh::Null = Mesh(nullptr);

// ---------------------------------------------------------------------------------------------------

MeshLoader::MeshLoader()
{
    
}

MeshLoader::~MeshLoader()
{
    
}

// ---------------------------------------------------------------------------------------------------

MeshManager::MeshManager()
{
    
}

MeshManager::~MeshManager()
{
    
}

Mesh MeshManager::load(const MeshHolder &holder)
{
    if ( !holder.isInvalid() )
    {
        if ( isLoaded(holder->getName()) )
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Resource Mesh '" << holder->getName() << "' has an already registered name." << std::endl;
#endif
            return Mesh ( nullptr );
        }
        
        iMeshes.add(holder);
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Resource Mesh '" << holder->getName() << "' registered." << std::endl;
#endif
        
        return Mesh ( holder );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'holder' parameter is invalid." << std::endl;
#endif
        return Mesh ( nullptr );
    }
}

Mesh MeshManager::load(const std::string &name, const std::string &filepath)
{
    if ( !name.empty() )
    {
        // Check if the name is already registered.
        
        {
            if ( isLoaded(name) )
            {
                // Name is already registered. Append a '*' and recall this function.
                
                return load(name + '*', filepath);
            }
        }
        
        if ( !filepath.empty() )
        {
            // Iterates through the Loader to find one able to load this file.
            
            auto loadermap = iLoaders.getLoaders();
            
            for ( auto it = loadermap.begin(); it != loadermap.end(); it++ )
            {
                if ( it->second->isLoadable(filepath) )
                {
                    // File is loadable by this loader, so use it.
                    
                    MeshHolder holder = it->second->load(name, filepath);
                    iMeshes.add(holder);
                    
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Resource '" << name << "' registered." << std::endl;
#endif
                    
                    return Mesh ( holder );
                }
            }
            
#ifdef GreIsDebugMode
            GreDebugPretty() << "No loader found for Resource '" << name << "'." << std::endl;
#endif
            return Mesh ( nullptr );
        }
        
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "No filepath given to load Resource '" << name << "'." << std::endl;
#endif
            return Mesh ( nullptr );
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "No name given for Resource's file '" << filepath << "'." << std::endl;
#endif
        return Mesh ( nullptr );
    }
}

bool MeshManager::isLoaded(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( auto holder : iMeshes )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return true;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Resource Mesh '" << name << "' not found." << std::endl;
#endif
        
        return false;
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Invalid name given." << std::endl;
#endif
        return false;
    }
}

Mesh MeshManager::get(const std::string &name, const std::string &filepath)
{
    if ( !name.empty() )
    {
        // Check if already loaded.
        
        for ( auto holder : iMeshes )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Mesh ( holder );
                }
            }
        }
        
        // If we are at this point, the 'name' Mesh was not found. Try to load using
        // the 'filepath'.
        
        return load(name, filepath);
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' parameter is empty." << std::endl;
#endif
        return Mesh ( nullptr );
    }
}

const Mesh MeshManager::get(const std::string &name) const
{
    if ( !name.empty() )
    {
        for ( auto holder : iMeshes )
        {
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    return Mesh ( holder );
                }
            }
        }
        
#ifdef GreIsDebugMode 
        GreDebugPretty() << "Resource Mesh '" << name << "' not found." << std::endl;
#endif
        
        return Mesh ( nullptr );
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "'name' parameter is empty." << std::endl;
#endif
        return Mesh ( nullptr );
    }
}

void MeshManager::unload(const std::string &name)
{
    if ( !name.empty() )
    {
        for ( auto it = iMeshes.begin(); it != iMeshes.end(); it++ )
        {
            auto holder = (*it);
            
            if ( !holder.isInvalid() )
            {
                if ( holder->getName() == name )
                {
                    iMeshes.erase(it);
#ifdef GreIsDebugMode
                    GreDebugPretty() << "Resource Mesh '" << name << "' unloaded." << std::endl;
#endif
                    return;
                }
            }
        }
        
#ifdef GreIsDebugMode
        GreDebugPretty() << "Resource Mesh '" << name << "' not found." << std::endl;
#endif
    }
}

void MeshManager::clearMeshes()
{
    iMeshes.clear();
}

MeshLoaderFactory& MeshManager::getLoaderFactory()
{
    return iLoaders;
}

const MeshLoaderFactory& MeshManager::getLoaderFactory() const
{
    return iLoaders;
}

void MeshManager::clear()
{
    iMeshes.clear();
    iLoaders.clear();
}

GreEndNamespace
