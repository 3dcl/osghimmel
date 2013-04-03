﻿
// Copyright (c) 2011-2012, Daniel Müller <dm@g4t3.de>
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


#include "test_astronomy.h"
#include "test.h"

#include "osgHimmel/mathmacros.h"
#include "osgHimmel/atime.h"
#include "osgHimmel/julianday.h"
#include "osgHimmel/siderealtime.h"
#include "osgHimmel/coords.h"
#include "osgHimmel/moon.h"
#include "osgHimmel/sun.h"
#include "osgHimmel/earth.h"
#include "osgHimmel/stars.h"


using namespace osgHimmel;

void test_aTime();
void test_jd();
void test_sideralTime();
void test_coords();
void test_sun();
void test_moon();
void test_stars();
void test_earth();

void test_astronomy()
{
    // Run Tests.
    test_aTime();
    test_jd();
    test_sideralTime();
    test_coords();
    test_sun();
    test_moon();
    test_stars();
    test_earth();

    TEST_REPORT();
}


void test_aTime()
{
    // Test decimal day time.
    t_aTime aTime0(1957, 10, 4.81);

    ASSERT_EQ(__int64, 4, aTime0.day);
    ASSERT_EQ(__int64, 19, aTime0.hour);
    ASSERT_EQ(__int64, 26, aTime0.minute);
    ASSERT_EQ(__int64, 24, aTime0.second);

    // Test decimal day time:.
    t_aTime aTime1(2000, 1, 1);

    ASSERT_EQ(__int64, 1, aTime1.day);
    ASSERT_EQ(__int64, 0, aTime1.hour);
    ASSERT_EQ(__int64, 0, aTime1.minute);
    ASSERT_EQ(__int64, 0, aTime1.second);
}


