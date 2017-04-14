//////////////////////////////////////////////////////////////////////
//
//  ObjMeshLoader.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 20/02/2017.
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

#include "ObjMeshLoader.h"
#include "ResourceManager.h"

GreBeginNamespace

ObjMeshLoader::ObjMeshLoader () : Gre::MeshLoader()
{
    
}

ObjMeshLoader::~ObjMeshLoader() noexcept ( false )
{
    
}

ResourceLoader* ObjMeshLoader::clone() const
{
    return new ObjMeshLoader () ;
}

bool ObjMeshLoader::isLoadable(const std::string &filepath) const
{
    return filepath.substr(filepath.find_last_of(".") + 1) == "obj" ;
}

const std::string ObjMeshLoader::getDirectory() const
{
    return "OBJ" ;
}

struct OBJ_V
{
    float x, y, z ;
    
    bool operator == ( const OBJ_V& rhs ) const {
        return x == rhs.x && y == rhs.y && z == rhs.z ;
    }
};

struct OBJ_VT
{
    float u, v ;
    
    bool operator == ( const OBJ_VT& rhs ) const {
        return u == rhs.u && v == rhs.v ;
    }
};

struct OBJ_VERTEX
{
    OBJ_V vertice ;
    OBJ_VT texture ;
    
    OBJ_V normal ;
    OBJ_V binormal ;
    OBJ_V tangente ;
    
    bool operator == ( const OBJ_VERTEX& rhs ) const {
        return vertice == rhs.vertice && normal == rhs.normal && texture == rhs.texture ;
    }
};

struct OBJ_VERTEXNONORMAL
{
    OBJ_V vertice ;
    OBJ_VT texture ;
};

struct OBJ_VERTEXNOTEXTURE
{
    OBJ_V vertice , normal ;
};

struct OBJ_O
{
    std::string name ;
    std::vector < OBJ_VERTEX > vertexs ;
    std::vector < int > indices ;
    
    int32_t verticecount ;
    int32_t normalcount ;
    int32_t texcount ;
    
    /// @brief Bounding Box Max point.
    OBJ_V ptmin ;
    
    /// @brief Bouding Box Min point.
    OBJ_V ptmax ;
};

void CalculateTangentBinormal ( const OBJ_VERTEX& v1 , const OBJ_VERTEX& v2 , const OBJ_VERTEX& v3 , OBJ_V& tan , OBJ_V& bi )
{
    Vector3 diff1 ( v2.vertice.x - v1.vertice.x , v2.vertice.y - v1.vertice.y , v2.vertice.z - v1.vertice.z ) ;
    Vector3 diff2 ( v3.vertice.x - v1.vertice.x , v3.vertice.y - v1.vertice.y , v3.vertice.z - v1.vertice.z ) ;
    
    Vector2 tudiff ( v2.texture.u - v1.texture.u , v3.texture.u - v1.texture.u ) ;
    Vector2 tvdiff ( v2.texture.v - v1.texture.v , v3.texture.v - v1.texture.v ) ;
    
    float denominator = 1.0f / ( tudiff.x * tvdiff.y - tudiff.y * tvdiff.x ) ;
    
    // Calculate tangent .
    
    tan.x = (tvdiff.y * diff1.x - tvdiff.x * diff2.x) * denominator ;
    tan.y = (tvdiff.y * diff1.y - tvdiff.x * diff2.y) * denominator ;
    tan.z = (tvdiff.y * diff1.z - tvdiff.x * diff2.z) * denominator ;
    
    float tanlenght = sqrtf( (tan.x*tan.x) + (tan.y*tan.y) + (tan.z*tan.z) ) ;
    
    tan.x = tan.x / tanlenght ;
    tan.y = tan.y / tanlenght ;
    tan.z = tan.z / tanlenght ;
    
    // Calculate binormal .
    
    bi.x = (tudiff.x * diff2.x - tudiff.y * diff1.x) * denominator ;
    bi.y = (tudiff.x * diff2.y - tudiff.y * diff1.y) * denominator ;
    bi.z = (tudiff.x * diff2.z - tudiff.y * diff1.z) * denominator ;
    
    float bilenght = sqrtf( (bi.x*bi.x) + (bi.y*bi.y) + (bi.z*bi.z) ) ;
    
    bi.x = bi.x / bilenght ;
    bi.y = bi.y / bilenght ;
    bi.z = bi.z / bilenght ;
}

