//////////////////////////////////////////////////////////////////////
//
//  TechniqueFileParser.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 02/04/2017.
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

#include "TechniqueFileParser.h"
#include "ResourceManager.h"

GreBeginNamespace

TechniqueFileParser::TechniqueFileParser ()
{

}

TechniqueFileParser::~TechniqueFileParser ()
{

}

TechniqueHolderList TechniqueFileParser::process ( const std::string & path , const std::string & source )
{
    //////////////////////////////////////////////////////////////////////
    // Check given values.

    if ( source.empty() )
    return TechniqueHolderList () ;

    //////////////////////////////////////////////////////////////////////
    // First step : tries to process the source into a node tree.

    internal::TechniqueFileRoot * root = parseFile ( source ) ;

    if ( !root )
    {
#ifdef GreIsDebugMode
        GreDebug ( "[WARN] Can't create Node Tree for file '" ) << path << "'." << gendl ;
#endif
        return TechniqueHolderList () ;
    }

    //////////////////////////////////////////////////////////////////////
    // Second step : tries to convert it into a context.

    internal::TechniqueFileContext * context = convertTree ( root ) ;

    if ( !context )
    {
#ifdef GreIsDebugMode
        GreDebug ( "[WARN] Can't convert Node Tree to File Context for file '" ) << path << "'." << gendl ;
#endif
        delete root ;
        return TechniqueHolderList () ;
    }

    //////////////////////////////////////////////////////////////////////
    // Third step : creates the returning holder list.

    TechniqueHolderList ret = createTechniques ( context ) ;

#ifdef GreIsDebugMode
    if ( ret.empty() )
    {
        GreDebug ( "[WARN] Can't create techniques from file '" ) << path << "'." << gendl ;
    }
#endif

    delete context ;
    delete root ;

    return ret ;
}

internal::TechniqueFileRoot* TechniqueFileParser::parseFile ( const std::string & source )
{
    //////////////////////////////////////////////////////////////////////
    // In order to parse the file , we loop through every characters untill
    // we find the '[' character , which is the definition beginner.

    uint32_t length = source.size() ;
    uint32_t pos = 0 ;

    pos = findNextDefinition ( source , pos ) ;

    if ( pos < length )
    {
        internal::TechniqueFileRoot* root = new internal::TechniqueFileRoot () ;

        parseBlockContent ( source , root ) ;
        return root ;
    }

    return nullptr ;
}

void TechniqueFileParser::parseBlockContent ( const std::string & source , internal::TechniqueFileNode* root )
{
    //////////////////////////////////////////////////////////////////////
    // In order to parse the file , we loop through every characters untill
    // we find the '[' character , which is the definition beginner.

    uint32_t length = source.size() ;
    uint32_t pos = 0 ;

    pos = findNextDefinition ( source , pos ) ;

    while ( pos < length )
    {
        //////////////////////////////////////////////////////////////////////
        // Detected new definition . We should create a new node , and Registers
        // it to the root node.

        internal::TechniqueFileNode* def = new internal::TechniqueFileNode () ;

        //////////////////////////////////////////////////////////////////////
        // Tries to see the definition end.

        uint32_t posend = findDefinitionEnd ( source , pos ) ;

        if ( pos >= length )
        {
#ifdef GreIsDebugMode
            GreDebug ( "[WARN] Definition End ']' not seen." ) << gendl ;
#endif
            delete def ;
            return ;
        }

        //////////////////////////////////////////////////////////////////////
        // Tries to compute the words in the definition.

        std::string tmp = source.substr ( pos + 1 , posend - pos ) ;
        std::vector < std::string > words = computeDefinitionWords ( tmp ) ;

        //////////////////////////////////////////////////////////////////////
        // Registers everything in defs.

        def -> definition.words = words ;
        def -> definition.brackbeg = pos ;
        def -> definition.brackend = posend ;

        //////////////////////////////////////////////////////////////////////
        // Now place our iterator to the end of the definition , ignores spaces
        // to see if next character is a block beginning.

        pos = posend ;

        char nextc ;
        uint32_t posblk = findNextCharacter ( source , pos , &nextc ) ;

        if ( nextc  == '{' )
        {
            //////////////////////////////////////////////////////////////////////
            // We have a block ! Tries to find the end of it.

            uint32_t posblkend = findBlockEnd ( source , posblk ) ;

            if ( posblkend >= length )
            {
#ifdef GreIsDebugMode
                GreDebug ( "[WARN] Can't find Block end." ) << gendl ;
#endif
                delete def ;
                return ;
            }

            //////////////////////////////////////////////////////////////////////
            // Now we have to register the new block and positions.

            internal::TechniqueFileBlock* blk = new internal::TechniqueFileBlock () ;
            blk -> blkbeg = posblk ;
            blk -> blkend = posblkend ;

            def -> definition.hasblock = true ;
            def -> block = blk ;

            //////////////////////////////////////////////////////////////////////
            // Now we may reiterate the exact same thing on the block content.

            std::string blkcont = source.substr ( posblk + 1 , posblkend - posblk ) ;
            parseBlockContent ( blkcont , def ) ;

            //////////////////////////////////////////////////////////////////////
            // Updates the position to the block's end.

            pos = posblkend ;
        }

        //////////////////////////////////////////////////////////////////////
        // Finds the next character to parse and registers the definition.

        root -> children.push_back ( def ) ;
        pos = findNextDefinition ( source , pos ) ;
    }
}

