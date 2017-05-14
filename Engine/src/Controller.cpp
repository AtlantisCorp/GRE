//////////////////////////////////////////////////////////////////////
//
//  Controller.cpp
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

#include "Controller.h"

GreBeginNamespace

// -----------------------------------------------------------------------------

Controller::Controller ( const std::string & name ) : Gre::Resource ( name )
{

}

Controller::~Controller () noexcept ( false )
{

}

void Controller::control ( const Holder < RenderNode > & rendernode )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Check if node is not already controlled , or null.

    if ( rendernode.isInvalid() )
    return ;

    auto checkit = std::find ( iControlled.begin() , iControlled.end() , rendernode ) ;

    if ( checkit != iControlled.end() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // If we are here , add the controlled node.

    iControlled.push_back ( rendernode ) ;
}

void Controller::neglect ( const Holder < RenderNode > & rendernode )
{
    GreAutolock ;

    if ( rendernode.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Check if node is in here , if so removes it.

    auto checkit = std::find ( iControlled.begin() , iControlled.end() , rendernode ) ;

    if ( checkit != iControlled.end() )
    iControlled.erase ( checkit ) ;
}

void Controller::neglectAll ()
{
    GreAutolock ; iControlled.clear () ;
}

const RenderNodeHolderList & Controller::getControlled () const
{
    GreAutolock ; return iControlled ;
}

void Controller::clear ()
{
    neglectAll () ;
    Gre::Resource::clear () ;
}

// -----------------------------------------------------------------------------

ControllerManager::ControllerManager ( const std::string & name ) : Gre::Resource ( name )
{

}

ControllerManager::~ControllerManager () noexcept ( false )
{

}

void ControllerManager::create ( const std::string & createdname , const ControllerHolder & controller )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Checks arguments.

    if ( createdname.empty() || controller.isInvalid() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Checks controller name is not already taken.

    auto checkit = iControllers.find ( createdname ) ;
    if ( checkit != iControllers.end() )
    return ;

    //////////////////////////////////////////////////////////////////////
    // Clones the controller and register it.

    ControllerHolder cloned = controller -> clone ( createdname ) ;
    if ( cloned.isInvalid() )
    return ;

    iControllers.push_back ( cloned ) ;
    addFilteredListener ( cloned , { EventType::Update } ) ;

    GreDebug ( "[INFO] Added Controller '" ) << createdname << "'." << gendl ;
}

ControllerHolder ControllerManager::create ( const std::string & createdname , const std::string & creatorname )
{
    GreAutolock ;

    if ( createdname.empty() || creatorname.empty() )
    return ControllerHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Checks if controller's name already exists.

    auto checkit = iControllers.find ( createdname ) ;
    if ( checkit != iControllers.end() )
    return ControllerHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Checks creator name is valid.

    checkit = iControllers.find ( creatorname ) ;

    if ( checkit == iControllers.end() )
    return ControllerHolder ( nullptr ) ;

    if ( (*checkit).isInvalid() )
    return ControllerHolder ( nullptr ) ;

    //////////////////////////////////////////////////////////////////////
    // Creates the controller and returns it.

    ControllerHolder created = (*checkit)->clone ( createdname ) ;

    if ( created.isInvalid() )
    return ControllerHolder ( nullptr ) ;

    iControllers.push_back ( created ) ;
    addFilteredListener ( created , { EventType::Update } ) ;

    GreDebug ( "[INFO] Added Controller '" ) << createdname << "'." << gendl ;

    return created ;
}

const ControllerHolder ControllerManager::get ( const std::string & name ) const
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Finds the first controller with given name, or null.

    auto checkit = iControllers.find ( name ) ;

    if ( checkit == iControllers.end() )
    return ControllerHolder ( nullptr ) ;

    return (*checkit) ;
}

ControllerHolder ControllerManager::get ( const std::string & name )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Finds the first controller with given name, or null.

    auto checkit = iControllers.find ( name ) ;

    if ( checkit == iControllers.end() )
    return ControllerHolder ( nullptr ) ;

    return (*checkit) ;
}

void ControllerManager::remove ( const std::string & name )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Finds controller and removes it.

    auto checkit = iControllers.find ( name ) ;

    if ( checkit == iControllers.end() )
    return ;

    removeListener ( *checkit ) ;
    iControllers.erase ( checkit ) ;

    GreDebug ( "[INFO] Removed Controller '" ) << name << "'." << gendl ;
}

void ControllerManager::remove ( const ControllerHolder & controller )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Finds controller and removes it.

    auto checkit = std::find ( iControllers.begin() , iControllers.end() , controller ) ;

    if ( checkit == iControllers.end() )
    return ;

    removeListener ( controller ) ;
    iControllers.erase ( checkit ) ;

    GreDebug ( "[INFO] Removed Controller '" ) << controller->getName() << "'." << gendl ;
}

void ControllerManager::removeAll ()
{
    GreAutolock ; iControllers.clear () ;
}

void ControllerManager::clear ()
{
    removeAll () ;
    Gre::Resource::clear () ;
}

// -----------------------------------------------------------------------------

GreEndNamespace
