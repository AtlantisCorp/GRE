//////////////////////////////////////////////////////////////////////
//
//  TechniqueFileParser.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 01/04/2017.
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

#ifndef GRE_TECHNIQUEFILEPARSER_H
#define GRE_TECHNIQUEFILEPARSER_H

#include "Technique.h"

GreBeginNamespace

/// @brief Defines the currently used parser version.
#define GRE_TECHNIQUE_PARSER_VERSION 1

namespace internal
{
    //////////////////////////////////////////////////////////////////////
    /// @brief Defines an element.
    enum class TechniqueFileElementType : int
    {
        Shader , Program , Technique , Context , Framebuffer , Texture
    };

    struct TechniqueFileElement
    {
        TechniqueFileElementType type ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Technique File's Framebuffer definition.
    struct TechniqueFileFramebuffer : TechniqueFileElement
    {
        std::map < RenderFramebufferAttachement , std::string > staticattachments ;
        std::map < TechniqueParam , RenderFramebufferAttachement > attachements ;
        RenderColorBuffer writebuffer ;
        RenderColorBuffer readbuffer ;
        std::string name ;
        Viewport viewport ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Technique File's Shader definition.
    struct TechniqueFileShader : TechniqueFileElement
    {
        std::map < std::string , std::string > pathes ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Technique File's Program definition.
    struct TechniqueFileProgram : TechniqueFileElement
    {
        std::map < ShaderType , TechniqueFileShader > shaders ;
        std::string name ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Technique File's Technique definition.
    struct TechniqueFileTechnique : TechniqueFileElement
    {
        std::map < VertexAttribAlias , std::string > attributes ;
        std::map < TechniqueParam , std::string > aliases ;

        std::string name ;
        std::string program ;
        TechniqueLightingMode lightingmode ;
        std::string framebuffer ;
        bool selfrendered ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Technique File's Texture definition.
    struct TechniqueFileTexture : TechniqueFileElement
    {
        TextureType ttype ;
        std::string name ;
        std::string file ;

        PixelFormat pf ;
        InternalPixelFormat ipf ;
        PixelType pt ;

        std::pair < int , int > size ;
        int depth ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Technique File's Context.
    struct TechniqueFileContext : TechniqueFileElement
    {
        std::map < std::string , TechniqueFileProgram > programs ;
        std::map < std::string , TechniqueFileFramebuffer > framebuffers ;
        std::map < std::string , TechniqueFileTechnique > techniques ;
        std::map < std::string , TechniqueFileTexture > textures ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Defines a structure definition.
    struct TechniqueFileDefinition
    {
        int brackbeg ; int brackend ;
        std::vector < std::string > words ;
        bool hasblock ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Defines a structure block.
    struct TechniqueFileBlock
    {
        int blkbeg ; int blkend ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Defines a node in the file.
    struct TechniqueFileNode
    {
        TechniqueFileDefinition definition ;
        TechniqueFileBlock* block ;
        std::vector < TechniqueFileNode* > children ;
    };

    //////////////////////////////////////////////////////////////////////
    /// @brief Defines a root node in the file.
    struct TechniqueFileRoot : TechniqueFileNode
    {

    };

    /*

        A basic tree struture for the Technique's file will be :

        - Root
            - Node
                - Def
                    - brackbeg = 5
                    - brackend = 15
                    - words ( Program , Example )
                    - hasblock = true
                - Block
                    - blkbeg = 16
                    - blkend = 30
                - children
                    - Node
                        - Def
                            - [...]
                            - words ( "GLSL" , "..." )
                    - Node
                        - Def
                            - [..]
                            - words ( "GLSL" , "..." )
            - Node
                - Def
                    - [...]
                    - words ( Technique , Example )
                    - hasblock = true
                - Block
                    - [...]
                - children
                    - Node
                        - Def
                            - words ( Attribute , BLAH , BLAH )
                    - Node
                        - Def
                            - words ( Alias , BLAH )
    */

    // With a tree like this one above , we can create the file context structure
    // easily , using the elements.
}

//////////////////////////////////////////////////////////////////////
/// @brief Parses the given file in order to create technique's object
/// and program's objects , with defined attributes , parameters (aliases)
/// and other configurations.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC TechniqueFileParser
{
public:

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    TechniqueFileParser () ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~TechniqueFileParser () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Should process the given file into two steps : creates the
    /// node's tree , and resolve it into a context tree. Next , it should
    /// create the technique's using the context tree informations.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolderList process ( const std::string & path , const std::string & source ) ;

protected:

    //////////////////////////////////////////////////////////////////////
    /// @brief Parse the given source file to create a node tree.
    //////////////////////////////////////////////////////////////////////
    virtual internal::TechniqueFileRoot * parseFile ( const std::string & source ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Parse the given block content for definitions and blocks.
    //////////////////////////////////////////////////////////////////////
    virtual void parseBlockContent ( const std::string & source , internal::TechniqueFileNode* node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the next '[' character from position , including this
    /// position.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findNextDefinition ( const std::string & source , uint32_t idx ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the next ']' character from position , including this
    /// position.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findDefinitionEnd ( const std::string & source , uint32_t idx ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Computes the words used in a definition.
    /// Those words are : letters ( ex : abcde ) , or strings ( ex : "abcd" ) .
    //////////////////////////////////////////////////////////////////////
    virtual std::vector < std::string > computeDefinitionWords ( const std::string & source ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds any character next position that is not a space.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findNextCharacter ( const std::string & source , uint32_t pos , char * nextc ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Finds the block's end '}'. Also counts opened bracket since
    /// given position.
    //////////////////////////////////////////////////////////////////////
    virtual uint32_t findBlockEnd ( const std::string & source , uint32_t pos ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Converts the given tree into a context structure.
    //////////////////////////////////////////////////////////////////////
    virtual internal::TechniqueFileContext* convertTree ( const internal::TechniqueFileRoot* root ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Interprets the given node as a program and convert it to the
    /// context.
    //////////////////////////////////////////////////////////////////////
    virtual void convertProgram ( internal::TechniqueFileContext* context , const internal::TechniqueFileNode* node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Interprets the given node as a technique and convert it to the
    /// context.
    //////////////////////////////////////////////////////////////////////
    virtual void convertTechnique ( internal::TechniqueFileContext* context , const internal::TechniqueFileNode* node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Interprets the given node as a Framebuffer.
    //////////////////////////////////////////////////////////////////////
    virtual void convertFramebuffer ( internal::TechniqueFileContext* context , const internal::TechniqueFileNode* node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Interprets the given node as a Texture.
    //////////////////////////////////////////////////////////////////////
    virtual void convertTexture ( internal::TechniqueFileContext* context , const internal::TechniqueFileNode* node ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates techniques using the context object.
    //////////////////////////////////////////////////////////////////////
    virtual TechniqueHolderList createTechniques ( const internal::TechniqueFileContext* context ) ;
};

GreEndNamespace

#endif // GRE_TECHNIQUEFILEPARSER_H
