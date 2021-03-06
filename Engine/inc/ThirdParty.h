//
//  ThirdParty.h
//  GRE
//
//  Created by Jacques Tronconi on 21/01/2016.
//
//

#ifndef GRE_ThirdParty_h
#define GRE_ThirdParty_h

//http://stackoverflow.com/questions/23230003/something-between-func-and-pretty-function/29856690?noredirect=1#comment47839355_29856690
#include <cstring>
#include <string>
#include <algorithm>
#include <sstream>

template<size_t FL, size_t PFL>
const char* computeMethodName(const char (&function)[FL], const char (&prettyFunction)[PFL]) {
    using reverse_ptr = std::reverse_iterator<const char*>;
    /*thread_local*/ static char result[PFL];
    const char* locFuncName = std::search(prettyFunction,prettyFunction+PFL-1,function,function+FL-1);
    const char* locClassName = std::find(reverse_ptr(locFuncName), reverse_ptr(prettyFunction), ' ').base();
    const char* endFuncName = std::find(locFuncName,prettyFunction+PFL-1,'(');
    std::copy(locClassName, endFuncName, result);
    return result;
}
#define __COMPACT_PRETTY_FUNCTION__ computeMethodName(__FUNCTION__,__PRETTY_FUNCTION__)

// For those two functions, 'extern "C"' is needed only for the second because it uses the first, so the first
// is compiled. Making the second 'extern "C"' able it to be bundled in the library even if the Compiler tells
// it's unused.

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

extern "C" std::vector<std::string> split(const std::string &s, char delim);

#endif
