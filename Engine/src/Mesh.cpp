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
: Gre::Resource(name)
, iSoftVertexBuffer(nullptr)
, iSoftIndexBuffer(nullptr)
, iHardVertexBuffer(nullptr)
, iHardIndexBuffer(nullptr)
, iSoftBuffersChanged(false)
, iHardBuffersChanged(false)
, iUseHardwareBuffers(false)
, iSoftVertexBufferUpdate(false)
, iSoftIndexBufferUpdate(false)
{
    
}

MeshPrivate::~MeshPrivate() noexcept(false)
{
    
}

const SoftwareVertexBuffer MeshPrivate::getSoftwareVertexBuffer() const
{
    return SoftwareVertexBuffer(iSoftVertexBuffer);
}

SoftwareVertexBufferHolder& MeshPrivate::getSoftwareVertexBufferHolder()
{
    return iSoftVertexBuffer;
}

void MeshPrivate::setSoftwareVertexBuffer(const Gre::SoftwareVertexBuffer &softvertexbuffer)
{
    SoftwareVertexBufferHolder newbuf = softvertexbuffer.lock();
    
    if ( newbuf != iSoftVertexBuffer )
    {
        // We should remove old Software Vertex Buffer from listeners, then replace with new one.
        
        if ( !iSoftVertexBuffer.isInvalid() )
        {
            Resource::removeListener( iSoftVertexBuffer->getName() );
        }
        
        iSoftVertexBuffer = newbuf;
        iSoftBuffersChanged = true;
        iSoftVertexBufferUpdate = true;
        
        if ( !newbuf.isInvalid() )
        {
            Resource::addListener( softvertexbuffer );
        }
    }
}

const SoftwareIndexBuffer MeshPrivate::getSoftwareIndexBuffer() const
{
    return SoftwareIndexBuffer(iSoftIndexBuffer);
}

SoftwareIndexBufferHolder& MeshPrivate::getSoftwareIndexBufferHolder()
{
    return iSoftIndexBuffer;
}

void MeshPrivate::setSoftwareIndexBuffer(const Gre::SoftwareIndexBuffer &softindexbuffer)
{
    SoftwareIndexBufferHolder newbuf = softindexbuffer.lock();
    
    if ( newbuf != iSoftIndexBuffer )
    {
        // We should remove old Software Index Buffer from listeners, then replace with new one.
        
        if ( !iSoftIndexBuffer.isInvalid() )
        {
            Resource::removeListener( iSoftIndexBuffer->getName() );
        }
        
        iSoftIndexBuffer = newbuf;
        iSoftBuffersChanged = true;
        iSoftIndexBufferUpdate = true;
        
        if ( !newbuf.isInvalid() )
        {
            Resource::addListener( softindexbuffer );
        }
    }
}

const HardwareVertexBuffer MeshPrivate::getVertexBuffer() const
{
    return HardwareVertexBuffer(iHardVertexBuffer);
}

void MeshPrivate::setVertexBuffer(const HardwareVertexBuffer& vertexbuffer)
{
    HardwareVertexBufferHolder newbuf = vertexbuffer.lock();
    
    if ( newbuf != iHardVertexBuffer )
    {
        if ( !iHardVertexBuffer.isInvalid() )
        {
            Resource::removeListener( iHardVertexBuffer->getName() );
        }
        
        iHardVertexBuffer = newbuf;
        iHardBuffersChanged = true;
        
        if ( !iHardVertexBuffer.isInvalid() )
        {
            Resource::addListener( vertexbuffer );
        }
    }
}

const HardwareIndexBuffer MeshPrivate::getIndexBuffer() const
{
    return HardwareIndexBuffer(iHardIndexBuffer);
}

