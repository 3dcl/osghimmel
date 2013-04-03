
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

#include "bruneton_irradiance.h"

#include "bruneton_common.h"
#include "common.h"


namespace osgHimmel
{

// based on Brunetons free code (http://www-evasion.imag.fr/Members/Eric.Bruneton/PrecomputedAtmosphericScattering2.zip)

/**
    * Precomputed Atmospheric Scattering
    * Copyright (c) 2008 INRIA
    * All rights reserved.
    *
    * Redistribution and use in source and binary forms, with or without
    * modification, are permitted provided that the following conditions
    * are met:
    * 1. Redistributions of source code must retain the above copyright
    *    notice, this list of conditions and the following disclaimer.
    * 2. Redistributions in binary form must reproduce the above copyright
    *    notice, this list of conditions and the following disclaimer in the
    *    documentation and/or other materials provided with the distribution.
    * 3. Neither the name of the copyright holders nor the names of its
    *    contributors may be used to endorse or promote products derived from
    *    this software without specific prior written permission.
    *
    * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
    * THE POSSIBILITY OF SUCH DAMAGE.
    */

/**
    * Author: Eric Bruneton
    */

// computes ground irradiance due to direct sunlight E[L0] (line 2 in algorithm 4.1)

const std::string glsl_bruneton_f_irradiance1() // requires: transmittanceSampler
{
    static const std::string source(

        glsl_cmn_uniform()
    +   glsl_bruneton_const_ISize()

    +   glsl_bruneton_irradianceRMuS()
    +   glsl_bruneton_transmittanceUV()
    +   glsl_bruneton_transmittance()

    +   PRAGMA_ONCE(main,

        "void main() {\n"
        "    float r, muS;\n"
        "    getIrradianceRMuS(r, muS);\n"
        "    gl_FragColor = vec4(transmittance(r, muS) * max(muS, 0.0), 0.0);\n"
        "}"));

    return source;
};

    
// computes ground irradiance due to skylight E[deltaS] (line 8 in algorithm 4.1)

const std::string glsl_bruneton_f_irradianceN() // requires: deltaSRSampler, deltaSMSampler, first
{
    static const std::string source(

        glsl_bruneton_const_PI()
    +   glsl_bruneton_const_Samples()
    +   glsl_bruneton_const_R()
    +   glsl_bruneton_const_M()
    +   glsl_cmn_uniform()
    +   glsl_bruneton_const_RSize()
    +   glsl_bruneton_const_ISize()

    +   glsl_bruneton_irradianceRMuS()
    +   glsl_bruneton_phaseFunctionR()
    +   glsl_bruneton_phaseFunctionM()
    +   glsl_bruneton_texture4D()

    +   PRAGMA_ONCE(main,

        "uniform sampler3D deltaSRSampler;\n"
        "uniform sampler3D deltaSMSampler;\n"
        "uniform float first;\n"
        "\n"
        "const float dphi = PI / float(IRRADIANCE_INTEGRAL_SAMPLES);\n"
        "const float dtheta = PI / float(IRRADIANCE_INTEGRAL_SAMPLES);\n"
        "\n"
        "void main() {\n"
        "    float r, muS;\n"
        "    getIrradianceRMuS(r, muS);\n"
        "    vec3 s = vec3(max(sqrt(1.0 - muS * muS), 0.0), 0.0, muS);\n"
        "\n"
        "    vec3 result = vec3(0.0);\n"
            // integral over 2.PI around x with two nested loops over w directions (theta,phi) -- Eq (15)
        "    for (int iphi = 0; iphi < 2 * IRRADIANCE_INTEGRAL_SAMPLES; ++iphi) {\n"
        "        float phi = (float(iphi) + 0.5) * dphi;\n"
        "        for (int itheta = 0; itheta < IRRADIANCE_INTEGRAL_SAMPLES / 2; ++itheta) {\n"
        "            float theta = (float(itheta) + 0.5) * dtheta;\n"
        "            float dw = dtheta * dphi * sin(theta);\n"
        "            vec3 w = vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));\n"
        "            float nu = dot(s, w);\n"
        "            if (first == 1.0) {\n"
                        // first iteration is special because Rayleigh and Mie were stored separately,
                        // without the phase functions factors; they must be reintroduced here
        "                float pr1 = phaseFunctionR(nu);\n"
        "                float pm1 = phaseFunctionM(nu);\n"
        "                vec3 ray1 = texture4D(deltaSRSampler, r, w.z, muS, nu).rgb;\n"
        "                vec3 mie1 = texture4D(deltaSMSampler, r, w.z, muS, nu).rgb;\n"
        "                result += (ray1 * pr1 + mie1 * pm1) * w.z * dw;\n"
        "            } else {\n"
        "                result += texture4D(deltaSRSampler, r, w.z, muS, nu).rgb * w.z * dw;\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "\n"
        "    gl_FragColor = vec4(result, 0.0);\n"
        "}"));

    return source;
};


// copies deltaE into E (lines 4 and 10 in algorithm 4.1)

const std::string glsl_bruneton_f_copyIrradiance() // requires: k, deltaESampler, irradianceSampler
{
    static const std::string source(

        glsl_bruneton_const_ISize()
    +
    +   PRAGMA_ONCE(main,

        "uniform float k;\n" // k = 0 for line 4, k = 1 for line 10
        "uniform sampler2D deltaESampler;\n"
        "uniform sampler2D irradianceSampler;\n"
        "\n"
        "void main() {\n"
        "    vec2 uv = gl_FragCoord.xy / vec2(SKY_W, SKY_H);\n"
        "    gl_FragColor  = texture2D(irradianceSampler, uv);\n"
        "    gl_FragColor += k * texture2D(deltaESampler, uv);\n" // k = 0 for line 4, k = 1 for line 10
        "}"));

    return source;
};

} // namespace osgHimmel