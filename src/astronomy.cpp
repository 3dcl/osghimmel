
// Copyright (c) 2011-2012, Daniel M�ller <dm@g4t3.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright 
//     notice, this list of conditions and the following disclaimer in the 
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the 
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its 
//     contributors may be used to endorse or promote products derived from 
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include "astronomy.h"

#include "earth.h"
#include "sun.h"
#include "moon.h"
#include "stars.h"
#include "siderealtime.h"


namespace osgHimmel
{

Astronomy::Astronomy()
{
}


const float Astronomy::sunDistance(const t_julianDay t) const
{
    return Sun::distance(t);
}

const float Astronomy::angularSunRadius(const t_julianDay t) const
{
    return Earth::apparentAngularSunDiameter(t) * 0.5;
}


const float Astronomy::moonRadius() const
{
    return Moon::meanRadius();
}


const float Astronomy::moonDistance(const t_julianDay t) const
{
    return Moon::distance(t);
}

const float Astronomy::angularMoonRadius(const t_julianDay t) const
{
    return Earth::apparentAngularMoonDiameter(t) * 0.5;
}


const osg::Vec3f Astronomy::moonPosition(
    const t_aTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    t_hord moon = Moon::horizontalPosition(aTime, latitude, longitude);
    if(refractionCorrected)
        moon.altitude += Earth::atmosphericRefraction(moon.altitude);

    osg::Vec3f moonv  = moon.toEuclidean();
    moonv.normalize();

    return moonv;
}


const osg::Vec3f Astronomy::sunPosition(
    const t_aTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    t_hord sun = Sun::horizontalPosition(aTime, latitude, longitude);
    if(refractionCorrected)
        sun.altitude += Earth::atmosphericRefraction(sun.altitude);

    osg::Vec3f sunv  = sun.toEuclidean();
    sunv.normalize();

    return sunv;
}


const osg::Matrixf Astronomy::moonOrientation(
    const t_aTime &aTime
,   const float latitude
,   const float longitude) const
{    
    const t_julianDay t(jd(aTime));

    t_longf l, b;
    Moon::opticalLibrations(t, l, b);

    const osg::Matrixf libLat = osg::Matrixf::rotate(_rad(b), -1, 0, 0);
    const osg::Matrixf libLon = osg::Matrixf::rotate(_rad(l),  0, 1, 0);

    const float a = _rad(Moon::positionAngleOfAxis(t));
    const float p = _rad(Moon::parallacticAngle(aTime, latitude, longitude));

    const osg::Matrixf zenith = osg::Matrixf::rotate(p - a, 0, 0, 1);

    // finalOrientationWithLibrations
    const osg::Matrixf R(libLat * libLon * zenith);

    return R;
}


const float Astronomy::earthShineIntensity(
    const t_aTime &aTime
,   const float latitude
,   const float longitude) const
{
    const osg::Vec3f m = moonPosition(aTime, latitude, longitude, false);
    const osg::Vec3f s = sunPosition(aTime, latitude, longitude, false);

    // ("Multiple Light Scattering" - 1980 - Van de Hulst) and 
    // ("A Physically-Based Night Sky Model" - 2001 - Wann Jensen et al.) -> the 0.19 is the earth full intensity
    
    const float ep  = (_PI - acos(s * (-m))) * 0.5;
    const float Eem = 0.19 * 0.5 * (1.0 - sin(ep) * tan(ep) * log(1.0 / tan(ep * 0.5)));

    return Eem;
}


const osg::Matrixf Astronomy::equToHorTransform(
    const t_aTime &aTime
,   const float latitude
,   const float longitude) const
{
    const t_julianDay T(jCenturiesSinceSE(jd(aTime)));
    const float s = siderealTime(aTime);

    return osg::Matrixf::scale(-1, 1, 1)
        * osg::Matrixf::rotate( _rad(latitude)  - _PI_2, 1, 0, 0)
        * osg::Matrixf::rotate(-_rad(s + longitude)    , 0, 0, 1)
        // precession as suggested in (Jensen et al. 2001)
        * osg::Matrixf::rotate( 0.01118 * T, 0, 0, 1)
        * osg::Matrixf::rotate(-0.00972 * T, 1, 0, 0)
        * osg::Matrixf::rotate( 0.01118 * T, 0, 0, 1);
}

} // namespace osgHimmel