uint32_t TechniqueFileParser::findNextDefinition ( const std::string & source , uint32_t idx )
{
    uint32_t length = source.size() ;

    while ( idx < length )
    {
        if ( source.at(idx) == '[' )
        return idx ;

        idx ++ ;
    }

    return length ;
}

uint32_t TechniqueFileParser::findDefinitionEnd ( const std::string & source , uint32_t idx )
{
    uint32_t length = source.size() ;

    while ( idx < length )
    {
        if ( source.at(idx) == ']' )
        return idx ;

        idx ++ ;
    }

    return length ;
}

std::vector < std::string > TechniqueFileParser::computeDefinitionWords ( const std::string & source )
{
    uint32_t pos = 0 ;
    uint32_t length = source.size() ;
    std::vector < std::string > words ;

    //////////////////////////////////////////////////////////////////////
    // Iterates through the characters.

    while ( pos < length )
    {
        char c = source.at(pos) ;

        if ( std::isalpha(c) )
        {
            //////////////////////////////////////////////////////////////////////
            // We have a beginning word. Try to get it untill next space. A word is
            // an alphanumeric set of characters which are began by an alphabetic
            // character.

            std::string word ;

            while ( pos < length )
            {
                c = source.at(pos) ;

                if ( std::isspace(c) || c == ']' || c == '[' || c == '{' || c == '}' || std::iscntrl(c) )
                break ;

                word.push_back(c) ;
                pos ++ ;
            }

            words.push_back ( word ) ;
        }

        else if ( c == ']' )
        {
            //////////////////////////////////////////////////////////////////////
            // End of definition , exit the loop.
            break ;
        }

        else if ( c == '"' )
        {
            //////////////////////////////////////////////////////////////////////
            // String beginning. Try to get the next '"' and substr.

            uint32_t endpos = pos + 1 ;
            while ( endpos < length )
            {
                if ( source.at(endpos) == '"' )
                    break ;
                endpos ++ ;
            }

            if ( endpos < length )
            {
                std::string str = source.substr ( pos + 1 , endpos - pos - 1 ) ;
                words.push_back ( str ) ;
            }

            pos = endpos + 1 ;
        }

        else
        {
            //////////////////////////////////////////////////////////////////////
            // Space or any other characters , we can ignore it.
            pos ++ ;
        }
    }

    return words ;
}

