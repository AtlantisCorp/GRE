//////////////////////////////////////////////////////////////////////
//
//  ObjMeshLoader.h
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

#ifndef ObjMeshLoader_h
#define ObjMeshLoader_h

#include "Mesh.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Loads OBJ file meshes.
/// This loader is not a plugin, in order to provide basic suport for
/// the OBJ mesh file format.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ObjMeshLoader : public MeshLoader
{
public:
    
    POOLED ( Pools::Loader )
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ObjMeshLoader () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ObjMeshLoader () noexcept ( false ) ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Loads one or more Mesh from a file.
    /// As some files can store more than one mesh (OBJ file for example),
    /// a MeshLoader should be able to return more than one resource.
    //////////////////////////////////////////////////////////////////////
    virtual MeshHolderList load ( const std::string& name , const std::string& filepath ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const ;
};

GreEndNamespace

#endif /* ObjMeshLoader_h */