void MeshPrivate::setIndexBuffer(const HardwareIndexBuffer& indexbuffer)
{
    HardwareIndexBufferHolder newbuf = indexbuffer.lock();
    
    if ( newbuf != iHardIndexBuffer )
    {
        if ( !iHardIndexBuffer.isInvalid() )
        {
            Resource::removeListener( iHardIndexBuffer->getName() );
        }
        
        iHardIndexBuffer = newbuf;
        iHardBuffersChanged = true;
        
        if ( !iHardIndexBuffer.isInvalid() )
        {
            Resource::addListener( indexbuffer );
        }
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
    
    iBoundingBox.clear();
    iSoftBuffersChanged = false;
    iHardBuffersChanged = false;
    iUseHardwareBuffers = false;
    iSoftVertexBufferUpdate = false;
    iSoftIndexBufferUpdate = false;
}

void MeshPrivate::clearSoftwareBuffers()
{
    iSoftVertexBuffer.reset();
    iSoftIndexBuffer.reset();
    iSoftBuffersChanged = false;
    iSoftVertexBufferUpdate = false;
    iSoftIndexBufferUpdate = false;
}

void MeshPrivate::clearHardwareBuffers()
{
    iHardVertexBuffer.reset();
    iHardIndexBuffer.reset();
    iHardBuffersChanged = false;
}

const BoundingBox& MeshPrivate::getBoundingBox() const
{
    return iBoundingBox;
}

void MeshPrivate::onUpdateEvent(const Gre::UpdateEvent &e)
{
    if ( iSoftVertexBufferUpdate )
    {
        if ( !iHardVertexBuffer.isInvalid() )
        {
            iHardVertexBuffer->setData( HardwareVertexBufferHolder(iSoftVertexBuffer.get()) );
        }
        
        if ( !iIsBoundingBoxUser && !iSoftVertexBuffer.isInvalid() )
        {
            iBoundingBox = iSoftVertexBuffer->getBoundingBox();
        }
        
        iSoftVertexBufferUpdate = false;
    }
    
    if ( iSoftIndexBufferUpdate )
    {
        if ( !iHardIndexBuffer.isInvalid() )
        {
            iHardIndexBuffer->setData( HardwareIndexBufferHolder(iSoftIndexBuffer.get()) );
        }
        
        iSoftIndexBufferUpdate = false;
    }
    
    iSoftBuffersChanged = false;
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

const BoundingBox& Mesh::getBoundingBox() const
{
    auto ptr = lock();
    if ( ptr )
        return ptr->getBoundingBox();
    throw GreInvalidUserException("Mesh");
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

Mesh MeshManager::createRectangle(const Gre::Surface &surface)
{
    if ( surface.height > 0 && surface.width > 0 )
    {
        std::string name = std::string ( "MeshRectangle#" ) + std::to_string(iMeshes.size());
        std::string svbname = name + "/svb";
        std::string sibname = name + "/sib";
        
        // To create a Mesh, we have to fill a SoftwareVertexBuffer.
        
        SoftwareVertexBufferHolder svbholder = SoftwareVertexBufferHolder ( new SoftwareVertexBufferPrivate(svbname) );
        
        if ( !svbholder.isInvalid() )
        {
            VertexDescriptor vdesc;
            vdesc << VertexComponentType::Position;
            svbholder->setVertexDescriptor(vdesc);
            
            VertexPosition* data = (VertexPosition*) new VertexPosition[4];
            data[0] = VertexPosition ( surface.left ,                 surface.top ,                  0.0f );
            data[1] = VertexPosition ( surface.left + surface.width , surface.top ,                  0.0f );
            data[2] = VertexPosition ( surface.left + surface.width , surface.top - surface.height , 0.0f );
            data[3] = VertexPosition ( surface.left ,                 surface.top - surface.height , 0.0f );
            
            svbholder->addData(data, sizeof(VertexPosition) * 4);
        }
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "SoftwareVertexBuffer '" << svbname << "' couldn't be created." << std::endl;
#endif
            return Mesh ( nullptr );
        }
        
        // We also try to fill a SoftwareIndexBuffer.
        
        SoftwareIndexBufferHolder sibholder = SoftwareIndexBufferHolder ( new SoftwareIndexBufferHolder(sibname) );
        
        if ( !sibholder.isInvalid() )
        {
            IndexDescriptor idesc;
            idesc.setType(IndexType::UnsignedInteger);
            
            unsigned int data[6] = { 0, 1, 2, 2, 3, 0 };
            
            sibholder->setIndexDescriptor(idesc, 0);
            sibholder->addDataToIndexBatch(data, sizeof(unsigned int) * 6, 0);
        }
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "SoftwareIndexBuffer '" << sibname << "' couldn't be created." << std::endl;
#endif
            return Mesh ( nullptr );
        }
        
        // Now we can add those two HardwareBuffer to a Mesh object.
        
        MeshHolder rectangle = MeshHolder ( new MeshPrivate(name) );
        
        if ( !rectangle.isInvalid() )
        {
            rectangle->setSoftwareVertexBuffer(SoftwareVertexBuffer(svbholder));
            rectangle->setSoftwareIndexBuffer(SoftwareIndexBuffer(sibholder));
            
            // Everything is set, just add this Mesh to the list and return it.
            
            iMeshes.add(rectangle);
            return Mesh ( rectangle );
        }
        else
        {
#ifdef GreIsDebugMode
            GreDebugPretty() << "Mesh '" << name << "' couldn't be created." << std::endl;
#endif
            return Mesh ( nullptr );
        }
    }
    
    else
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Can't create a Mesh Resource using null Surface." << std::endl;
#endif
        return Mesh ( nullptr );
    }
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