uint32_t TechniqueFileParser::findNextCharacter ( const std::string & source , uint32_t pos , char* nextc )
{
    //////////////////////////////////////////////////////////////////////
    // Here , we just iterate over the source to find the next non-space
    // character.

    uint32_t length = source.size() ;
    uint32_t npos = pos + 1 ;

    while ( npos < length )
    {
        if ( !std::isspace(source.at(npos)) )
        {
            if ( nextc ) *nextc = source.at(npos) ;
            return npos ;
        }

        npos ++ ;
    }

    return length ;
}

uint32_t TechniqueFileParser::findBlockEnd ( const std::string & source , uint32_t pos )
{
    uint32_t length = source.size() ;
    uint32_t npos = pos + 1 ;
    uint32_t counter = 1 ;

    //////////////////////////////////////////////////////////////////////
    // Here , we iterates over the source. When a '{' is found , we push
    // the bracket holder. When a '}' is found , we pop this holder. When
    // the holder is at 0 , it means it is the right bracket.

    while ( npos < length )
    {
        if ( source.at(npos) == '{' ) {
            counter ++ ;
        }

        else if ( source.at(npos) == '}' ) {
            counter -- ;
            if ( counter == 0 )
            return npos ;
        }

        npos ++ ;
    }

    return length ;
}

internal::TechniqueFileContext* TechniqueFileParser::convertTree ( const internal::TechniqueFileRoot* root )
{
    //////////////////////////////////////////////////////////////////////
    // Check input.

    if ( root )
    {
        //////////////////////////////////////////////////////////////////////
        // We may declare a context only if the root node holds at least one
        // definition.

        if ( root->children.size() )
        {
            internal::TechniqueFileContext* context = new internal::TechniqueFileContext () ;
            context -> type = internal::TechniqueFileElementType::Context ;

            //////////////////////////////////////////////////////////////////////
            // Now , we may iterate over definitions. For now , in the global context ,
            // we only have two possibilities : Program , or Technique definition.

            for ( auto node : root->children )
            {
                if ( node->definition.words.empty() )
                continue ;

                std::string deftype = node->definition.words.at(0) ;

                if ( deftype == "Program" ) {
                    convertProgram ( context , node ) ;
                }

                else if ( deftype == "Technique" ) {
                    convertTechnique ( context , node ) ;
                }

                else if ( deftype == "Framebuffer" ) {
                    convertFramebuffer ( context , node ) ;
                }

#ifdef GreIsDebugMode
                else
                {
                    GreDebug ( "[WARN] Unidentified Node '" ) << deftype << "'." << gendl ;
                }
#endif
            }

            return context ;
        }
    }

    return nullptr ;
}

void TechniqueFileParser::convertProgram ( internal::TechniqueFileContext* context , const internal::TechniqueFileNode* node )
{
    //////////////////////////////////////////////////////////////////////
    // When we have to convert to a program , we know this one has a name ,
    // and a set of subprograms where words are the compiler and the file path.
    // The program name is given by the second definition word.

    std::string name = node -> definition.words.size() >= 2 ? node -> definition.words.at(1) : "" ;
    internal::TechniqueFileProgram program ;
    program.type = internal::TechniqueFileElementType::Program ;
    program.name = name ;

    //////////////////////////////////////////////////////////////////////
    // Iterates through the children of this node to get the shaders.

    for ( auto shadernode : node -> children )
    {
        if ( shadernode -> definition.words.empty() )
        continue ;

        internal::TechniqueFileShader shader ;
        shader.type = internal::TechniqueFileElementType::Shader ;

        //////////////////////////////////////////////////////////////////////
        // Here , we must handle the shader pathes. As they are definitions in the
        // shader node , we can just handle them by another loop.

        for ( auto pathnode : shadernode -> children )
        {
            if ( pathnode -> definition.words.size() < 2 )
            continue ;

            std::string comp = pathnode -> definition.words.at(0) ;
            std::string path = pathnode -> definition.words.at(1) ;
            shader.pathes[comp] = path ;
        }

        //////////////////////////////////////////////////////////////////////
        // Register the shadernode depending on its type declaration.

        if ( shadernode -> definition.words.at(0) == "Vertex" )
            program.shaders[ShaderType::Vertex] = shader ;
        else if ( shadernode -> definition.words.at(0) == "Fragment" )
            program.shaders[ShaderType::Fragment] = shader ;
    }

    //////////////////////////////////////////////////////////////////////
    // Register the program to the context.

    context -> programs [name] = program ;
}

