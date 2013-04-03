
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

#include "cloudlayer.h"

#include "common.h"


namespace osgHimmel
{

// Intersection of view ray (d) with a sphere of radius = mean earth 
// radius + altitude (altitude). Support is only for rays starting 
// below the cloud layer (o must be inside the sphere...).

// (http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection) and
// (http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm)

const std::string glsl_cloud_layer_intersection_or_discard()
{
    static const std::string source(

        PRAGMA_ONCE(layerIntersectionOrDiscard,

        "float layerIntersectionOrDiscard(\n"
        "    const vec3 d\n"
        ",   const float altitude)\n"
        "{\n"
        "    vec3  o = vec3(0.0, 0.0, cmn[1] + cmn[0]);\n"
        "    float r = cmn[1] + altitude;\n"
        "\n"
            // for now, ignore if altitude is above cloud layer
        "    if(o.z > r) \n"
        "        discard;\n"
        "\n"
        "    float a = dot(d, d);\n"
        "    float b = 2 * dot(d, o);\n"
        "    float c = dot(o, o) - r * r;\n"
        "\n"
        "    float B = b * b - 4 * a * c;\n"
        "    if(B < 0)\n"
        "        discard;\n"
        "\n"
        "    B = sqrt(B);\n"
        "\n"
        "    return (-b + B) * 0.5 / a;\n"
        "}"));

    return source;
};


const std::string glsl_cloud_layer_intersection()
{
    static const std::string source(

        PRAGMA_ONCE(layerIntersection,

        "bool layerIntersection(\n"
        "    const vec3 d\n"
        ",   const vec3 o\n"
        ",   const float altitude\n"
        ",   out float t)\n"
        "{\n"
        "    float r = cmn[1] + altitude;\n"
        "\n"
            // for now, ignore if altitude is above cloud layer
        "    if(o.z > r) \n"
        "        return false;\n"
        "\n"
        "    float a = dot(d, d);\n"
        "    float b = 2 * dot(d, o);\n"
        "    float c = dot(o, o) - r * r;\n"
        "\n"
        "    float B = b * b - 4 * a * c;\n"
        "    B = sqrt(B);\n"
        "\n"
        "    float q;\n"
        "\n"
        "    if(b < 0)\n"
        "        q = (-b - B) * 0.5;\n"
        "    else\n"
        "        q = (-b + B) * 0.5;\n"
        "\n"
        "    float t0 = q / a;\n"
        "    float t1 = c / q;\n"
        "\n"
        "    if(t0 > t1)\n"
        "    {\n"
        "        q  = t0;\n"
        "        t0 = t1;\n"
        "        t1 = q ;\n"
        "    }\n"
        "\n"
        "    if(t1 < 0)\n"
        "        return false;\n"
        "\n"
        "    t = t0 < 0 ? t1 : t0;\n"
        "    return true;\n"
        "}"));

    return source;
};

} // namespace osgHimmel