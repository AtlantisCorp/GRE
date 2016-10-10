//
//  WavefrontLoader.cpp
//  GRE
//
//  Created by Jacques Tronconi on 26/09/2016.
//
//

#include "ResourceManager.h"
#include "WavefrontLoader.h"
#include "WavefrontStructs.h"

WavefrontLoader::WavefrontLoader()
{
    
}

WavefrontLoader::~WavefrontLoader()
{
    
}

Gre::MeshHolder WavefrontLoader::load(const std::string &name, const std::string &filepath) const
{
    // First , we determine the File format. In case of a binary 'mod' format , this is for now
    // unsupported.
    
    std::string ext = filepath.substr(filepath.find_last_of(".") + 1);
    
    if ( ext == "mod" )
    {
        GreDebugPretty() << "Binary Wavefront file format not supported yet." << std::endl;
        return Gre::MeshHolder ( nullptr );
    }
    
    if ( ext == "obj" )
    {
        return iLoadAsciiWavefront ( name , filepath );
    }
    
    GreDebugPretty() << "Format '" << ext << "' not supported by this loader." << std::endl;
    return Gre::MeshHolder ( nullptr );
}

bool WavefrontLoader::isLoadable(const std::string &filepath) const
{
    // The Wavefront OBJ file is loadable only if the extension is 'obj' or 'mod'. We also try to load the file, to
    // ensure the file exists. The 'mod' extension file format signifies a binary Wavefront format.
    
    std::string ext = filepath.substr(filepath.find_last_of(".") + 1);
    
    if ( ext == "obj" || ext == "mod" )
    {
        std::ifstream filestream ( filepath );
        
        if ( filestream )
        {
            return true;
        }
        
        else
        {
            return false;
        }
    }
    
    return false;
}

Gre::ResourceLoader* WavefrontLoader::clone() const
{
    return new WavefrontLoader () ;
}