void TechniqueFileParser::convertTechnique ( internal::TechniqueFileContext* context , const internal::TechniqueFileNode* node )
{
    //////////////////////////////////////////////////////////////////////
    // When we convert a technique , we must set aliases , attributes ,
    // lighting mode. Program is set using a name reference , but it is not
    // guaranteed the program has been declared when parsing the file.

    std::string name = node -> definition.words.size() >= 2 ? node -> definition.words.at(1) : "" ;
    internal::TechniqueFileTechnique technique ;
    technique.type = internal::TechniqueFileElementType::Technique ;
    technique.name = name ;
    technique.selfrendered = false ;

    //////////////////////////////////////////////////////////////////////
    // Iterates through the children nodes.

    for ( auto subnode : node -> children )
    {
        if ( subnode -> definition.words.empty() )
        continue ;

        //////////////////////////////////////////////////////////////////////
        // When we find an attribute , we have three words : Attribute , Alias , name.

        if (subnode -> definition.words.at(0) == "Attribute" &&
            subnode -> definition.words.size() >= 3)
        {
            std::string alias = subnode -> definition.words.at(1) ;
            std::string attname = subnode -> definition.words.at(2) ;
            technique.attributes[VertexAttribFromString(alias)] = attname ;
        }

        //////////////////////////////////////////////////////////////////////
        // Alias : Alias type , name .

        else if (subnode -> definition.words.at(0) == "Alias" &&
                 subnode -> definition.words.size() >= 3)
        {
            std::string alias = subnode -> definition.words.at(1) ;
            std::string alname = subnode -> definition.words.at(2) ;
            technique.aliases[TechniqueParamFromString(alias)] = alname ;
        }

        //////////////////////////////////////////////////////////////////////
        // Lighting Mode

        else if (subnode -> definition.words.at(0) == "LightingMode" &&
                 subnode -> definition.words.size() >= 2)
        {
            std::string mode = subnode -> definition.words.at(1) ;
            technique.lightingmode = TechniqueLightingModeFromString ( mode ) ;
        }

        //////////////////////////////////////////////////////////////////////
        // Program reference.

        else if (subnode -> definition.words.at(0) == "Program" &&
                 subnode -> definition.words.size() >= 2)
        {
            std::string prog = subnode -> definition.words.at(1) ;
            technique.program = prog ;
        }

        //////////////////////////////////////////////////////////////////////
        // Framebuffer reference.

        else if (subnode -> definition.words.at(0) == "Framebuffer" &&
                 subnode -> definition.words.size() >= 2)
        {
            std::string framebuffer = subnode -> definition.words.at(1) ;
            technique.framebuffer = framebuffer ;
        }

        //////////////////////////////////////////////////////////////////////
        // If the sequence 'Self-Rendered' is encountered , this means the
        // technique should be Self-Rendered.

        else if (subnode -> definition.words.at(0) == "Self-Rendered")
        technique.selfrendered = true ;

        //////////////////////////////////////////////////////////////////////
        // Unknown sequence.

        else
        GreDebug ( "[WARN] Unknown declaration '" ) << subnode -> definition.words.at(0) << gendl ;

    }

    //////////////////////////////////////////////////////////////////////
    // Registers the tecnique and quit.

    context -> techniques [name] = technique ;
}

