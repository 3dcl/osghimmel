
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

#include "astronomy2.h"

#include "earth2.h"
#include "sun2.h"
#include "moon2.h"
#include "stars.h"
#include "siderealtime.h"
#include "interpolate.h"


namespace osgHimmel
{

Astronomy2::Astronomy2()
{
}


const float Astronomy2::sunDistance(const t_julianDay t) const
{
    return Sun2::distance(t);
}

const float Astronomy2::angularSunRadius(const t_julianDay t) const
{
    return Earth2::apparentAngularSunDiameter(t) * 0.5;
}


const float Astronomy2::moonRadius() const
{
    return Moon2::meanRadius();
}


const float Astronomy2::moonDistance(const t_julianDay t) const
{
    return Moon2::distance(t);
}

const float Astronomy2::angularMoonRadius(const t_julianDay t) const
{
    return Earth2::apparentAngularMoonDiameter(t) * 0.5;
}


const osg::Vec3f Astronomy2::moonPosition(
    const t_aTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    t_horf moon = Moon2::horizontalPosition(aTime, latitude, longitude);
    if(refractionCorrected)
        moon.altitude += Earth2::atmosphericRefraction(moon.altitude);

    osg::Vec3f moonv  = moon.toEuclidean();
    moonv.normalize();

    return moonv;
}


const osg::Vec3f Astronomy2::sunPosition(
    const t_aTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    t_horf sun = Sun2::horizontalPosition(aTime, latitude, longitude);

    if(refractionCorrected)
        sun.altitude += Earth2::atmosphericRefraction(sun.altitude);

    osg::Vec3f sunv  = sun.toEuclidean();
    sunv.normalize();

    return sunv;
}


const osg::Matrixf Astronomy2::moonOrientation(
    const t_aTime &aTime
,   const float latitude
,   const float longitude) const
{    
    const t_julianDay t(jd(aTime));

    float l, b;
    Moon2::opticalLibrations(t, l, b);

    const osg::Matrixf libLat = osg::Matrixf::rotate(_rad(b), -1, 0, 0);
    const osg::Matrixf libLon = osg::Matrixf::rotate(_rad(l),  0, 1, 0);

    const float a = _rad(Moon2::positionAngleOfAxis(t));
    const float p = _rad(Moon2::parallacticAngle(aTime, latitude, longitude));

    const osg::Matrixf zenith = osg::Matrixf::rotate(a - p, 0, 0, 1);

    // finalOrientationWithLibrations
    const osg::Matrixf R(libLat * libLon * zenith);

    return R;
}


const float Astronomy2::earthShineIntensity(
    const t_aTime &aTime
,   const float latitude
,   const float longitude) const
{
    const osg::Vec3f m = moonPosition(aTime, latitude, longitude, false);
    const osg::Vec3f s = sunPosition(aTime, latitude, longitude, false);

    // ("Multiple Light Scattering" - 1980 - Van de Hulst) and 
    // ("A Physically-Based Night Sky Model" - 2001 - Wann Jensen et al.) -> the 0.19 is the earth full intensity
    
    // My approximation with non-perceivable difference. (max error is ~2.6 %)
    //"    float ep = acos(dot(-m, sun));\n"
    //"    float Eem = 0.095 * smoothstep(0.0, PI, ep) - 0.01 * sin(ep);\n"
    //"\n"

    const float ep  = acos(s * (-m));
    //const float Eem = 0.1095 * _smoothstep(ep / _PI) - 0.01 * sin(ep);

    const float ep2 = ep * ep;
    const float ep3 = ep * ep2;

    const float Eem = -0.0061 * ep3 + 0.0289 * ep2 - 0.0105 * sin(ep); // with solved smoothstep and bold factors, max error to hulst 3%

    return Eem;
}


const osg::Matrixf Astronomy2::equToHorTransform(
        const t_aTime &aTime
    ,   const float latitude
    ,   const float longitude) const
{
    const float s = siderealTime2(aTime);

    return osg::Matrixf::scale(-1, 1, 1)
        * osg::Matrixf::rotate( _rad(latitude) -  _PI_2, 1, 0, 0)
        * osg::Matrixf::rotate(-_rad(s + longitude)    , 0, 0, 1);
}

} // namespace osgHimmel