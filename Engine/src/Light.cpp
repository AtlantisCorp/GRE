//////////////////////////////////////////////////////////////////////
//
//  Light.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2017 Luk2010
//  Created on 20/02/2017.
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

#include "Light.h"
#include "ResourceManager.h"

GreBeginNamespace

Light::Light ( const std::string & name ) : Gre::Resource ( name )
{
    iType = LightType::Directionnal ;
    iEnabled = true ;
    iAttenuationCst = 0.0f ;
    iAttenuationLinear = 0.0f ;
    iAttenuationQuad = 0.0f ;
    iAngle = 0.0f ;
    iExposition = 0.0f ;
}

Light::Light ( const std::string& name ,
               const Vector3& position , const Vector3& direction ,
               bool shadowProjector ,
               const Color& ambient , const Color& diffuse , const Color& specular )

: Gre::Resource(name)
, iType(LightType::Directionnal)
, iEnabled(true)

, iPosition(position)
, iDirection(direction)

, iAmbient(ambient)
, iDiffuse(diffuse)
, iSpecular(specular)

, iAttenuationCst(0.0f)
, iAttenuationLinear(0.0f)
, iAttenuationQuad(0.0f)

, iAngle(glm::radians(12.5f))
, iExposition(64.0f)

{

}

Light::~Light() noexcept ( false )
{

}

const LightType& Light::getType() const
{
    GreAutolock ; return iType ;
}

void Light::setType(const Gre::LightType &type)
{
    GreAutolock ; iType = type ;
}

bool Light::isEnabled() const
{
    GreAutolock ; return iEnabled ;
}

void Light::setEnabled(bool value)
{
    GreAutolock ; iEnabled = value ;
}

void Light::setPosition(const Vector3 &position)
{
    GreAutolock ; iPosition = position ;
    
    //////////////////////////////////////////////////////////////////////
    // Sends a Position changed event.
    
    EventHolder e = new PositionChangedEvent ( this , position ) ;
    sendEvent(e) ;
}

const Vector3& Light::getPosition () const
{
    GreAutolock ; return iPosition ;
}

const Color& Light::getAmbient() const
{
    GreAutolock ; return iAmbient ;
}

void Light::setAmbient(const Color &ambient)
{
    GreAutolock ; iAmbient = ambient ;
}

const Color& Light::getDiffuse() const
{
    GreAutolock ; return iDiffuse ;
}

void Light::setDiffuse(const Color &diffuse)
{
    GreAutolock ; iDiffuse = diffuse ;
}

const Color& Light::getSpecular() const
{
    GreAutolock ; return iSpecular ;
}

void Light::setSpecular(const Color &specular)
{
    GreAutolock ; iSpecular = specular ;
}

float Light::getShininess() const
{
    GreAutolock ; return iShininess ;
}

float Light::getAttenuationCst() const
{
    GreAutolock ; return iAttenuationCst ;
}

void Light::setAttenuationCst(float value)
{
    GreAutolock ; iAttenuationCst = value ;
}

float Light::getAttenuationLinear() const
{
    GreAutolock ; return iAttenuationLinear ;
}

void Light::setAttenuationLinear (float value)
{
    GreAutolock ; iAttenuationLinear = value ;
}

float Light::getAttenuationQuad() const
{
    GreAutolock ; return iAttenuationQuad ;
}

void Light::setAttenuationQuad(float value)
{
    GreAutolock ; iAttenuationQuad = value ;
}

const Vector3& Light::getDirection () const
{
    GreAutolock ; return iDirection ;
}

void Light::setDirection(const Vector3& direction)
{
    GreAutolock ; iDirection = direction ;
    
    //////////////////////////////////////////////////////////////////////
    // Sends a Direction Changed event. ( This helps the camera from shadow
    // matrix knows updates )
    
    EventHolder e = new DirectionChangedEvent ( this , iDirection ) ;
    sendEvent(e) ;
}

float Light::getAngle () const
{
    GreAutolock ; return iAngle ;
}

void Light::setAngle(float value)
{
    GreAutolock ; iAngle = value ;
}

float Light::getExposition() const
{
    GreAutolock ; return iExposition ;
}

void Light::setExposition(float value)
{
    GreAutolock ; iExposition = value ;
}

IntersectionResult Light::intersectFrustrum(const Gre::Frustrum &frustrum) const
{
    GreAutolock ;
    
    //////////////////////////////////////////////////////////////////////
    // If the light is direction , return always intersect.
    
    if ( iType == LightType::Directionnal )
    return IntersectionResult::Between ;
    
    //////////////////////////////////////////////////////////////////////
    // If the light is a point , make a sphere from attenuation value and
    // test it with the frustrum planes.
    
    if (iType == LightType::Directionnal &&
        !iAttenuationCst && !iAttenuationQuad && !iAttenuationLinear)
    return IntersectionResult::Between ;
    
    //////////////////////////////////////////////////////////////////////
    // Computes the maximum distance from attenuations values.
    
    float maxatt = 1/0.001f ;
    float a = iAttenuationQuad ;
    float b = iAttenuationLinear ;
    float c = iAttenuationCst - maxatt ;
    
    float delta = b*b - 4*a*c ;
    float x1 = (-b - sqrtf(delta)) / 2 * a ;
    float x2 = ( -b - sqrtf(delta) ) / 2 * a ;
    float dist = std::max(x1, x2) ;
    
    if ( iType == LightType::Point )
    {
        //////////////////////////////////////////////////////////////////////
        // The light sphere has for radius the distance , and for center the
        // light position . Returns it intersection with the frustrum .
        
        return frustrum.intersect ( dist , iPosition ) ;
    }
    
    if ( iType == LightType::Spot )
    {
        //////////////////////////////////////////////////////////////////////
        // Computes the frustrum which goes with this spot.
        
        Frustrum lightproj (iAngle, 16/9, 0.1f, dist) ;
        lightproj.setView(glm::lookAt(iPosition, iPosition + iDirection, {0.0f , 1.0f , 0.0f}));
        lightproj.computePlanes() ;
        
        return frustrum.intersect ( lightproj ) ;
    }
    
    return IntersectionResult::Outside ;
}

GreEndNamespace
