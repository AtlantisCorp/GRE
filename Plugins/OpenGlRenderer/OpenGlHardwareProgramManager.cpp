//
//  OpenGlHardwareProgramManager.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/01/2016.
//
//

#include "OpenGlHardwareProgramManager.h"
#include "OpenGlHardwareProgram.h"

GreBeginNamespace

OpenGlHardwareProgramManager::OpenGlHardwareProgramManager(const std::string& name)
: HardwareProgramManagerPrivate(name)
{
    
}

OpenGlHardwareProgramManager::~OpenGlHardwareProgramManager()
{
    
}

std::string GetShaderFileText(const std::string& filepath)
{
    std::ifstream filestream(filepath, std::ios::in);
    if(!filestream) {
        return std::string();
    }
    
    filestream.seekg(0, filestream.end);
    int sz = (int) filestream.tellg();
    filestream.seekg(0, filestream.beg);
    
    if(sz > 0)
    {
        char buf [sz];
        memset(buf, 0, sz+1);
        
        filestream.read(buf, sz);
        filestream.close();
        
        return std::string(buf, sz);
    }
    
    filestream.close();
    return std::string();
}

HardwareShader OpenGlHardwareProgramManager::loadShader(const ShaderType& stype, const std::string& name, const std::string& filepath)
{
    // We must verify that the Resource has not already been loaded !
    ResourceUser tmpShader = ResourceManager::Get().findResourceByName(name);
    if(!tmpShader.expired())
    {
        // The shader has already been loaded. We can directly return
        // it to the user.
        return HardwareShader(tmpShader);
    }
    
    std::string shaderText = GetShaderFileText(filepath);
    if(shaderText.empty())
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "No File '" << filepath << "'." << std::endl;
#endif
        return HardwareShader::Null;
    }

    HardwareShaderLoader* hdwShaderLoader = ResourceManager::Get().getHardwareShaderLoaderFactory().get("OpenGlHdwShaderLoader");
    HardwareShader shaderRet = HardwareShader(ResourceManager::Get().loadResourceWith(hdwShaderLoader, Resource::Type::HdwShader, name, shaderText, stype));
    return shaderRet;
}

HardwareProgram OpenGlHardwareProgramManager::createHardwareProgram(const std::string& name, const HardwareShader &vertexShader, const HardwareShader &fragmentShader)
{
    if(vertexShader.expired() || fragmentShader.expired())
    {
#ifdef GreIsDebugMode
        GreDebugPretty() << "Vertex or Fragment shader null !" << std::endl;
#endif
        return HardwareProgram::Null;
    }
    
    std::shared_ptr<HardwareProgramPrivate> hdwProgramPriv = std::make_shared<OpenGlHardwareProgram>(name, vertexShader, fragmentShader);
    if(hdwProgramPriv)
    {
        _mPrograms[name] = hdwProgramPriv;
        return HardwareProgram(ResourceUser(hdwProgramPriv));
    }
    
#ifdef GreIsDebugMode
    GreDebugPretty() << "Can't allocate new HardwareProgram '" << name << "'." << std::endl;
#endif
    return HardwareProgram::Null;
}

// ---------------------------------------------------------------------------------------------------

OpenGlHardwareProgramManagerLoader::OpenGlHardwareProgramManagerLoader()
: HardwareProgramManagerLoader()
{
    
}

OpenGlHardwareProgramManagerLoader::~OpenGlHardwareProgramManagerLoader()
{
    
}

ResourceLoader* OpenGlHardwareProgramManagerLoader::clone() const
{
    return new OpenGlHardwareProgramManagerLoader();
}

Resource* OpenGlHardwareProgramManagerLoader::load(Resource::Type type, const std::string &name) const
{
    return new OpenGlHardwareProgramManager(name);
}

GreEndNamespace
