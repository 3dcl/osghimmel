
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

#pragma once
#ifndef __GLSL_COMPOSE_H__
#define __GLSL_COMPOSE_H__

namespace osgHimmel
{

// The general composition of two colors is specified in these two macros:
// The first (default) uses the alpha value of src, the other allows over-
// writing it. The Blending function is specified via COMPOSE parameter.

// e.g. IMP_COMPOSE(multiply, b * s) creates the blend_multiply function
// that uses back * src as intermediate composition.


#ifndef IMP_COMPOSE
#define IMP_COMPOSE(MODE, COMPOSE)                                    \
                                                                      \
    "vec4 blend_" #MODE "(vec4 back, vec4 src)\n"                     \
    "{\n"                                                             \
    "    vec3 b = back.rgb;\n"                                        \
    "    vec3 s = src.rgb;\n"                                         \
    "\n"                                                              \
    "    vec3 bs = " #COMPOSE ";\n"                                   \
    "\n"                                                              \
    "    float ba = back.a;\n"                                        \
    "    float sa = src.a;\n"                                         \
    "\n"                                                              \
    "    float ra = ba + sa - (ba * sa);\n"                           \
    "\n"                                                              \
    "    vec3 r = (1.0 - sa / ra) * b + (sa / ra) * ((1.0 - ba) * s + ba * bs);\n" \
    "\n"                                                              \
    "    return vec4(r, ra);\n"                                       \
    "}"
#endif // IMP_COMPOSE


#ifndef IMP_COMPOSE_SRC_ALPHA
#define IMP_COMPOSE_SRC_ALPHA(MODE, COMPOSE)                          \
                                                                      \
    "vec4 blend_" #MODE "(vec4 back, vec4 src, float srca)\n"         \
    "{\n"                                                             \
    "    vec3 b = back.rgb;\n"                                        \
    "    vec3 s = src.rgb;\n"                                         \
    "\n"                                                              \
    "    vec3 bs = " #COMPOSE ";\n"                                   \
    "\n"                                                              \
    "    float ba = back.a;\n"                                        \
    "    float sa = clamp(srca, 0.0, 1.0);\n"                         \
    "\n"                                                              \
    "    float ra = ba + sa - (ba * sa);\n"                           \
    "\n"                                                              \
    "    vec3 r = (1.0 - sa / ra) * b + (sa / ra) * ((1.0 - ba) * s + ba * bs);\n" \
    "\n"                                                              \
    "    return vec4(r, ra);\n"                                       \
    "}"
#endif // IMP_COMPOSE_SRC_ALPHA

} // namespace osgHimmel

#endif // __GLSL_COMPOSE_H__