void test_jd()
{
    // Times from "Astronomical Algorithms" - Chapter 7

    // Test some dates.

    // Standard equinox (decimal day time).
    ASSERT_AB(long double, 2436116.31,jd(t_aTime(1957, 10,  4.81)), 0.000001);
    ASSERT_EQ(long double, 1842713.0, jd(t_aTime( 333,  1, 27, 12, 0, 0)));
    
    // Test some dates.
    ASSERT_EQ(long double, 2451545.0, jd(t_aTime( 2000,  1,  1.5)));
    ASSERT_EQ(long double, 2446822.5, jd(t_aTime( 1987,  1, 27.0)));
    ASSERT_EQ(long double, 2446966.0, jd(t_aTime( 1987,  6, 19.5)));
    ASSERT_EQ(long double, 2447187.5, jd(t_aTime( 1988,  1, 27.0)));
    ASSERT_EQ(long double, 2447332.0, jd(t_aTime( 1988,  6, 19.5)));
    ASSERT_AB(long double, 2443259.9, jd(t_aTime( 1977,  4, 26.4)), 0.000001);
    ASSERT_EQ(long double, 2415020.5, jd(t_aTime( 1900,  1,  1.0)));
    ASSERT_EQ(long double, 2305447.5, jd(t_aTime( 1600,  1,  1.0)));
    ASSERT_EQ(long double, 2305812.5, jd(t_aTime( 1600, 12, 31.0)));
    ASSERT_AB(long double, 2026871.8, jd(t_aTime(  837,  4, 10.3)), 0.000001);
    ASSERT_EQ(long double, 1356001.0, jd(t_aTime(-1000,  7, 12.5)));
    ASSERT_EQ(long double, 1355866.5, jd(t_aTime(-1000,  2, 29.0)));
    ASSERT_AB(long double, 1355671.4, jd(t_aTime(-1001,  8, 17.9)), 0.000001);
    ASSERT_EQ(long double,       0.0, jd(t_aTime(-4712,  1,  1.5)));


    // Times from WikiPedia
    ASSERT_EQ(long double, 1721423.5000, jd(t_aTime(   1,  1,  1,  0,  0,  0)));
    ASSERT_EQ(long double, 1842713.0000, jd(t_aTime( 333,  1, 27, 12,  0,  0)));
    // Check Calendar Swap.
    ASSERT_EQ(long double, 2299160.5000, jd(t_aTime(1582, 10,  4, 24,  0,  0)));
    ASSERT_EQ(long double, 2299160.5000, jd(t_aTime(1582, 10, 15,  0,  0,  0)));
    // Check from http://www.stjarnhimlen.se/comp/tutorial.html.
    ASSERT_EQ(long double, 2448000.5000, jd(t_aTime(1990,  4, 19,  0,  0,  0))); 

    // Test some dates.
    ASSERT_EQ(long double, 2415020.5000, jd(t_aTime(1900,  1,  1,  0,  0,  0))); 
    ASSERT_EQ(long double, 2447893.0000, jd(t_aTime(1990,  1,  1, 12,  0,  0))); 
    ASSERT_EQ(long double, 2447893.2500, jd(t_aTime(1990,  1,  1, 18,  0,  0))); 

    // Standard Equinox.
    ASSERT_EQ(long double, 2451545.0000, jd(t_aTime(2000,  1,  1, 12,  0,  0))); 

    ASSERT_EQ(long double, 2453750.1875, jd(t_aTime(2006,  1, 14, 16, 30,  0))); 
    ASSERT_EQ(long double, 2455281.1875, jd(t_aTime(2010,  3, 25, 16, 30,  0))); 

    // Check undefined days.
    ASSERT_EQ(long double,      0.0000, jd(t_aTime(1582, 10,  5,  0,  0,  0))); 
    ASSERT_EQ(long double,      0.0000, jd(t_aTime(1582, 10, 14,  0,  0,  0))); 


    // Test mjd;
    ASSERT_EQ(long double,       0.0, mjd(t_aTime(1858, 11, 17.0)));




    // Test julianday to aTime.

    t_aTime aTime0 = makeTime(2436116.31);
    
    ASSERT_EQ(__int64, 1957, aTime0.year);
    ASSERT_EQ(__int64,   10, aTime0.month);
    ASSERT_EQ(__int64,    4, aTime0.day);
    ASSERT_EQ(__int64,   19, aTime0.hour);
    ASSERT_EQ(__int64,   26, aTime0.minute);
    ASSERT_EQ(__int64,   24, aTime0.second);

    ASSERT_AB(long double, 4.81, aTime0.dayf(), 0.000000001);


    t_aTime aTime1 = makeTime(1842713.0);
    
    ASSERT_EQ(__int64,  333, aTime1.year);
    ASSERT_EQ(__int64,    1, aTime1.month);
    ASSERT_EQ(__int64,   27, aTime1.day);
    ASSERT_EQ(__int64,   12, aTime1.hour);
    ASSERT_EQ(__int64,    0, aTime1.minute);
    ASSERT_EQ(__int64,    0, aTime1.second);

    ASSERT_EQ(long double, 27.5, aTime1.dayf());


    t_aTime aTime2 = makeTime(1507900.13);
    
    ASSERT_EQ(__int64, -584, aTime2.year);
    ASSERT_EQ(__int64,    5, aTime2.month);
    ASSERT_EQ(__int64,   28, aTime2.day);
    ASSERT_EQ(__int64,   15, aTime2.hour);
    ASSERT_EQ(__int64,    7, aTime2.minute);
    ASSERT_EQ(__int64,   12, aTime2.second);

    ASSERT_AB(long double, 28.63, aTime2.dayf(), 0.000001);
}


void test_sideralTime()
{
    ASSERT_AB(long double, _hour(13, 10, 46.3668), _hours(siderealTime(t_aTime(1987, 4, 10,  0,  0, 0))), 0.00000001);
    ASSERT_AB(long double, _hour( 8, 34, 57.0896), _hours(siderealTime(t_aTime(1987, 4, 10, 19, 21, 0))), 0.00000001);
}


