//////////////////////////////////////////////////////////////////////
//
//  Camera.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 07/05/2016.
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

#include "Camera.h"
#include "FreeMovingCamera.h"

GreBeginNamespace

Camera::Camera(const std::string& name) : Gre::Resource(name)
{
    iPosition = { 0.0f , 0.0f , 0.0f } ;
    iTarget = { 0.0f , 0.0f , 0.0f } ;
    iUpwardDirection = Vector3 ( 0.0f, 1.0f, 0.0f ) ;
    iMustCalcView = true ;

    iMaxVerticalAngle = 89.0f ;

    iControllerFilters.push_back ( EventType::DirectionChanged ) ;
    iControllerFilters.push_back ( EventType::PositionChanged ) ;
    iControllerFilters.push_back ( EventType::WindowSized ) ;
}

Camera::Camera(const std::string& name, const Vector3& position, const Vector3& to, const Vector3& up) : Gre::Resource(name)
{
    iPosition = position ;
    iTarget = to ;
    iUpwardDirection = up ;
    iMustCalcView = true ;

    iMaxVerticalAngle = 89.0f ;

    iControllerFilters.push_back ( EventType::DirectionChanged ) ;
    iControllerFilters.push_back ( EventType::PositionChanged ) ;
    iControllerFilters.push_back ( EventType::WindowSized ) ;
}

Camera::~Camera()
{
    clearController();
}

void Camera::setPosition(const Vector3 &position)
{
    GreAutolock ;

    if ( iPosition != position )
    {
        iPosition = position ;
        iMustCalcView = true ;
    }
}

const Vector3 & Camera::getPosition() const
{
    GreAutolock ; return iPosition ;
}

void Camera::setTarget(const Vector3 &target)
{
    GreAutolock ;

    if ( iTarget != target )
    {
        iTarget = target ;
        iMustCalcView = true ;
    }
}

const Vector3 & Camera::getTarget() const
{
    GreAutolock ; return iTarget ;
}

Vector3 Camera::getDirection() const
{
    GreAutolock ; return iTarget - iPosition ;
}

void Camera::setDirection(const Vector3 &direction)
{
    GreAutolock ;

    if ( iTarget - iPosition != direction )
    {
        iTarget = iPosition + glm::normalize( direction ) ;
        iMustCalcView = true ;
    }
}

const Vector3& Camera::getUp() const
{
    GreAutolock ; return iUpwardDirection ;
}

void Camera::lookAt(const Vector3 &origin, const Vector3 &point, const Vector3& up)
{
    GreAutolock ;

    // We assume this function is called by 'onUpdateEvent'. So , we must always send
    // an event , and we don't have to copy the vectors. We just calculate the view matrix
    // and sends events to notifiate our listeners.

    Matrix4 view = glm::lookAt(origin, point, up) ;
    iFrustrum.setView(view) ;
    iFrustrum.computePlanes() ;

    EventHolder e1 ( new PositionChangedEvent(this, origin) ) ;
    sendEvent(e1) ;

    EventHolder e2 ( new DirectionChangedEvent(this, point - origin) ) ;
    sendEvent(e2) ;
    
    iMustCalcView = false ;
}

bool Camera::contains(const Vector3 &object) const
{
    GreAutolock ; return iFrustrum.contains(object);
}

bool Camera::isVisible(const Gre::BoundingBox &bbox) const
{
    GreAutolock ; return iFrustrum.intersect(bbox) != IntersectionResult::Outside;
}

const Matrix4 & Camera::getProjectionMatrix() const
{
    GreAutolock ; return iFrustrum.getPerspective() ;
}

const Matrix4 & Camera::getViewMatrix() const
{
    GreAutolock ; return iFrustrum.getView() ;
}

Matrix4 Camera::getProjectionViewMatrix () const
{
    GreAutolock ; return getProjectionMatrix() * getViewMatrix() ;
}

void Camera::setController(const EventProceederUser &user)
{
    GreAutolock ;

    if ( !iController.isInvalid() ) {
        iController.lock() -> removeListener(EventProceederUser(this));
    }

    iController = user ;

    if ( !iController.isInvalid() ) {
        iController.lock() -> addFilteredListener(EventProceederUser(this), iControllerFilters) ;
    }
}

void Camera::clearController ()
{
    GreAutolock ;

    if ( !iController.isInvalid() ) {
        iController.lock() -> removeListener ( EventProceederUser(this) ) ;
        iController.clear();
    }
}

const Frustrum& Camera::getFrustrum() const
{
    GreAutolock ; return iFrustrum ;
}

Frustrum& Camera::getFrustrum()
{
    GreAutolock ; return iFrustrum ;
}

void Camera::setFrustrum(const Gre::Frustrum &frustrum)
{
    GreAutolock ; iFrustrum = frustrum ;
}

void Camera::use ( const TechniqueHolder & technique ) const
{
    GreAutolock ;

    if ( !technique.isInvalid() )
    {
        technique -> setAliasedParameterValue ( TechniqueParam::CameraPosition , HdwProgVarType::Float3 , iPosition ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::CameraDirection , HdwProgVarType::Float3 , iTarget - iPosition ) ;

        technique -> setAliasedParameterValue ( TechniqueParam::ViewMatrix , HdwProgVarType::Matrix4 , getViewMatrix() ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ProjectionMatrix , HdwProgVarType::Matrix4 , getProjectionMatrix() ) ;
        technique -> setAliasedParameterValue ( TechniqueParam::ProjectionViewMatrix , HdwProgVarType::Matrix4 , getProjectionViewMatrix() ) ;
    }
}

