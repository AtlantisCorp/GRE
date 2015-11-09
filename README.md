# GRE
Gangtella Resourced Engine

## Principes

GRE is a generic Engine. Its model is simple :
- Every shared object that can be is a Resource.
- A Resource User is a Proxy to that Resource.
- The Resource is owned by the Resource Manager.
- The Resource can be loaded by an appropriate Loader.

And that is the principe of GRE.

## Goals, what we want to achieve

A simple way to make, and product multiplatform product, only by loading what we need through Plugins. Plugins can do everythings like programs, but their first goal is to register new Loaders to the ResourceManager factories, for the user to be able to use them at his convenience.

For example, if you develop a new Window System, a revolutionnary Window System, you can create easily a new Plugin for GRE. And any user will juste have to load the Plugin to use your Window System, without changing anything of its own Window-related code !

## Notes, dependancies, other things

The Engine is completly written in C++11. You, obviously, need a compiler that can compile C++11 (We recommend at least GCC, Clang/llvm, MSVC++).
Dependencies are, for now, only from loading and unloading Dynamic Libraries (for the Engine to work alone).
