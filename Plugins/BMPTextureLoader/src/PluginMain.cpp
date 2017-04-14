//////////////////////////////////////////////////////////////////////
//
//  PluginMain.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 21/02/2017.
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

#include <bmpread.h>
#include <Texture.h>
#include <ResourceManager.h>

using namespace Gre ;

class BMPTextureLoader : public TextureFileLoader
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    BMPTextureLoader () { }
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~BMPTextureLoader () noexcept ( false ) { }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual ResourceLoader* clone() const
    {
        return new BMPTextureLoader () ;
    }
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const
    {
        return filepath.substr(filepath.find_last_of(".") + 1) == "bmp" ;
    }
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a file to the given SoftwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual SoftwarePixelBufferHolder load ( const std::string & filepath , const ResourceLoaderOptions & ops ) const
    {
        bmpread_t bmp ;
        
        if ( !bmpread(filepath.c_str(), BMPREAD_BYTE_ALIGN, &bmp) ) {
            GreDebug("[WARN] Can't load file '") << filepath << "'." << gendl ;
            return SoftwarePixelBufferHolder ( nullptr ) ;
        }
        
        SoftwarePixelBufferHolder buf = SoftwarePixelBufferHolder ( new SoftwarePixelBuffer() ) ;
        buf -> setSurface ( { 0, 0, bmp.width , bmp.height } ) ;
        buf -> setPixelType ( PixelType::UnsignedByte ) ;
        buf -> setPixelFormat ( PixelFormat::RGB ) ;
        buf -> setInternalPixelFormat ( InternalPixelFormat::RGB ) ;
        buf -> setDepth ( 0 ) ;
        buf -> setData ( bmp.rgb_data , bmp.width * bmp.height * 3 ) ;
        
        bmpread_free(&bmp) ;
        
        return buf ;
    }
};

PluginInfo info ;

extern "C" PluginInfo * GetPluginInfo ( void )
{
    info.name = "BMPTextureLoader Plugin" ;
    info.author = "Luk2010" ;
    info.version = GRE_PLUGIN_VERSION ;
    
    uuid_parse("1e11fdac-dff2-4f7b-9332-82d11406b049", info.uuid);
    
    return & info ;
}

extern "C" void StartPlugin ( void )
{
    ResourceManager::Get()->getTextureManager()->getFactory().registers("BMPTextureLoader",
                                                                       new BMPTextureLoader());
}

extern "C" void StopPlugin ( void )
{
    GreDebug ( "[INFO] BMP Texture Loader Plugin stopped." ) << Gre::gendl ;
}



