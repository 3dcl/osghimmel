
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

#include "scattering.h"

#include "common.h"


namespace osgHimmel
{

const std::string glsl_scattering()
{
    static const std::string source(

        glsl_cmn_uniform()

    +   PRAGMA_ONCE(scattering,
        
        // coefficients are taken from calculateions by means of Bucholtz 94
        // -> Scattering coefficient for air molecules (Rayleigh theory) for wavelengths (680, 550, 440) nm

        "const vec3 lambda = vec3(0.52, 1.22, 2.98);\n"
        "\n"

        "float optical(const float theta)\n"
        "{\n"
        "    float sin_theta = sin(theta);\n"
        "\n"
        "    float s = -sin(asin((cmn[1] + cmn[0]) / cmn[2] * sin_theta) - theta);\n"
        "    s *= cmn[2];\n"
        "    s /= sin_theta;\n"
        "    s /= cmn[2] - cmn[0];\n"
        "\n"
        "    return s;\n"
        "}\n"
        "\n"

        // theta is the angle between ray and zenith ~ probably acos(ray.z)
        "vec3 scatt(const float theta)\n"   
        "{\n"
        "    return lambda * optical(theta);\n"
        "}"));

    return source;
};

} // namespace osgHimmel