void test_coords()
{
    t_julianDay t;

    // Test nutation and obliquity. (AA. Example 21.a)

    t = jd(t_aTime(1987,  4, 10, 0, 0, 0));

    ASSERT_AB(long double, -0.127296372348, jCenturiesSinceSE(t),             0.00000001);
    ASSERT_AB(long double, _decimal(23, 26, 27.407), Earth::meanObliquity(t), 0.000001);

    ASSERT_AB(long double, _decimal( 0,  0,  9.443), Earth::obliquityNutation(t), 0.00005);
    ASSERT_AB(long double, _decimal(23, 26, 36.850), Earth::trueObliquity(t),     0.005);

    ASSERT_AB(long double, _decimal( 0,  0, -3.788), Earth::longitudeNutation(t), 0.00005);

    ASSERT_AB(long double, 229.27840, Moon::meanAnomaly(t),  0.001);


    t = jd(t_aTime(1992, 10, 13, 0, 0, 0));

    ASSERT_AB(long double, -0.072183436, jCenturiesSinceSE(t), 0.00000001);

    ASSERT_AB(long double, 201.80719, Sun::meanLongitude(t),   0.00001);
    ASSERT_AB(long double, 278.99396, Sun::meanAnomaly(t),     0.00001);
    ASSERT_AB(long double,  -1.89732, Sun::center(t),          0.00001);
    ASSERT_AB(long double, 199.90987, Sun::trueLongitude(t),   0.00001);
    ASSERT_AB(long double,  23.44023, Earth::meanObliquity(t), 0.000001);

    ASSERT_AB(long double, 0.99766, _AUs(Sun::distance(t)), 0.00001);
    ASSERT_AB(long double, 0.016711651, Earth::orbitEccentricity(t), 0.000001);


    t_equd equ = Sun::apparentPosition(t);

    ASSERT_AB(long double,  13.225388, _hours(equ.right_ascension), 0.00005);
    ASSERT_AB(long double, - 7.78507 , equ.declination,             0.005);
}



void test_sun()
{
    t_aTime aTime;

    // Berlin
    
    long double lat = _decimal(52, 31, 0);
    long double lon = _decimal(13, 24, 0);

    // Test nutation and obliquity.

    // Azimuth is interpreted from north from:
    // http://www.sunposition.info/sunposition/spc/locations.php

    aTime = t_aTime(2011, 12, 4,  8, 0, 0, 1 * 3600); 
    {
        const int hour[9] = { 8, 9, 10, 11, 12, 13, 14, 15, 16 };
        const int azim[9] = { 127, 139, 152, 166, 180, 195, 208, 221, 233 };
        const int alt[9]  = { -1, 6, 11, 14, 15, 14, 11, 5, -1 };

        for(unsigned int i = 0; i < 9; ++i)
        {
            aTime.hour = hour[i];
            t_hord hor = Sun::horizontalPosition(aTime, lat, lon);

            ASSERT_AB(int, azim[i] - 180, hor.azimuth, 1);
            ASSERT_AB(int, alt[i], hor.altitude, 1);
        }
    }

    aTime = t_aTime(2011,  6, 4,  8, 0, 0, 2 * 3600); 
    {
        const int hour[17] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
        const int azim[17] = { 52, 63, 75, 86, 98,  112, 129, 151, 177, 204, 227, 245, 259, 271, 283, 294, 305 };
        const int alt[17]  = { 1, 8, 17, 26, 35, 44, 52, 57, 60, 58, 53, 45, 36, 27, 18, 10, 2 };

        for(unsigned int i = 0; i < 17; ++i)
        {
            aTime.hour = hour[i];
            t_hord hor = Sun::horizontalPosition(aTime, lat, lon);

            ASSERT_AB(int, azim[i] - 180, hor.azimuth, 1);
            ASSERT_AB(int, alt[i], hor.altitude, 1);
        }
    }

    // random: Peru, Pueblo Libre

    lat = _decimal(-12,  5, 0);
    lon = _decimal(-77,  4, 0);

    aTime = t_aTime(2007,  5, 16,  0, 0, 0, -5 * 3600); 
    {
        const int hour[12] = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
        const int azim[12] = { 68, 63, 56, 44, 27, 2, 336, 317, 305, 297, 292, 289 };
        const int alt[12]  = { 9, 22, 35, 46, 55, 59, 56, 48, 37, 24, 11, -3 };

        for(unsigned int i = 0; i < 12; ++i)
        {
            aTime.hour = hour[i];
            t_hord hor = Sun::horizontalPosition(aTime, lat, lon);

            ASSERT_AB(int, azim[i] - 180, hor.azimuth, 1);
            ASSERT_AB(int, alt[i], hor.altitude, 1);
        }
    }
}