Gre::MeshHolder WavefrontLoader::iLoadAsciiWavefront(const std::string &name, const std::string &filepath) const
{
    std::ifstream filestream ( filepath );
    
    if ( !filestream )
    {
        GreDebugPretty() << "Can't open filepath '" << filepath << "'." << std::endl;
        return Gre::MeshHolder ( nullptr );
    }
    
    file_t wavefrontfile;
    
    // So , we must parse the whole file as :
    // Every v , vn , vt , vp statements are always assigned at a number.
    // Every o statement sets a new ind_base corresponding to the last v registered.
    // Every f statement creates a new face with indice will be added the ind_base.
    // Every usemtl statement uses a material which name is 'mtllib + / + materialname'
    
    std::string cur ;
    int ind_basev = 0;
    int ind_baset = 0;
    int ind_basen = 0;
    
    while ( std::getline(filestream, cur) )
    {
        std::stringstream line ( cur );
        std::string met;
        
        while ( line >> met )
        {
            if ( met == "#" ) // Comment line.
                break;
            
            if ( met == "mtllib" ) // Material Library.
            {
                line >> met;
                
                GreDebugPretty() << "Trying to load Material's library '" << met << "'..." << std::endl;
                
                Gre::MaterialManager& matmanager = Gre::ResourceManager::Get().getMaterialManager();
                Gre::MaterialVector material = matmanager.load(met);
                
                if ( material.size() == 0 )
                {
                    GreDebugPretty() << "Sorry , no Materials in given library." << std::endl;
                }
                else
                {
                    GreDebugPretty() << "Material library '" << met << "' ready." << std::endl;
                    wavefrontfile.mtlfile = met;
                }
            }
            
            else if ( met == "o" ) // Objects definition
            {
                line >> met ; // Object name.
                GreDebugPretty() << "Defining new sub-object name '" << met << "'." << std::endl;
                
                obj_t newobject;
                newobject.name = met;
                ind_basev = wavefrontfile.vertexs.size() > 0 ? (int) wavefrontfile.vertexs.size() - 1 : 0;
                ind_baset = wavefrontfile.textures.size() > 0 ? (int) wavefrontfile.textures.size() - 1 : 0;
                ind_basen = wavefrontfile.normals.size() > 0 ? (int) wavefrontfile.normals.size() - 1 : 0;
                
                wavefrontfile.objects.push_back(newobject);
            }
            
            else if ( met == "usemtl" ) // Material definition
            {
                line >> met ; // Material name.
                GreDebugPretty() << "Finding material name '" << wavefrontfile.mtlfile + "/" + met << "'." << std::endl;
                
                Gre::MaterialManager& matmanager = Gre::ResourceManager::Get().getMaterialManager();
                Gre::Material mat = matmanager.get(wavefrontfile.mtlfile + "/" + met);
                
                if ( mat.isInvalid() )
                {
                    GreDebugPretty() << "No material with name '" << wavefrontfile.mtlfile + "/" + met << "'." << std::endl;
                }
                
                else
                {
                    wavefrontfile.objects.at(wavefrontfile.objects.size()-1).mtlname = wavefrontfile.mtlfile + "/" + met;
                }
            }
            
            else if ( met == "v" ) // Vertex
            {
                float a,b,c;
                line >> a >> b >> c ;
                
                v_t v; v.a = a; v.b = b; v.c = c;
                wavefrontfile.vertexs.push_back(v);
            }
            
            else if ( met == "vt" ) // Texture coordinate
            {
                float a,b,c;
                line >> a >> b >> c ;
                
                vt_t vt; vt.a = a; vt.b = b; vt.c = c;
                wavefrontfile.textures.push_back(vt);
            }
            
            else if ( met == "vn" )
            {
                float a,b,c;
                line >> a >> b >> c ;
                
                vn_t vn; vn.a = a; vn.b = b; vn.c = c;
                wavefrontfile.normals.push_back(vn);
            }
            
            else if ( met == "vp" )
            {
                float a,b;
                line >> a >> b;
                
                vp_t vp; vp.a = a; vp.b = b;
                wavefrontfile.freeforms.push_back(vp);
            }
            
            else if ( met == "f" )
            {
                // For now , we only accurately create triangles.
                
                while ( line >> met )
                {
                    facepoint_t fp;
                    char* pEnd;
                    
                    // We have a string of form 'a/b/c' . So :
                    fp.v = (int) strtol(met.c_str(), &pEnd, 10) + ind_basev;
                    
                    if ( *pEnd == '\0' || *pEnd != '/' )
                    {
                        fp.vn = 0;
                        fp.vt = 0;
                    }
                    
                    else if ( *pEnd == '/' )
                    {
                        if ( * (pEnd+1) == '/' )
                        {
                            // Directly sets the vn.
                            fp.vt = 0;
                            fp.vn = (int) strtol(pEnd+2, &pEnd, 10) + ind_basen;
                        }
                        
                        else
                        {
                            // See for vt.
                            fp.vt = (int) strtol(pEnd+1, &pEnd, 10) + ind_baset;
                            
                            if ( *pEnd == '/' )
                            {
                                fp.vn = (int) strtol(pEnd+1, &pEnd, 10) + ind_basen;
                            }
                            else
                            {
                                fp.vn = 0;
                            }
                        }
                    }
                    
                    wavefrontfile.objects.at(wavefrontfile.objects.size()-1).faces.push_back(fp);
                }
            }
        }
    }
    
    if ( wavefrontfile.objects.size() )
    {
        GreDebugPretty() << "Objects found : " << wavefrontfile.objects.size() << std::endl;
        GreDebugPretty() << "Vertex found : " << wavefrontfile.vertexs.size() << std::endl;
        GreDebugPretty() << "Normals found : " << wavefrontfile.normals.size() << std::endl;
        GreDebugPretty() << "Textures found :" << wavefrontfile.textures.size() << std::endl;
        
        for ( auto& obj : wavefrontfile.objects )
        {
            GreDebugPretty() << "Object '" << obj.name << "' has " << obj.faces.size() << " facepoints." << std::endl;
        }
        
        GreDebugPretty() << "Creating SoftwareVertexBuffer." << std::endl;
        
        Gre::SoftwareVertexBufferHolder vertexbuf ( new Gre::SoftwareVertexBufferPrivate(filepath+"/softvbuf") );
        
        Gre::VertexDescriptor vdesc;
        if ( wavefrontfile.vertexs.size() )
            vdesc.addComponentUnique(Gre::VertexComponentType::Position);
        if ( wavefrontfile.textures.size() )
            vdesc.addComponentUnique(Gre::VertexComponentType::Texture);
        if ( wavefrontfile.normals.size() )
            vdesc.addComponentUnique(Gre::VertexComponentType::Normal);
        vertexbuf->setVertexDescriptor(vdesc);
        
        GreDebugPretty() << "Creating collection of SoftwareIndexBuffer." << std::endl;
        
        Gre::SoftwareIndexBufferHolder indexbuf ( new Gre::SoftwareIndexBufferPrivate(filepath+"/softibuf") );
        Gre::IndexBatchVector indexbatches;
        
        Gre::IndexDescriptor indexdesc;
        indexdesc.setType( Gre::IndexType::UnsignedInteger );
        
        int ind_basev = 0;
        int ind_baset = 0;
        int ind_basen = 0;
        
        int nextind_basev = 0;
        int nextind_baset = 0;
        int nextind_basen = 0;
        
        v_t nullv = { 0.0f , 0.0f , 0.0f };
        vt_t nullvt = { 0.0f , 0.0f };
        
        for ( auto & obj : wavefrontfile.objects )
        {
            Gre::IndexBatch batch;
            
            Gre::IndexDescriptor batchdesc;
            batchdesc.setType( Gre::IndexType::UnsignedInteger );
            
            Gre::Material mat = Gre::ResourceManager::Get().getMaterialManager().get(obj.mtlname);
            
            if ( mat.isInvalid() )
            {
                GreDebugPretty() << "Material name '" << obj.mtlname << "' not found." << std::endl;
            }
            else
            {
                batchdesc.setMaterial(mat);
            }
            
            batch.setDescriptor( batchdesc );
            
            std::vector<unsigned int> indices;
            
            for ( auto& fp : obj.faces )
            {
                // Creates a vertex to send to SoftwareVertexBuffer.
                
                if ( fp.v > 0 ) {
                    vertexbuf->addData( (const char*) & (wavefrontfile.vertexs.at(fp.v - ind_basev - 1)), sizeof (float) * 3 );
                    nextind_basev++;
                }
                else
                    vertexbuf->addData( (const char*) & nullv , sizeof (float) * 3 );
                
                if ( fp.vt > 0 ) {
                    vertexbuf->addData( (const char*) & (wavefrontfile.textures.at(fp.vt - ind_baset - 1)), sizeof (float) * 2 );
                    nextind_baset++;
                }
                else
                    vertexbuf->addData( (const char*) & nullvt , sizeof ( float ) * 2 );
                
                if ( fp.vn > 0 ) {
                    vertexbuf->addData( (const char*) & (wavefrontfile.normals.at(fp.vn - ind_basen - 1)), sizeof (float) * 3 );
                    nextind_basen++;
                }
                else
                    vertexbuf->addData( (const char*) & nullv , sizeof (float) * 3 );
                
                // Then , adds those data to the SoftwareIndexBuffer. Number of the Vertex is given using the VertexDescriptor.
                
                size_t numelements = vertexbuf->count();
                numelements = numelements - 1;
                indices.push_back( (unsigned int) numelements);
            }
            
            if ( indices.size() )
            {
                batch.addData( (const char*) & indices.at(0) , sizeof(unsigned int) * indices.size() );
                indexbuf->addIndexBatch(batch);
            }
            
            ind_basev = nextind_basev;
            ind_baset = nextind_baset;
            ind_basen = nextind_basen;
        }
        
        // Creating the Mesh.
        
        GreDebugPretty() << "Creating and populating the Mesh." << std::endl;
        
        Gre::MeshHolder mesh ( new Gre::MeshPrivate(filepath+"/mesh") );
        mesh->setSoftwareVertexBuffer(vertexbuf);
        mesh->setSoftwareIndexBuffer(indexbuf);
        
        return mesh;
    }
    
    GreDebugPretty() << "Nothing found in '" << filepath << "'." << std::endl;
    return Gre::MeshHolder ( nullptr );
}
