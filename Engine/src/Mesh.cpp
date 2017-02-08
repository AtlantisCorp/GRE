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

// Because of 'MeshManager::CreateSquare()' , we must use the ResourceManager to retrieve the
// MeshManager in place .
#include "ResourceManager.h"

GreBeginNamespace

Mesh::Mesh ( )
: Gre::Resource( )
, iIndexBuffer ( nullptr ) , iBoundingBox ( )
, iAutomateBoundingBox( false ), iOriginalFile ( "" )
{
    
}

Mesh::Mesh ( const std::string & name )
: Gre::Resource( ResourceIdentifier::New() , name )
, iIndexBuffer( nullptr ) , iBoundingBox ( )
, iAutomateBoundingBox( false ) , iOriginalFile( "" )
{
    
}

Mesh::~Mesh() noexcept ( false )
{
    
}

void Mesh::setVertexBuffers(const MeshAttributeList &attributes)
{
    GreAutolock ;
    
    if ( !iVertexBuffers.empty() )
        clearVertexBuffers () ;
    
    for ( const MeshAttribute & attribute : attributes )
        addVertexBuffer(attribute) ;
}

const MeshAttributeList & Mesh::getVertexBuffers() const
{
    GreAutolock ; return iVertexBuffers ;
}

void Mesh::addVertexBuffer(const Gre::MeshAttribute &attribute)
{
    GreAutolock ;
    
    iVertexBuffers.push_back(attribute) ;
    addListener( HardwareVertexBufferUser(attribute.buffer) ) ;
    
    if ( attribute.enabled )
    {
        if ( !attribute.buffer.isInvalid() )
        {
            if ( attribute.buffer->getVertexDescriptor().getComponentLocation(VertexComponentType::Position) != -1 )
            {
                // This buffer has Positions Vertex , so register on next update event to update the bounding
                // box , if we can . The workaround to use the lambda is to pass a pointer to the mesh attribute
                // in order not to copy it. In fact, the lambda will copy the pointer. In libc++, a 'bug' disallow
                // the bracing of objects with a throw possibility.
                // We assume that the mesh attribute will not be destroyed betwen this and the update event, wich
                // should be a very little time.
                if ( iAutomateBoundingBox )
                {
                    const MeshAttribute* ptr = &attribute ;
                    addNextEventCallback(EventType::Update, [this , ptr] (const EventHolder &) {
                        this->iUpdateBoundingBox ( *ptr ) ;
                    });
                }
            }
        }
    }
}

void Mesh::setIndexBuffer(const HardwareIndexBufferHolder &buffer)
{
    GreAutolock ;
    
    if ( !iIndexBuffer.isInvalid() )
        clearIndexBuffer () ;
    
    iIndexBuffer = buffer ;
    addListener( HardwareIndexBufferUser(buffer) ) ;
}

const HardwareIndexBufferHolder & Mesh::getIndexBuffer() const
{
    GreAutolock ; return iIndexBuffer ;
}

void Mesh::unload ()
{
    GreAutolock ;
    
    iVertexBuffers.clear() ;
    iIndexBuffer.clear() ;
    iBoundingBox.clear() ;
    iAutomateBoundingBox = true ;
    Resource::unload() ;
}

void Mesh::bind() const
{
    GreAutolock ;
    
    // This is an example of what to do , but you should really overwrite it.
    // Here we only bind every buffers enabled , more the index buffer if there
    // is one . But one can bind other things related to API - specific data.
    
    for ( const MeshAttribute & attr : iVertexBuffers )
    {
        if ( attr.enabled && !attr.buffer.isInvalid() )
        {
            attr.buffer->bind() ;
        }
    }
    
    if ( !iIndexBuffer.isInvalid() )
        iIndexBuffer->bind() ;
}

void Mesh::clearVertexBuffers()
{
    GreAutolock ;
    
    for ( MeshAttribute& attr : iVertexBuffers )
        removeListener( HardwareVertexBufferUser(attr.buffer) ) ;
    iVertexBuffers.clear() ;
}

void Mesh::clearIndexBuffer()
{
    GreAutolock ;
    
    removeListener( HardwareIndexBufferUser(iIndexBuffer) ) ;
    iIndexBuffer.clear() ;
}

void Mesh::setBoundingBox ( const BoundingBox & bbox )
{
    GreAutolock ; iBoundingBox = bbox ;
}

const BoundingBox & Mesh::getBoundingBox ( ) const
{
    GreAutolock ; return iBoundingBox ;
}

void Mesh::iUpdateBoundingBox(const Gre::MeshAttribute &attribute)
{
    GreAutolock ;
    
    // When calling this function , the BoundingBox should be updated using the Positions
    // in the given buffer .
    if ( iAutomateBoundingBox && attribute.enabled && !attribute.buffer.isInvalid() )
    {
        int positionloc = attribute.buffer->getVertexDescriptor().getComponentLocation(VertexComponentType::Position) ;
        size_t stride = attribute.buffer->getVertexDescriptor().getStride(VertexComponentType::Position) ;
        size_t positioncount = attribute.buffer->count() ;
        size_t positioncur = 0 ;
        
        if ( positionloc >= -1 )
        {
            for ( const char * first = attribute.buffer->getData() + positionloc ; positioncur != positioncount ;
                 first += stride , positioncur ++ )
            {
                const Vector3 * pos = (const Vector3 *) first ;
                iBoundingBox.add( *pos ) ;
            }
        }
    }
}

