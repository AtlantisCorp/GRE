//////////////////////////////////////////////////////////////////////
//
//  Controller.h
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 11/05/2017.
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

#ifndef GRE_CONTROLLER_H
#define GRE_CONTROLLER_H

#include "RenderNode.h"

GreBeginNamespace

//////////////////////////////////////////////////////////////////////
/// @brief Defines a basic controller object.
///
/// A Controller is a Resource able to manage one or more nodes, responding
/// to events listened by this controller. For example, a controller can
/// listen to a Window's keys events, and control a camera node in order
/// to make it move. But it can also control other nodes, thus the
/// controller is able to do almost any desired action on a node.
///
/// Basic controllers are cloned from a base. When a user wants to create
/// a totally new controller, it should be implemented either as a
/// plugin or directly in the program, as a new class.
///
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC Controller : public Resource
{
public:

    POOLED ( Pools::Referenced )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    Controller ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~Controller () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a Controller of the same type of this one. This function
    /// should always be overwritten by the user or the derived type. Notes
    /// it may or may not copy the informations held by this controller to
    /// the cloned one.
    //////////////////////////////////////////////////////////////////////
    virtual Holder < Controller > clone ( const std::string & name ) const = 0 ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Adds a rendernode to control.
    //////////////////////////////////////////////////////////////////////
    virtual void control ( const Holder < RenderNode > & rendernode ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes a rendernode from the control.
    //////////////////////////////////////////////////////////////////////
    virtual void neglect ( const Holder < RenderNode > & rendernode ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes every rendernode from the controller.
    //////////////////////////////////////////////////////////////////////
    virtual void neglectAll () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns every controlled rendernodes.
    //////////////////////////////////////////////////////////////////////
    virtual const RenderNodeHolderList & getControlled () const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Calls 'neglectAll()' and the base's clear.
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

protected:

    /// @brief Controlled rendernodes.
    RenderNodeHolderList iControlled ;
};

GRE_MAKE_HOLDER( Controller ) ;

//////////////////////////////////////////////////////////////////////
/// @brief Manages Controllers loaded by the user, or provided by any
/// controller provider.
//////////////////////////////////////////////////////////////////////
class DLL_PUBLIC ControllerManager : public Resource
{
public:

    POOLED ( Pools::Manager )

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    ControllerManager ( const std::string & name = std::string("controllers.managers.default") ) ;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    virtual ~ControllerManager () noexcept ( false ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new controller, cloning the given one. This method
    /// should be used to register a Controller.
    //////////////////////////////////////////////////////////////////////
    virtual void create ( const std::string & createdname , const ControllerHolder & controller ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Creates a new controller with given name and from given base
    /// controller.
    //////////////////////////////////////////////////////////////////////
    virtual ControllerHolder create ( const std::string & createdname , const std::string & creatorname ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the controller specified by given name.
    //////////////////////////////////////////////////////////////////////
    virtual const ControllerHolder get ( const std::string & name ) const ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Returns the controller specified by given name.
    //////////////////////////////////////////////////////////////////////
    virtual ControllerHolder get ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the controller specified by name.
    //////////////////////////////////////////////////////////////////////
    virtual void remove ( const std::string & name ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Removes the controller specified by holder.
    //////////////////////////////////////////////////////////////////////
    virtual void remove ( const ControllerHolder & controller ) ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Remove every controllers in this manager.
    //////////////////////////////////////////////////////////////////////
    virtual void removeAll () ;

    //////////////////////////////////////////////////////////////////////
    /// @brief Calls 'removeAll()' and base's clear.
    //////////////////////////////////////////////////////////////////////
    virtual void clear () ;

protected:

    /// @brief Controllers created by this manager.
    ControllerHolderList iControllers ;
};

GRE_MAKE_HOLDER( ControllerManager ) ;

GreEndNamespace

#endif // GRE_CONTROLLER_H
