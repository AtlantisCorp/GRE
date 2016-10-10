/* 
//
//  mypluginmain.cpp
//  GResource
//
//  Created by Jacques Tronconi on 05/11/2015.
//  Copyright (c) 2015 Atlanti's Corporation. All rights reserved.
//

#include "Version.h"
#include "MeshLoader.h"
#include "ResourceManager.h"

#include <fstream>
#include <sstream>

using namespace Gre;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

struct ObjVDesc
{
    int v;
    int n;
    int t;
};

struct ObjFace
{
    std::vector<ObjVDesc> descs;
};

struct Object
{
    std::string name;
    std::vector<Vector3> vertexs;
    std::vector<ObjFace> faces;
};

class ObjModelLoader : public MeshLoader
{
public:
    
    ObjModelLoader() { }
    ObjModelLoader(const ObjModelLoader&) { }
    ~ObjModelLoader() { }
    
    Resource* load(Resource::Type type, const std::string& name, const std::string& file, Renderer renderer) const
    {
        std::ifstream stream (file);
        if(!stream) {
            GreDebugPretty() << "Can't find file " << file << "." << std::endl;
            return nullptr;
        }
        
        std::vector<Object> objects;
        Object* curobj = nullptr;
        
        std::string cur;
        while (std::getline(stream, cur))
        {
            std::stringstream line(cur);
            std::string met;
            
            while(line >> met)
            {
                if(met == "o")
                {
                    if(curobj)
                        objects.push_back(*curobj);
                    curobj = new Object;
                    line >> (curobj->name);
                }
                
                else if(met == "v" && curobj)
                {
                    float x,y,z;
                    line >> x >> y >> z;
                    curobj->vertexs.push_back(Vector3(x,y,z));
                }
                
                else if(met == "f" && curobj)
                {
                    ObjFace face;
                    char* pend;
                    
                    std::string desc1, desc2, desc3, desc4;
                    line >> desc1 >> desc2 >> desc3 >> desc4;
                    
                    std::vector<std::string> tmp = split(desc1, '/');
                    if(tmp.size() == 1) { // v
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = 0;
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 2) { // v/t
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 3) { // v/t/n
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = std::strtol(tmp[2].c_str(), &pend, 10);
                        face.descs.push_back(vdesc);
                    }
                    
                    tmp = split(desc2, '/');
                    if(tmp.size() == 1) { // v
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = 0;
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 2) { // v/t
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 3) { // v/t/n
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = std::strtol(tmp[2].c_str(), &pend, 10);
                        face.descs.push_back(vdesc);
                    }
                    
                    tmp = split(desc3, '/');
                    if(tmp.size() == 1) { // v
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = 0;
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 2) { // v/t
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 3) { // v/t/n
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = std::strtol(tmp[2].c_str(), &pend, 10);
                        face.descs.push_back(vdesc);
                    }
                    
                    tmp = split(desc4, '/');
                    if(tmp.size() == 1) { // v
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = 0;
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 2) { // v/t
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = 0;
                        face.descs.push_back(vdesc);
                    } else if(tmp.size() == 3) { // v/t/n
                        ObjVDesc vdesc;
                        vdesc.v = std::strtol(tmp[0].c_str(), &pend, 10);
                        vdesc.t = std::strtol(tmp[1].c_str(), &pend, 10);
                        vdesc.n = std::strtol(tmp[2].c_str(), &pend, 10);
                        face.descs.push_back(vdesc);
                    }
                    
                    curobj->faces.push_back(face);
                }
            }
        }
        
        if(curobj)
            objects.push_back(*curobj);
        
        GreDebugPretty() << "Loaded Objects : " << objects.size() << std::endl;
        for(auto obj : objects)
        {
            GreDebugPretty() << obj.name << " -> Vertexs : " << obj.vertexs.size() << std::endl;
            GreDebugPretty() << obj.name << " -> Faces   : " << obj.faces.size() << std::endl;
        }
        
        if(objects.size())
        {
            // For now we only support object 1.
            Object& obj = objects[0];
            
            // An object has many vertexs. We will get them.
            VertexBatch vbatch;
            for(auto v : obj.vertexs) {
                vbatch.push_back({v, Vector2(), Color()});
            }
            
            // We create the VertexBuffer object.
            HardwareVertexBuffer vbuf = renderer.createVertexBuffer();
            vbuf.add(vbatch);
            vbuf.activateColor(false);
            
            // Now for the indices.
            // For now we only support Vertices indices, triangles type.
            HardwareIndexBuffer ibuf = renderer.createIndexBuffer(PrimitiveType::Triangles, StorageType::UnsignedInt);
            IndexedFaceBatch fbatch;
            
            for (auto face : obj.faces) {
                IndexedFace f;
                f.indices.push_back(face.descs[0].v - 1);
                f.indices.push_back(face.descs[1].v - 1);
                f.indices.push_back(face.descs[2].v - 1);
                f.material = Material::Null;
                fbatch.push_back(f);
            }
            ibuf.add(fbatch);
            ibuf.setMaterial(Material::Null);
            
            HardwareIndexBufferBatch ibufs;
            ibufs.batchs.push_back(ibuf);
            
            return MeshLoader::load(Resource::Type::Mesh, name, vbuf, ibufs);
        }
        
        else
        {
            return nullptr;
        }
    }
    
    ResourceLoader* clone() const
    {
        return new ObjModelLoader;
    }
};

extern "C" DLL_PUBLIC void StartPlugin (void)
{
    ResourceManager::Get().getMeshLoaderFactory().registers("ObjMeshLoader", new ObjModelLoader);
}

extern "C" DLL_PUBLIC void StopPlugin (void)
{
    
}

extern "C" DLL_PUBLIC void* GetPluginName (void)
{
    return (void*) "ObjModelLoader";
}

*/

// ObjLoader.cpp : Main Plugin file to load Wavefront OBJ Meshes files.
// Author : Luk2010
// Copyright : Atlanti's Corporation , 2016

#include "ResourceManager.h"
#include "WavefrontLoader.h"

extern "C" DLL_PUBLIC void* GetPluginName ( void )
{
    return ( void* ) "Wavefront OBJ Mesh Loader";
}

extern "C" DLL_PUBLIC void StartPlugin ( void )
{
    Gre::MeshManager& meshmanager = Gre::ResourceManager::Get().getMeshManager();
    WavefrontLoader* loader = new WavefrontLoader ( );
    meshmanager.getLoaderFactory().registers("WavefrontLoader", loader);
    
    GreDebugPretty() << "WavefrontLoader Plugin ready." << std::endl;
}

extern "C" DLL_PUBLIC void StopPlugin ( void )
{
    
}