void _objecttomesh ( MeshHolderList& list , OBJ_O* obj )
{
    if ( obj )
    {
        if ( obj->verticecount == 0 )
            return ;
        
        MeshHolder mesh = MeshHolder ( new Mesh(obj->name) ) ;
        
        if ( obj->indices.size() )
        {
            IndexDescriptor idesc ;
            idesc.setMode(IndexDrawmode::Triangles);
            idesc.setType(IndexType::UnsignedInteger);
            HardwareIndexBufferHolder ibuf = ResourceManager::Get()->getMeshManager()->
                createIndexBuffer(&obj->indices[0], obj->indices.size()*sizeof(unsigned int), idesc) ;
            mesh -> setIndexBuffer(ibuf) ;
            ibuf -> setEnabled(true) ;
        }
        
        if ( obj->verticecount && obj->normalcount && obj->texcount )
        {
            VertexDescriptor vdesc ;
            
            vdesc.addComponent(VertexAttribAlias::Position, 3, VertexAttribType::Float, false, 3 * sizeof(float)) ;
            vdesc.addComponent(VertexAttribAlias::Texture, 2, VertexAttribType::Float, false, 2 * sizeof(float)) ;
            
            vdesc.addComponent(VertexAttribAlias::Normal, 3, VertexAttribType::Float, true, 3 * sizeof(float)) ;
            vdesc.addComponent(VertexAttribAlias::Binormals, 3, VertexAttribType::Float, true, 3 * sizeof(float)) ;
            vdesc.addComponent(VertexAttribAlias::Tangents, 3, VertexAttribType::Float, true, 3 * sizeof(float)) ;
            
            HardwareVertexBufferHolder buf = ResourceManager::Get()->getMeshManager()->
                createVertexBuffer(&obj->vertexs[0], obj->vertexs.size()*sizeof(OBJ_VERTEX), vdesc) ;
            HardwareVertexBufferHolderList buflist ; buflist.add(buf) ;
            
            mesh -> setVertexBuffers(buflist) ;
            buf -> setEnabled(true) ;
        }
        
        else if ( obj->verticecount && obj->normalcount )
        {
            std::vector<OBJ_VERTEXNOTEXTURE> tmp ;
            for ( OBJ_VERTEX& vertex : obj->vertexs ) tmp.push_back({vertex.vertice, vertex.normal}) ;
            
            VertexDescriptor vdesc ;
            
            vdesc.addComponent(VertexAttribAlias::Position, 3, VertexAttribType::Float, false, 3 * sizeof(float)) ;
            vdesc.addComponent(VertexAttribAlias::Normal, 3, VertexAttribType::Float, true, 3 * sizeof(float)) ;
            
            HardwareVertexBufferHolder buf = ResourceManager::Get()->getMeshManager()->
            createVertexBuffer(&tmp[0], tmp.size()*sizeof(OBJ_VERTEXNOTEXTURE), vdesc) ;
            
            HardwareVertexBufferHolderList buflist ; buflist.add(buf) ;
            mesh -> setVertexBuffers(buflist) ;
            buf -> setEnabled(true) ;
        }
        
        else if ( obj->verticecount && obj->texcount )
        {
            std::vector<OBJ_VERTEXNONORMAL> tmp ;
            for ( OBJ_VERTEX& vertex : obj->vertexs ) tmp.push_back({vertex.vertice, vertex.texture}) ;
            
            VertexDescriptor vdesc ;
            
            vdesc.addComponent(VertexAttribAlias::Position, 3, VertexAttribType::Float, false, 3 * sizeof(float)) ;
            vdesc.addComponent(VertexAttribAlias::Texture, 2, VertexAttribType::Float, false, 2 * sizeof(float)) ;
            
            HardwareVertexBufferHolder buf = ResourceManager::Get()->getMeshManager()->
            createVertexBuffer(&tmp[0], tmp.size()*sizeof(OBJ_VERTEXNONORMAL), vdesc) ;
            
            HardwareVertexBufferHolderList buflist ; buflist.add(buf) ;
            mesh -> setVertexBuffers(buflist) ;
            buf -> setEnabled(true) ;
        }
        
        else
        {
            std::vector<OBJ_V> tmp ;
            for ( OBJ_VERTEX& vertex : obj->vertexs ) tmp.push_back(vertex.vertice) ;
            
            VertexDescriptor vdesc ;
            vdesc.addComponent(VertexAttribAlias::Position, 3, VertexAttribType::Float, false, 3 * sizeof(float)) ;
            
            HardwareVertexBufferHolder buf = ResourceManager::Get()->getMeshManager()->
            createVertexBuffer(&tmp[0], tmp.size()*sizeof(OBJ_V), vdesc) ;
            
            HardwareVertexBufferHolderList buflist ; buflist.add(buf) ;
            mesh -> setVertexBuffers(buflist) ;
            buf -> setEnabled(true) ;
        }
        
        //////////////////////////////////////////////////////////////////////
        // Computes boundingbox.
        
        BoundingBox bbox ({ obj->ptmax.x, obj->ptmax.y, obj->ptmax.z } ,
                          { obj->ptmin.x, obj->ptmin.y, obj->ptmin.z });
        mesh -> setBoundingBox(bbox) ;
        
        list.add(mesh) ;
    }
}

