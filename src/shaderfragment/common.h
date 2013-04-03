
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
#ifndef __GLSL_COMMON_H__
#define __GLSL_COMMON_H__

#include <string>

namespace osgHimmel
{

#ifndef PRAGMA_ONCE
#define PRAGMA_ONCE(name, source)  \
    "\n#ifndef __" #name "__\n" \
    "#define __" #name "__\n\n" \
    source \
    "\n\n#endif // __" #name "__\n\n"
#endif // PRAGMA_ONCE


#ifndef ENABLE_IF
#define ENABLE_IF(flag, condition)  \
    "" + ((condition) ? "#define __enable_" #flag "__\n" : "") + ""
#endif // ENABLE_IF

#ifndef IF_ENABLED
#define IF_ENABLED(flag, source)         \
    "\n#ifdef __enable_" #flag "__\n\n"  \
    source                               \
    "\n\n#endif // __enable_" #flag "__\n\n"
#endif // IF_ENABLED

#ifndef IF_ELSE_ENABLED
#define IF_ELSE_ENABLED(flag, if_source, else_source)  \
    "\n#ifdef __enable_" #flag "__\n\n"                \
    if_source                                          \
    "\n\n#else // __enable_" #flag "__\n\n"            \
    else_source                                        \
    "\n\n#endif // __enable_" #flag "__\n\n" 
#endif // IF_ELSE_ENABLED

#ifndef IF_NOT_ENABLED
#define IF_NOT_ENABLED(flag, source)      \
    "\n#ifndef __enable_" #flag "__\n\n"  \
    if_source                             \
    "\n\n#endif // __enable_" #flag "__\n\n"
#endif // IF_NOT_ENABLED
    

const std::string glsl_version_150();
const std::string glsl_geometry_ext();

const std::string glsl_quadRetrieveRay(); // vec4 quadRetrieveRay()
const std::string glsl_quadTransform(); // void quadTransform()

const std::string glsl_cmn_uniform();
const std::string glsl_horizon(); // bool belowHorizon(vec3 ray)

} // namespace osgHimmel

#endif // __GLSL_COMMON_H__