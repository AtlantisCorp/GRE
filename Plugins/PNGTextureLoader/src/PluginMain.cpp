//////////////////////////////////////////////////////////////////////
//
//  PluginMain.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
//  Created on 01/03/2017.
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

#include <png.h>
#include <ResourceManager.h>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class PNGTextureLoader : public Gre::TextureFileLoader
{
public:
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    PNGTextureLoader () ;
    
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~PNGTextureLoader () noexcept ( false ) ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Returns a clone of this object.
    /// Typically, this function is implemented as 'return new MyLoaderClass();',
    /// but you are free to do whatever you want.
    ////////////////////////////////////////////////////////////////////////
    virtual Gre::ResourceLoader* clone() const ;
    
    //////////////////////////////////////////////////////////////////////
    /// @brief Returns true if the file given is loadable by this loader.
    //////////////////////////////////////////////////////////////////////
    virtual bool isLoadable( const std::string& filepath ) const ;
    
    ////////////////////////////////////////////////////////////////////////
    /// @brief Loads a file to the given SoftwarePixelBuffer.
    //////////////////////////////////////////////////////////////////////
    virtual Gre::SoftwarePixelBufferHolder load ( const std::string & filepath , const Gre::ResourceLoaderOptions & ops ) const ;
};

PNGTextureLoader::PNGTextureLoader ()
{
    
}

PNGTextureLoader::~PNGTextureLoader() noexcept ( false )
{
    
}

Gre::ResourceLoader* PNGTextureLoader::clone() const
{
    return new PNGTextureLoader () ;
}

bool PNGTextureLoader::isLoadable(const std::string &filepath) const
{
    return filepath.substr(filepath.find_last_of(".") + 1) == "png" ;
}

Gre::SoftwarePixelBufferHolder PNGTextureLoader::load(const std::string &filepath , const Gre::ResourceLoaderOptions & ops) const
{
    FILE* fp ;
    png_byte magic[8] ;
    
    fp = fopen(filepath.c_str(), "rb") ;
    if ( !fp )
    {
        GreDebug("[WARN] Can't open file '") << filepath << "'." << Gre::gendl ;
        return Gre::SoftwarePixelBufferHolder ( nullptr ) ;
    }
    
    fread(magic, 1, sizeof(magic), fp) ;
    
    if ( !png_check_sig(magic, sizeof(magic)) )
    {
        GreDebug("[WARN] Invalid PNG Magic check in file '") << filepath << "'." << Gre::gendl ;
        fclose(fp) ;
        return Gre::SoftwarePixelBufferHolder ( nullptr ) ;
    }
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if ( !png_ptr )
    {
        GreDebug("[WARN] Invalid png_ptr structure.") << Gre::gendl ;
        fclose(fp) ;
        return Gre::SoftwarePixelBufferHolder ( nullptr ) ;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr) ;
    if ( !info_ptr )
    {
        GreDebug("[WARN] Invalid info_ptr structure.") << Gre::gendl ;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return Gre::SoftwarePixelBufferHolder ( nullptr ) ;
    }
    
    if ( setjmp(png_jmpbuf(png_ptr)) )
    {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return Gre::SoftwarePixelBufferHolder ( nullptr ) ;
    }
    
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sizeof(magic));
    
    png_read_info(png_ptr, info_ptr);
    
    int bit_depth, colortype ;
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    colortype = png_get_color_type(png_ptr, info_ptr);
    
    if ( colortype == PNG_COLOR_TYPE_PALETTE )
        png_set_palette_to_rgb(png_ptr) ;
    if ( colortype == PNG_COLOR_TYPE_GRAY && bit_depth < 8 )
        png_set_expand_gray_1_2_4_to_8(png_ptr) ;
    if ( png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) )
        png_set_tRNS_to_alpha(png_ptr) ;
    if ( bit_depth == 16 )
        png_set_strip_16(png_ptr) ;
    if ( bit_depth < 8 )
        png_set_packing(png_ptr) ;
    
    png_read_update_info(png_ptr, info_ptr) ;
    
    Gre::Surface texsurface ;
    png_get_IHDR(png_ptr, info_ptr,
                 (png_uint_32*) &texsurface.width,
                 (png_uint_32*) &texsurface.height,
                 &bit_depth, &colortype,
                 NULL, NULL, NULL);
    
    Gre::SoftwarePixelBufferHolder buffer = Gre::SoftwarePixelBufferHolder ( new Gre::SoftwarePixelBuffer() ) ;
    int components = 0 ;
    
    switch ( colortype )
    {
        case PNG_COLOR_TYPE_GRAY:
            buffer -> setPixelFormat(Gre::PixelFormat::Luminance) ;
            buffer -> setInternalPixelFormat(Gre::InternalPixelFormat::Red) ;
            components = 1 ;
            break ;
            
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            buffer -> setPixelFormat(Gre::PixelFormat::LuminanceAlpha) ;
            buffer -> setInternalPixelFormat(Gre::InternalPixelFormat::RG) ;
            components = 2 ;
            break ;
            
        case PNG_COLOR_TYPE_RGB:
            buffer -> setPixelFormat(Gre::PixelFormat::RGB) ;
            buffer -> setInternalPixelFormat(Gre::InternalPixelFormat::RGB) ;
            components = 3 ;
            break ;
            
        case PNG_COLOR_TYPE_RGB_ALPHA:
            buffer -> setPixelFormat(Gre::PixelFormat::RGBA) ;
            buffer -> setInternalPixelFormat(Gre::InternalPixelFormat::RGBA) ;
            components = 4 ;
            break ;
            
        default:
            GreDebug("[WARN] Can't deduce final pixel format from file '") << filepath << "'." << Gre::gendl ;
            fclose(fp);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return Gre::SoftwarePixelBufferHolder ( nullptr ) ;
    }
    
    buffer -> setData(nullptr, texsurface.width * texsurface.height * components) ;
    buffer -> setSurface(texsurface);
    buffer -> setPixelType(Gre::PixelType::UnsignedByte);
    
    unsigned char* bytes = (unsigned char*) buffer -> getData() ;
    png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * texsurface.height) ;
    
    for ( unsigned int i = 0 ; i < texsurface.height ; ++i ) {
        row_pointers[i] = (png_bytep) (bytes + (texsurface.height - (i+1)) * texsurface.width * components) ;
    }
    
    png_read_image(png_ptr, row_pointers);
    return buffer ;
}

Gre::PluginInfo info ;

extern "C" Gre::PluginInfo * GetPluginInfo ( void )
{
    info.name = "PNGTextureLoader Plugin" ;
    info.author = "Luk2010" ;
    info.version = GRE_PLUGIN_VERSION ;
    
    uuid_parse("50b322ec-d35f-4178-96e4-960afff2cf5c", info.uuid);
    
    return & info ;
}

extern "C" void StartPlugin ( void )
{
    Gre::ResourceManager::Get()->getTextureManager()->getFactory()
    .registers("PNGTextureFileLoader", new PNGTextureLoader()) ;
}

extern "C" void StopPlugin ( void )
{
    GreDebug ( "[INFO] PNG Texture Loader Plugin stopped." ) << Gre::gendl ;
}




