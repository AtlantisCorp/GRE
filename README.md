# GRE
Gangtella Resource'd Engine

## Principes

GRE is a generic Engine. Its model is simple :
- Every base object is a Resource.
- Objects can be shared between Objects Users and Objects Holders.
- Objects Holders destroys the Object when there are no more Holder.
- Users can create Holders using a 'lock' system.
- Objects can treat events.

And that is the principe of GRE.

## Goals, what we want to achieve

A simple way to make, and product multiplatform product, only by loading what we need through Plugins. Plugins can do everythings like programs, but their first goal is to register new Loaders to the ResourceManager factories, for the user to be able to use them at his convenience.

For example, if you develop a new Window System, a revolutionnary Window System, you can create easily a new Plugin for GRE. And any user will juste have to load the Plugin to use your Window System, without changing anything of its own Window-related code.

## Notes, dependancies, other things

The Engine is completly written in C++11. You, obviously, need a compiler that can compile C++11 (We recommend at least GCC, Clang/llvm, MSVC++).

### Dependancies 

- C++11 Compiler,
- std library,
- Dynamic Library loader.

### Directory structure 

- Engine   : GRE sources files .
- Plugins  : Plugins sources files .
- Example  : Examples sources files .
- Projects : Deprecated projects files for some IDE. This directory should not be used as CMakeFiles are now present.
- Sdk      : Sdk files . 
    - Resources : Resources files , like shaders , models , ...
    - Plugins : Every Plugins should be in this directory.

Examples and Engine library goes to Sdk/ directory. Plugins builds to Sdk/Plugins directory. CMakeFiles are used to generate the build system. 
