//
//  WavefrontStructs.h
//  GRE
//
//  Created by Jacques Tronconi on 27/09/2016.
//
//

#ifndef WavefrontStructs_h
#define WavefrontStructs_h

#include "Pools.h"

// see 'http://www.hodge.net.au/sam/blog/wp-content/uploads/obj_format.txt' for format specifications.

struct v_t
{
    float a;
    float b;
    float c;
};

struct vt_t
{
    float a;
    float b;
    float c;
};

struct vn_t
{
    float a;
    float b;
    float c;
};

struct vp_t
{
    float a;
    float b;
};

struct facepoint_t
{
    int v;
    int vt;
    int vn;
};

struct obj_t
{
    std::vector< facepoint_t > faces;
    std::string mtlname;
    std::string name;
};

struct file_t
{
    std::vector< obj_t > objects;
    std::vector< v_t > vertexs;
    std::vector< vt_t > textures;
    std::vector< vn_t > normals;
    std::vector< vp_t > freeforms;
    std::string mtlfile;
};

#endif /* WavefrontStructs_h */