void TechniqueFileParser::convertFramebuffer ( internal::TechniqueFileContext* context , const internal::TechniqueFileNode* node )
{
    //////////////////////////////////////////////////////////////////////
    // Framebuffer has a name in the definition.

    std::string name = node -> definition.words.size() >= 2 ? node -> definition.words.at(1) : "" ;
    internal::TechniqueFileFramebuffer framebuffer ;
    framebuffer.name = name ;
    framebuffer.readbuffer = RenderColorBuffer::None ;
    framebuffer.writebuffer = RenderColorBuffer::None ;
    framebuffer.type = internal::TechniqueFileElementType::Framebuffer ;

    //////////////////////////////////////////////////////////////////////
    // Iterates through children nodes. Normally a framebuffer take three
    // types of definition : Attachement , Drawbuffer , Readbuffer .

    for ( auto subnode : node -> children )
    {
        if ( subnode->definition.words.empty() )
        continue ;

        //////////////////////////////////////////////////////////////////////
        // Attachement definition : Attachement and Technique Parameter we have
        // to bind it to.

        if (subnode -> definition.words.at(0) == "Attachement" &&
            subnode -> definition.words.size() >= 3)
        {
            std::string attach = subnode -> definition.words.at(1) ;
            std::string param = subnode -> definition.words.at(2) ;
            framebuffer.attachements [TechniqueParamFromString(param)] = RenderFramebufferAttachementFromString(attach) ;
        }

        //////////////////////////////////////////////////////////////////////
        // Drawbuffer definition : Color buffer.

        else if (subnode -> definition.words.at(0) == "Drawbuffer" &&
                 subnode -> definition.words.size() >= 2)
        {
            std::string buffer = subnode -> definition.words.at(1) ;
            framebuffer.writebuffer = RenderColorBufferFromString(buffer) ;
        }

        //////////////////////////////////////////////////////////////////////
        // Readbuffer definition.

        else if (subnode -> definition.words.at(0) == "Readbuffer" &&
                 subnode -> definition.words.size() >= 2)
        {
            std::string buffer = subnode -> definition.words.at(1) ;
            framebuffer.readbuffer = RenderColorBufferFromString(buffer) ;
        }

        //////////////////////////////////////////////////////////////////////
        // Viewport definition.

        else if (subnode -> definition.words.at(0) == "Viewport" &&
                 subnode -> definition.words.size() >= 5)
        {
            framebuffer.viewport = Viewport ( (Surface) {
                static_cast<int>(strtol(subnode -> definition.words.at(1).c_str(), nullptr, 10)),
                static_cast<int>(strtol(subnode -> definition.words.at(2).c_str(), nullptr, 10)),
                static_cast<int>(strtol(subnode -> definition.words.at(3).c_str(), nullptr, 10)),
                static_cast<int>(strtol(subnode -> definition.words.at(4).c_str(), nullptr, 10))
            });
        }

#ifdef GreIsDebugMode
        else
        GreDebug ( "[WARN] Invalid framebuffer definition : " ) << subnode -> definition.words.at(0) << gendl ;
#endif
    }

    context -> framebuffers [name] = framebuffer ;
}

