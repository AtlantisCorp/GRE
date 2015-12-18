//
//  OpenGlIncludes.cpp
//  GRE
//
//  Created by Jacques Tronconi on 11/12/2015.
//
//

#include "OpenGlIncludes.h"

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

GLenum OpenGlUtils::PrimitiveTypeToGl (PrimitiveType ptype)
{
    switch (ptype)
    {
        case PrimitiveType::Points:
            return GL_POINTS;
        case PrimitiveType::Lines:
            return GL_LINES;
        case PrimitiveType::LinesAdjacency:
            return GL_LINES_ADJACENCY_EXT;
        case PrimitiveType::LinesLoop:
            return GL_LINE_LOOP;
        case PrimitiveType::LinesStrip:
            return GL_LINE_STRIP;
        case PrimitiveType::LinesStripAdjacency:
            return GL_LINE_STRIP_ADJACENCY_EXT;
        case PrimitiveType::Patches:
            return GL_POINTS;
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;
        case PrimitiveType::TrianglesAdjacency:
            return GL_TRIANGLES_ADJACENCY_EXT;
        case PrimitiveType::TrianglesFan:
            return GL_TRIANGLE_FAN;
        case PrimitiveType::TrianglesStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::TrianglesStripAdjacency:
            return GL_TRIANGLE_STRIP_ADJACENCY_EXT;
        default:
            return GL_POINTS;
    }
}

GLenum OpenGlUtils::StorageTypeToGl (StorageType stype)
{
    switch (stype)
    {
        case StorageType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case StorageType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case StorageType::UnsignedInt:
            return GL_UNSIGNED_INT;
        default:
            return GL_UNSIGNED_INT;
    }
}

#ifdef GRE_OSX

void * MyNSGLGetProcAddress (const char *name)
{
    NSSymbol symbol;
    char *symbolName;
    symbolName = (char*) malloc (strlen (name) + 2); // 1
    strcpy(symbolName + 1, name); // 2
    symbolName[0] = '_'; // 3
    symbol = NULL;
    if (NSIsSymbolNameDefined (symbolName)) // 4
        symbol = NSLookupAndBindSymbol (symbolName);
    free (symbolName); // 5
    return symbol ? NSAddressOfSymbol (symbol) : NULL; // 6
}

#endif
