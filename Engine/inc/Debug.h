//////////////////////////////////////////////////////////////////////
//
//  Debug.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 10/02/2017.
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

#ifndef GRE_DEBUG_H
#define GRE_DEBUG_H

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
// When debug mode is enabed (GreIsDebugMode is defined) , the macro
// 'GreDebug()' can be used to initiate the debug output , and 'gendl'
// should always be used when finishing output.

#   ifdef GreIsDebugMode

extern std::recursive_mutex __globcoutmutex ;

template <class _CharT, class _Traits>
inline std::basic_ostream<_CharT, _Traits>& gendl(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os.put(__os.widen('\n'));
    __os.flush();
    __globcoutmutex.unlock();
    return __os;
}

/// @brief Debug using an intro (should use __COMPACT_PRETTY_FUNCTION__ macro) and the body message.
DLL_PUBLIC std::ostream& GreDebugBase(const std::string& func);
#define GreDebugPretty() Gre::GreDebugBase( __COMPACT_PRETTY_FUNCTION__ )
#define GreDebug(msg) GreDebugPretty() << msg

#define _GreDebugNotImplemented( message , arg ) \
    static std::string __message##__arg = std::string( message ); \
    static bool __already_passed##__arg = false; \
    if( __already_passed##__arg ) { GreDebugPretty() << message << Gre::gendl; __already_passed##__arg = true; }

#define GreDebugNotImplemented( message ) _GreDebugNotImplemented( message , __LINE__ )
#define GreDebugFunctionNotImplemented() GreDebugNotImplemented( std::string ( __COMPACT_PRETTY_FUNCTION__ ) + " : Not implemented !" )

#define GreDebugThrow( exception ) throw exception

//////////////////////////////////////////////////////////////////////
// When debug mode is disabled , unuse every debug command. We also make
// a dummy 'DebugNoOutput' object to be able to disable every debug commands.

#   else

class DebugNoOutput
{
public:

    template < typename Class >
    const DebugNoOutput & operator << ( const Class & ) const { return *this ; }
};

extern DebugNoOutput __dbgobject ;
inline const DebugNoOutput & gendl ( const DebugNoOutput & dbg ) { return dbg ; }

#define GreDebugPretty ()                           __dbgobject
#define GreDebug ( message )                        __dbgobject
#define _GreDebugNotImplemented ( message , arg )   __dbgobject
#define GreDebugNotImplemented ( message )          __dbgobject
#define GreDebugFunctionNotImplemented ( message )  __dbgobject
#define GreDebugThrow ( exception )                 __dbgobject

#   endif

GreEndNamespace

#endif // GRE_DEBUG_H