MeshHolderList ObjMeshLoader::load(const std::string &filepath, const ResourceLoaderOptions & ops) const
{
    std::ifstream stream ( filepath , std::ios_base::in ) ;
    if ( !stream ) {
#ifdef GreIsDebugMode
        GreDebug("[WARN] Can't open file '") << filepath << "'." << Gre::gendl ;
#endif
        return MeshHolderList () ;
    }
    
    // We have to load the file line by line. So, when loading, we will generate one
    // or more mesh, when 'o' is encountered.
    
    MeshHolderList ret ;
    OBJ_O * current = new OBJ_O ;
    current -> verticecount = 0 ;
    current -> normalcount = 0 ;
    current -> texcount = 0 ;
    
    // These are globals variables.
    
    std::vector < OBJ_V > vertices ;
    std::vector < OBJ_VT > textures ;
    std::vector < OBJ_V > normals ;
    
    while ( !stream.eof() )
    {
        // Get the line and analyze it.
        std::string l ; std::getline(stream, l) ;
        std::stringstream line ( l ) ;
        
        while ( !line.eof() )
        {
            std::string next ; line >> next ;
            
            if ( next == "#" ) {
                std::getline(line, next);
                break;
            }
            
            else if ( next == "o" ) {
                
                if ( current ) {
                    _objecttomesh(ret, current) ;
                    delete current ;
                    current = nullptr ;
                }
                
                current = new OBJ_O ;
                current -> verticecount = 0 ;
                current -> normalcount = 0 ;
                current -> texcount = 0 ;
                line >> current -> name ;
            }
            
            else if ( next == "v" ) {
                
                OBJ_V v ; line >> v.x >> v.y >> v.z ;
                vertices.push_back(v) ;
            }
            
            else if ( next == "vt" ) {
                
                OBJ_VT vt ; line >> vt.u >> vt.v ;
                textures.push_back(vt) ;
            }
            
            else if ( next == "vn" ) {
                
                OBJ_V v ; line >> v.x >> v.y >> v.z ;
                normals.push_back(v) ;
            }
            
            else if ( next == "f" ) {
                
                if ( current ) {
                    
                    bool generatenormal = true ;
                    std::vector < OBJ_VERTEX > newvertex ;
                    
                    int texcount = 0 ;
                    
                    while ( !line.eof() )
                    {
                        line >> next ;
                        
                        OBJ_VERTEX vertex ;
                        
                        std::vector < std::string > splitted = split(next, '/') ;
                        
                        if ( splitted.size() ) {
                            vertex.vertice = vertices.at( atoi(splitted.at(0).c_str()) - 1 ) ;
                            vertex.normal = { -1, -1, -1 } ;
                            vertex.texture = { -1, -1 } ;
                        }
                        
                        std::size_t sep1 = next.find_first_of('/') ;
                        std::size_t sep2 = next.find_last_of('/') ;
                        
                        if ( sep2 - sep1 == 1 && splitted.size() == 3) {
                            vertex.normal = normals.at( atoi(splitted.at(2).c_str()) - 1 ) ;
                            generatenormal = false ;
                        } else if ( splitted.size() >= 2 ) {
                            vertex.texture = textures.at( atoi(splitted.at(1).c_str()) - 1 ) ;
                            texcount ++ ;
                            
                            if ( splitted.size() == 3 ) {
                                vertex.normal = normals.at( atoi(splitted.at(2).c_str()) - 1 ) ;
                                generatenormal = false ;
                            }
                            
                        }
                        
                        newvertex.push_back(vertex);
                    }
                    
                    if ( newvertex.size() == 3 && generatenormal )
                    {
                        Vector3 V1 ( newvertex[0].vertice.x , newvertex[0].vertice.y , newvertex[0].vertice.z ) ;
                        Vector3 V2 ( newvertex[1].vertice.x , newvertex[1].vertice.y , newvertex[1].vertice.z ) ;
                        Vector3 V3 ( newvertex[2].vertice.x , newvertex[2].vertice.y , newvertex[2].vertice.z ) ;
                        
                        Vector3 N = glm::cross(V1 - V2, V2 - V3) ;
                        N = glm::normalize(N) ;
                        
                        OBJ_V n ; n.x = N.x ; n.y = N.y ; n.z = N.z ;
                        newvertex[0].normal = newvertex[1].normal = newvertex[2].normal = n ;
                    }
                    
                    if ( newvertex.size() == 3 && texcount == 3 )
                    {
                        // Generate tangents and binormals for this Face.
                        
                        OBJ_V tan , bi ;
                        CalculateTangentBinormal(newvertex[0], newvertex[1], newvertex[2], tan, bi) ;
                        
                        newvertex[0] . tangente = tan ;
                        newvertex[1] . tangente = tan ;
                        newvertex[2] . tangente = tan ;
                        
                        newvertex[0] . binormal = bi ;
                        newvertex[1] . binormal = bi ;
                        newvertex[2] . binormal = bi ;
                    }
                    
                    for ( OBJ_VERTEX & vertex : newvertex )
                    {
                        //////////////////////////////////////////////////////////////////////
                        // If the object has no vertexes yet , this means the current processed
                        // vertex will take place has min and max point.
                        
                        if ( current -> vertexs.empty() )
                        {
                            current -> ptmax = vertex.vertice ;
                            current -> ptmin = vertex.vertice ;
                        }
                        
                        else
                        {
                            if ( current->ptmax.x < vertex.vertice.x ) current->ptmax.x = vertex.vertice.x ;
                            if ( current->ptmax.y < vertex.vertice.y ) current->ptmax.y = vertex.vertice.y ;
                            if ( current->ptmax.z < vertex.vertice.z ) current->ptmax.z = vertex.vertice.z ;
                            
                            if ( current->ptmin.x > vertex.vertice.x ) current->ptmin.x = vertex.vertice.x ;
                            if ( current->ptmin.y > vertex.vertice.y ) current->ptmin.y = vertex.vertice.y ;
                            if ( current->ptmin.z > vertex.vertice.z ) current->ptmin.z = vertex.vertice.z ;
                        }
                        
                        //////////////////////////////////////////////////////////////////////
                        // Computes the vertice indice.
                        
                        auto it = std::find(current->vertexs.begin(), current->vertexs.end(), vertex) ;
                        if ( it == current->vertexs.end() ) {
                            current->vertexs.push_back(vertex) ;
                            current->indices.push_back(current->vertexs.size() - 1) ;
                            if ( texcount )
                                current -> texcount ++ ;
                            current -> normalcount ++ ;
                            current -> verticecount ++ ;
                        } else {
                            current->indices.push_back( it - current->vertexs.begin() ) ;
                        }
                    }
                    
                }
            }
        }
    }
    
    if ( current )
    {
        _objecttomesh(ret, current) ;
        delete current ;
        current = nullptr ;
    }
    
    return ret ;
}

GreEndNamespace