TechniqueHolderList TechniqueFileParser::createTechniques ( const internal::TechniqueFileContext* context )
{
    //////////////////////////////////////////////////////////////////////
    // Check input.

    if ( !context )
    return TechniqueHolderList () ;

    TechniqueHolderList techniques ;

    //////////////////////////////////////////////////////////////////////
    // Here we will first try to load every programs.

    for ( auto it : context -> programs )
    {
        //////////////////////////////////////////////////////////////////////
        // In order to create program from this structure , we must make the
        // program manager able to load a shader from multiple pathes. Also , makes
        // it able to load multiple shaders. To do this , we convert the shader
        // table here to be usable by the program manager.

        ShaderPathTableList shaders ;

        for ( auto it2 : it.second.shaders )
        shaders.add ( it2.first , it2.second.pathes ) ;

        //////////////////////////////////////////////////////////////////////
        // Tries to load the shaders by the program manager.

        HardwareShaderHolderList loaded = ResourceManager::Get() -> getHardwareProgramManager () -> loadShaderTables ( shaders ) ;
        if ( loaded.empty() ) continue ;

        //////////////////////////////////////////////////////////////////////
        // Tries to create a program and attach it those shaders.

        HardwareProgramHolder program = ResourceManager::Get() -> getHardwareProgramManager () -> loadProgram ( it.first ) ;
        if ( program.isInvalid() ) continue ;

        //////////////////////////////////////////////////////////////////////
        // Attaches the shaders to the program and finalize it.

        program -> attachShaders ( loaded ) ;
        program -> finalize () ;

#ifdef GreIsDebugMode
        //////////////////////////////////////////////////////////////////////
        // Check finalize status in debugging mode.

        if ( !program -> isFinalized() )
        GreDebug ( "[WARN] Program '" ) << program -> getName () << "' could not be finalized." << gendl ;
#endif
    }

    //////////////////////////////////////////////////////////////////////
    // Creates the framebuffer objects.

    for ( auto it : context -> framebuffers )
    {
        //////////////////////////////////////////////////////////////////////
        // Creates a new framebuffer.

        RenderFramebufferHolder framebuffer = ResourceManager::Get() -> getFramebufferManager() -> loadBlank ( it.first ) ;
        if ( framebuffer.isInvalid() ) continue ;

        //////////////////////////////////////////////////////////////////////
        // Draw / Read buffers.

        framebuffer -> setWriteBuffer ( it.second.writebuffer ) ;
        framebuffer -> setReadBuffer ( it.second.readbuffer ) ;
        framebuffer -> setViewport ( it.second.viewport ) ;

        //////////////////////////////////////////////////////////////////////
        // We can't do anything more for now. The technique has to bind the correct
        // textures when rendering. For example , it has to check the framebuffer
        // binding when setting an alias for the light shadow texture.
    }

    //////////////////////////////////////////////////////////////////////
    // Now , try to create the techniques objects.

    for ( auto it : context -> techniques )
    {
        //////////////////////////////////////////////////////////////////////
        // First , try to get a default blank technique.

        TechniqueHolder technique = ResourceManager::Get() -> getTechniqueManager () -> loadBlank ( it.first ) ;
        if ( technique.isInvalid() ) continue ;

        //////////////////////////////////////////////////////////////////////
        // Lets make the technique usable.

        HardwareProgramHolder program = ResourceManager::Get() -> getHardwareProgramManager () -> getProgram ( it.second.program ) ;
        if ( !program.isInvalid() ) technique -> setHardwareProgram ( program ) ;

        technique -> setLightingMode ( it.second.lightingmode ) ;

        //////////////////////////////////////////////////////////////////////
        // Self-Rendered flag.

        technique -> setSelfRendered ( it.second.selfrendered ) ;

        //////////////////////////////////////////////////////////////////////
        // Computes aliases.

        for ( auto it2 : it.second.aliases )
        technique -> setAlias ( it2.first , it2.second ) ;

        //////////////////////////////////////////////////////////////////////
        // Computes attributes.

        for ( auto it2 : it.second.attributes )
        technique -> setAttribName ( it2.first , it2.second ) ;

        //////////////////////////////////////////////////////////////////////
        // Register the framebuffer to the technique.

        RenderFramebufferHolder framebuffer = ResourceManager::Get() -> getFramebufferManager() -> get ( it.second.framebuffer ) ;
        if ( !framebuffer.isInvalid() ) technique -> setFramebuffer(framebuffer) ;

        //////////////////////////////////////////////////////////////////////
        // Registers the framebuffer attachements.

        auto framebufit = context -> framebuffers.find(it.second.framebuffer) ;

        if ( framebufit != context -> framebuffers.end() )
        technique -> setFramebufferAttachements ( framebufit->second.attachements ) ;

        //////////////////////////////////////////////////////////////////////
        // Registers the technique.

        techniques.add ( technique ) ;
    }

    return techniques ;
}

GreEndNamespace
