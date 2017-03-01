//////////////////////////////////////////////////////////////////////
//
//  Light.cpp
//  This source file is part of Gre
//		(Gang's Resource Engine)
//
//  Copyright (c) 2015 - 2016 Luk2010
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

GreBeginNamespace

Light::Light ( const Vector3& position , const Color& ambient , const Color& diffuse , const Color& specular , float shininess )
: Gre::Lockable()
, iType(LightType::Directionnal) , iEnabled(true)
, iPosition(position) , iAmbient(ambient) , iDiffuse(diffuse) , iSpecular(specular) , iShininess(shininess)
{
    iAttenuationCst = 0.0f ;
    iAttenuationLinear = 0.0f ;
    iAttenuationQuad = 0.0f ;
    iDirection = Vector3(0.0f, 0.0f, 0.0f) ;
    iAngle = glm::radians(12.5f) ;
    iExposition = 64.0f ;
}

Light::Light ( const Light& rhs )
: Gre::Lockable()
, iType(rhs.iType) , iEnabled(rhs.iEnabled)
, iPosition(rhs.iPosition) , iAmbient(rhs.iAmbient) , iDiffuse(rhs.iDiffuse) , iSpecular(rhs.iSpecular)
, iShininess(rhs.iShininess) , iAttenuationCst(rhs.iAttenuationCst) , iAttenuationLinear(rhs.iAttenuationLinear)
, iAttenuationQuad(rhs.iAttenuationQuad) , iDirection(rhs.iDirection) , iAngle(rhs.iAngle) , iExposition(rhs.iExposition)
{
    
}

Light& Light::operator=(const Gre::Light &rhs)
{
    iType = rhs.iType ;
    iEnabled = rhs.iEnabled ;
    iPosition = rhs.iPosition ;
    iAmbient = rhs.iAmbient ;
    iDiffuse = rhs.iDiffuse ;
    iSpecular = rhs.iSpecular ;
    iShininess = rhs.iShininess ;
    iAttenuationCst = rhs.iAttenuationCst ;
    iAttenuationLinear = rhs.iAttenuationLinear ;
    iAttenuationQuad = rhs.iAttenuationQuad ;
    iDirection = rhs.iDirection ;
    iAngle = rhs.iAngle ;
    iExposition = rhs.iExposition ;
    return *this ;
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

const Vector3& Light::getPosition () const
{
    GreAutolock ; return iPosition ;
}

const Color& Light::getAmbient() const
{
    GreAutolock ; return iAmbient ;
}

const Color& Light::getDiffuse() const
{
    GreAutolock ; return iDiffuse ;
}

const Color& Light::getSpecular() const
{
    GreAutolock ; return iSpecular ;
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

GreEndNamespace
