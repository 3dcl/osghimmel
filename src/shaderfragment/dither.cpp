
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

#include "dither.h"

#include "common.h"
#include "pseudo_rand.h"

namespace osgHimmel
{

// The dithering mainly relies on a frame-to-frame coherent pseudo 
// random number, described in pseudo_rand.fsf. The rand value used 
// non-uniformly for all 4 color channels and modulated by some 
// primes. The 0.0001 weight is good for default appliance (change 
// it as required). Depends on strength of color banding and the 
// displays color scheme and settings. 
// The exact value of most of the used constant values where gathered 
// by trial and error and benchmarked by subjective perception.

const std::string glsl_dither()
{
    static const std::string source(

        glsl_pseudo_rand()

    +   PRAGMA_ONCE(dither,

        "vec4 dither(\n"
        "    float multiplier\n"
        ",   int seed)\n"
        "{\n"
        "    float r = pseudo_rand(gl_FragCoord.xy, seed);\n"
        "    uvec4 v = uint(r * 3571) * uvec4(67, 89, 23, 71);\n"
        "\n"
        // A ditheringMultiplier of 1 will add frame to frame coherent noise for each pixel of about +-1.
        // The average brightness of the rendering will roughly remain unchanged.
        "    return (vec4(v % uvec4(853)) - 241 - multiplier * 1.41) * 0.00001 * multiplier;\n"
        "}"));

    return source;
};



} // namespace osgHimmel