void test_moon()
{
    // Test nutation and obliquity.
    {
    // Berlin

    const t_longf lat = _decimal(52, 31, 0);
    const t_longf lon = _decimal(13, 24, 0);

    // Azimuth is interpreted from north from:
    // http://www.sunposition.info/sunposition/spc/locations.php

    const t_aTime aTime(1992, 4, 12); 

    const t_julianDay t(jd(aTime));
    
    ASSERT_AB(long double,  97.643514, Sun::meanAnomaly(t),     0.000001);

    ASSERT_AB(long double, 134.290186, Moon::meanLongitude(t),  0.000001);
    ASSERT_AB(long double,   5.150839, Moon::meanAnomaly(t),    0.000001);
    ASSERT_AB(long double, 113.842309, Moon::meanElongation(t), 0.000001);
    ASSERT_AB(long double, 219.889726, Moon::meanLatitude(t),   0.000001);

    t_ecld ecl = Moon::position(t);

    ASSERT_AB(long double, 133.167269, ecl.longitude, 0.0001);
    ASSERT_AB(long double,  -3.229127, ecl.latitude,  0.0001);

    ASSERT_AB(long double, 368409.7, Moon::distance(t), 0.02);

    ASSERT_AB(long double, _decimal(0, 0, -3.788), Earth::longitudeNutation(
        jd(t_aTime(1987, 4, 10))), 0.0002);
    ASSERT_AB(long double, _decimal(0, 0, +9.443), Earth::obliquityNutation(
        jd(t_aTime(1987, 4, 10))), 0.0002);
    }

    // Lunar perigee and apogee, values from: http://en.wikipedia.org/wiki/File:Lunar_perigee_apogee.png
    {
    const t_aTime aTime1(2007, 10, 26);
    const t_aTime aTime2(2007,  4,  3);

    const t_julianDay t1(jd(aTime1));
    const t_julianDay t2(jd(aTime2));

    ASSERT_AB(long double, Earth::apparentAngularMoonDiameter(t1) 
        / Earth::apparentAngularMoonDiameter(t2), 1.13, 0.01);
    }


    // Test optical librations, parallactic angle and rotation axis angle
    // values gathered from http://www.jgiesen.de/moonlibration/index.htm
    {
    const t_aTime aTime(1992, 04, 12);
    const t_julianDay t(jd(aTime));

    t_longf l = 0.0, b = 0.0;
    Moon::opticalLibrations(t, l, b);

    ASSERT_AB(long double, l, -1.206, 0.001);
    ASSERT_AB(long double, b, +4.194, 0.001);

    ASSERT_AB(long double, Moon::parallacticAngle(aTime, 52.51, 13.41), 38.6, 0.5);
    ASSERT_AB(long double, Moon::positionAngleOfAxis(t), 15.08, 0.2);
    }

    {
    const t_aTime aTime(2040, 06, 20, 17, 16, 00);
    const t_julianDay t(jd(aTime));

    t_longf l = 0.0, b = 0.0;
    Moon::opticalLibrations(t, l, b);

    ASSERT_AB(long double, l, -7.09, 0.02);
    ASSERT_AB(long double, b, -3.28, 0.02);

    ASSERT_AB(long double, Moon::parallacticAngle(aTime, 12.51, 41.41), -7.4, 0.5);
    ASSERT_AB(long double, Moon::positionAngleOfAxis(t), 18.3, 0.2);
    }
}


void test_stars()
{
    const t_julianDay t(jd(t_aTime(1978, 1, 1)));

    const t_equd equ = Stars::apparentPosition(t
        , _decimal(10, 8, 22.3), _decimal(11, 58,  2)
        , _arcsecs(_decimal(0, 0, 0.248)), _decimal(0, 0, 0.006));
}


void test_earth()
{
    ASSERT_AB(long double, Earth::viewDistanceWithinAtmosphere( 1.0)
        , Earth::atmosphereThickness(), 1.e-6);
    ASSERT_AB(long double, Earth::viewDistanceWithinAtmosphere( 0.5)
        ,    15.95805877, 1.e-6);
    ASSERT_AB(long double, Earth::viewDistanceWithinAtmosphere( 0.1)
        ,    75.51480621, 1.e-6);
    ASSERT_AB(long double, Earth::viewDistanceWithinAtmosphere( 0.0)
        ,   319.2545255,  1.e-6);
    ASSERT_AB(long double, Earth::viewDistanceWithinAtmosphere(-0.5)
        ,  6386.958059,   1.e-6);
    ASSERT_AB(long double, Earth::viewDistanceWithinAtmosphere(-1.0)
        , Earth::meanRadius() * 2 + Earth::atmosphereThickness(), 1.e-6);
}