const std::string & Mesh::getOriginalFilepath () const
{
    GreAutolock ; return iOriginalFile ;
}

void Mesh::setOriginalFilepath ( const std::string & filepath )
{
    GreAutolock ; iOriginalFile = filepath ;
}

// ---------------------------------------------------------------------------------------------------

MeshLoader::MeshLoader ()
{
    
}

MeshLoader::~MeshLoader () noexcept ( false )
{
    
}

// ---------------------------------------------------------------------------------------------------

MeshHolder MeshManager::CreateSquare ( const Surface & surface )
{
    MeshManagerHolder manager = ResourceManager::Get() .getMeshManager() ;
    
    if ( manager.isInvalid() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty () << "Please load a MeshManager before using this function." << Gre::gendl;
#endif
        return MeshHolder ( nullptr ) ;
    }
    
    return manager->iCreateSquare ( surface ) ;
}

MeshManager::MeshManager ( )
: SpecializedResourceManager ( )
{
    
}

MeshManager::~MeshManager ( ) noexcept ( false )
{
    
}

MeshUser MeshManager::load ( const std::string & name , const std::string & filepath )
{
    GreAutolock ;
    
    {
        MeshUser check = findFirst ( name ) ;
        
        if ( !check.isInvalid() )
        {
            return load ( name + "*" , filepath ) ;
        }
        
        check = findFirstFile ( filepath ) ;
        
        if ( !check.isInvalid() )
        {
            return check ;
        }
    }
    
    MeshLoader * bestloader = iFindBestLoader ( filepath ) ;
    
    if ( !bestloader )
    {
#ifdef GreIsDebugMode
        GreDebugPretty () << "No loader found for filepath '" << filepath << "'." << Gre::gendl ;
#endif
        return MeshUser ( nullptr ) ;
    }
    
    MeshHolder holder = bestloader->load ( name , filepath ) ;
    
    if ( holder.isInvalid() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty () << "Filepath '" << filepath << "' could not be loaded." << Gre::gendl ;
#endif
        return MeshUser ( nullptr ) ;
    }
    
    MeshHolder convertedmesh = iConvertMesh ( holder ) ;
    
    if ( convertedmesh.isInvalid() )
    {
#ifdef GreIsDebugMode
        GreDebugPretty () << "Filepath '" << filepath << "' could not be converted to Gre::Mesh." << Gre::gendl ;
#endif
        return MeshUser ( nullptr ) ;
    }
    
    iHolders.add ( convertedmesh ) ;
    addListener ( MeshUser(convertedmesh) ) ;
    
    return convertedmesh ;
}

MeshUser MeshManager::findFirstFile(const std::string &filepath)
{
    for ( auto mesh : iHolders ) {
        if ( !mesh.isInvalid() && mesh->getOriginalFilepath() == filepath )
            return mesh ;
        
    }
    
    return MeshUser ( nullptr ) ;
}

MeshHolder MeshManager::iConvertMesh ( const MeshHolder & srcmesh ) const
{
    return srcmesh ;
}

MeshHolder MeshManager::iCreateSquare ( const Surface & surface )
{
    float squarepos [] =
    {
        static_cast<float>(surface.top) , static_cast<float>(surface.left) , 0.0f ,
        static_cast<float>(surface.top) , static_cast<float>(surface.left + surface.width) , 0.0f ,
        static_cast<float>(surface.top + surface.height) , static_cast<float>(surface.left + surface.width) , 0.0f ,
        static_cast<float>(surface.top + surface.height) , static_cast<float>(surface.left) , 0.0f
    } ;
    
    unsigned short squaretri [] =
    {
        0 , 1 , 2 ,
        2 , 3 , 0
    } ;
    
    // Create our Mesh .
    
    MeshHolder mesh = MeshHolder ( new Mesh () ) ;
    
    // Creates the SoftwareVertexBuffer .
    
    SoftwareVertexBufferHolder vbuf = SoftwareVertexBufferHolder ( new SoftwareVertexBuffer("") ) ;
    
    VertexDescriptor vdesc ; vdesc.addComponent ( VertexComponentType::Position ) ;
    vbuf->setVertexDescriptor ( vdesc ) ;
    
    vbuf->addData ( (const char*) squarepos , 4 * sizeof(float) ) ;
    mesh->addVertexBuffer ({ vbuf , true }) ;
    
    // Creates the SoftwareIndexBuffer .
    
    SoftwareIndexBufferHolder ibuf = SoftwareIndexBufferHolder ( new SoftwareIndexBuffer("") ) ;
    
    IndexDescriptor idesc ; idesc.setType ( IndexType::UnsignedShort ) ;
    ibuf->setIndexDescriptor ( idesc , 0 ) ;
    
    ibuf->addData ( (const char*) squaretri , sizeof ( unsigned short ) * 6 ) ;
    mesh->setIndexBuffer ( ibuf ) ;
    
    // Try to convert this mesh . If we did not success , just return the not converted mesh .
    
    MeshHolder converted = iConvertMesh ( mesh ) ;
    
    if ( converted.isInvalid() )
    {
        iSquares [surface] = mesh ;
        return mesh ;
    }
    
    else
    {
        iSquares [surface] = converted ;
        return converted ;
    }
}

GreEndNamespace
