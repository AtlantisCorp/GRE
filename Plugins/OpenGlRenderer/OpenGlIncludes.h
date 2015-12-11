//
//  OpenGlIncludes.h
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#ifndef GRE_OpenGlIncludes_h
#define GRE_OpenGlIncludes_h

#include "ResourceManager.h"
#include <OpenGL/gl.h>
#include <OpenGl/gl3.h>
#include <OpenGL/glu.h>
#include <sstream>

using namespace Gre;

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string>  split(const std::string &s, char delim);

class OpenGlUtils
{
public:
    
    static GLenum PrimitiveTypeToGl (PrimitiveType ptype);
    static GLenum StorageTypeToGl (StorageType stype);
};

#endif
