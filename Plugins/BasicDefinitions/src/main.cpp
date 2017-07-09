//////////////////////////////////////////////////////////////////////
//
//  main.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 25/06/2017.
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

#include <ResourceManager.h>
using namespace Gre ;

#include "TechniqueWorker.h"
#include "ProgramWorker.h"
#include "MatVarWorker.h"
#include "FramebufferWorker.h"
#include "ProjectionWorker.h"
#include "TextureWorker.h"

PluginInfo info =
{
    "BasicDefinitions" ,
    "Integrates basic definitions workers for technique's files." ,
    "Luk2010" , GRE_PLUGIN_VERSION
} ;

extern "C"
{
    PluginInfo* GetPluginInfo ( void )
    {
        uuid_parse( "62c69c7a-d0d6-41c2-847b-1f2c43568cf0" , info.uuid );
        return &info ;
    }

    void StartPlugin ( void )
    {
        auto manager = ResourceManager::Get() -> getDefinitionWorkerManager() ;

        if ( !manager.isInvalid() )
        {
            manager -> add ( new TechniqueWorker( "basics.technique" ) );
            manager -> add ( new ProgramWorker( "basics.program" ) );
            manager -> add ( new MatVarWorker( "basics.matvar" ) );
            manager -> add ( new FramebufferWorker( "basics.framebuffer" ) );
            manager -> add ( new TextureWorker( "basics.texture" ) );
            // manager -> add ( new MeshWorker( "basics.mesh" ) );
            manager -> add ( new ProjectionWorker( "basics.projection" ) );
        }
    }

    void StopPlugin ( void )
    {

    }
}