void Camera::onWindowSizedEvent(const Gre::WindowSizedEvent &e)
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Calculates new ratio depending on Window size.

    float ratio = e.Width / e.Height ;
    iFrustrum.setAspectRatio ( ratio ) ;
    iFrustrum.computePlanes () ;
}

void Camera::onUpdateEvent(const Gre::UpdateEvent &e)
{
    GreAutolock ;

    if ( iMustCalcView )
    {
        lookAt(iPosition, iTarget, iUpwardDirection) ;
    }
}

void Camera::onPositionChangedEvent(const Gre::PositionChangedEvent &e)
{
    setPosition(e.Position) ;
}

void Camera::onDirectionChangedEvent(const Gre::DirectionChangedEvent &e)
{
    setDirection(e.Direction) ;
}

// ---------------------------------------------------------------------------------------------------

CameraLoader::CameraLoader ()
{

}

CameraLoader::~CameraLoader () noexcept ( false )
{

}

// ---------------------------------------------------------------------------------------------------

DefaultCameraLoader::DefaultCameraLoader ()
{

}

DefaultCameraLoader::~DefaultCameraLoader () noexcept ( false )
{

}

CameraHolder DefaultCameraLoader::load(const std::string &name, const CameraOptions &options) const
{
    CameraHolder result ( new Camera(name) ) ;
    auto it = options.find("ProjectionMatrixType");

    if ( it != options.end() )
    {
        if ( it->second.to<std::string>() == "Orthogonal" )
            result -> getFrustrum () .setProjectionType ( ProjectionType::Orthogonal ) ;
        else if ( it->second.to<std::string>() == "Perspective" )
            result -> getFrustrum () .setProjectionType ( ProjectionType::Perspective ) ;
    }

    it = options.find("NearPlane");

    if ( it != options.end() ) {
        result -> getFrustrum () .setNear ( it->second.to<float>() ) ;
    }

    it = options.find("FarPlane");

    if ( it != options.end() ) {
        result -> getFrustrum () .setFar ( it->second.to<float>() ) ;
    }

    return result ;
}

ResourceLoader* DefaultCameraLoader::clone() const
{
    return new DefaultCameraLoader () ;
}

bool DefaultCameraLoader::isLoadable(const std::string &filepath) const
{
    return false ;
}

// ---------------------------------------------------------------------------------------------------

CameraManager::CameraManager ( const std::string & name )
: SpecializedResourceManager<Camera, CameraLoader> ( name )
{
    // Default Camera is centered at the beginning of the scene, looking straith forward.

    CameraHolder camera = CameraHolder ( new Camera("Default") ) ;
    camera -> lookAt({2.0f, 2.0f, -2.0f}, {2.0f, 2.0f, 5.0f});
    iHolders.push_back(camera);

    // We should add here some built-in Camera Loaders. This may help the user to create other Camera's
    // types and behaviours.

    getFactory().registers("FreeMovingCameraLoader" , new FreeMovingCameraLoader());
    getFactory().registers("DefaultCameraLoader" , new DefaultCameraLoader());
}

CameraManager::~CameraManager () noexcept ( false )
{

}

CameraHolder CameraManager::loadCamera ( const std::string & name )
{
    GreAutolock ;

    //////////////////////////////////////////////////////////////////////
    // Creates a new Camera object with no parameters , registers it to the
    // manager and returns it. Notes this camera should always be updated by
    // this manager when created this way.

    CameraHolder camera = new Camera ( name ) ;

    if ( camera.isInvalid() )
    return CameraHolder ( nullptr ) ;

    addFilteredListener ( EventProceederUser(camera) , { EventType::Update } ) ;
    iHolders.add ( camera ) ;

    return camera ;
}

CameraUser CameraManager::load ( const std::string & name , const CameraOptions & options )
{
    GreAutolock ;

    auto loadername = options.find("Loader") ;
    if ( loadername != options.end() )
    {
        std::string lname = loadername->second.to<std::string>() ;
        CameraLoader* loader = findLoader( lname ) ;

        if ( loader )
        {
            CameraHolder newcam = loader->load(name, options) ;

            auto it = options.find ( "Position" ) ;
            if ( it != options.end() ) {
                newcam -> setPosition ( it->second.to<Vector3>() ) ;
            }

            it = options.find ( "Direction" ) ;
            if ( it != options.end() ) {
                newcam -> setDirection ( it->second.to<Vector3>() ) ;
            }

            it = options.find ( "Target" ) ;
            if ( it != options.end() ) {
                newcam -> setTarget ( it->second.to<Vector3>() ) ;
            }

            bool sendupdate = true ;
            it = options.find ( "SendUpdate" ) ;
            if ( it != options.end() ) {
                sendupdate = it->second.to<bool>() ;
            }

            if ( sendupdate ) {
                addFilteredListener ( EventProceederUser(newcam.getObject()) , { EventType::Update} ) ;
            }

            GreDebug ( "[INFO] Camera '" ) << newcam -> getName() << "' registered." << gendl ;

            iHolders.push_back(newcam) ;
            return CameraUser ( newcam ) ;
        }

        else
        {
            GreDebug("No 'CameraLoader' found to load Camera '") << name << "' (options.loader='" << lname << "')." << Gre::gendl;
        }
    }

    else
    {
        GreDebug("No 'loader' options found in 'CameraOptions' object.") << Gre::gendl ;
    }

    return CameraUser ( nullptr ) ;
}

GreEndNamespace
