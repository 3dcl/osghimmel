
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

#include "paraboloidmappedhimmel.h"

#include "horizonband.h"

#include "shaderfragment/common.h"
#include "shaderfragment/blend_normal.h"
#include "shaderfragment/hband.h"
#include "shaderfragment/fakesun.h"

#include <osg/Texture2D>


namespace osgHimmel
{

ParaboloidMappedHimmel::ParaboloidMappedHimmel(
    const bool horizonBand
,   const bool fakeSun)
:   AbstractMappedHimmel(fakeSun)
,   m_hBand(NULL)
,   m_withHBand(horizonBand)
{
    setName("ParaboloidMappedHimmel");

    if(m_withHBand)
    {
        m_hBand = new HorizonBand();
        m_hBand->initialize(getOrCreateStateSet());
    }
};


ParaboloidMappedHimmel::~ParaboloidMappedHimmel()
{
};


HorizonBand *ParaboloidMappedHimmel::hBand()
{
    return m_hBand;
}


osg::Texture2D* ParaboloidMappedHimmel::getOrCreateTexture2D(const GLint textureUnit)
{
    // Retrieve an existing texture.

    const t_tex2DById::iterator existingTex2D(m_tex2DsById.find(textureUnit));
    if(existingTex2D != m_tex2DsById.end())
        return existingTex2D->second;


    // Create and configure new texture object.

    osg::ref_ptr<osg::Texture2D> newTex2D(new osg::Texture2D);

    newTex2D->setUnRefImageDataAfterApply(true);

    newTex2D->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    newTex2D->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

    newTex2D->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    newTex2D->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    m_tex2DsById[textureUnit] = newTex2D;

    // Assign some textures if there are none.

    if(m_tex2DsById.size() == 1)
        assignBackUnit(textureUnit);
    if(m_tex2DsById.size() == 2)
        assignSrcUnit(textureUnit);

    return newTex2D;
}


osg::StateAttribute *ParaboloidMappedHimmel::getTextureAttribute(const GLint textureUnit) const
{
    const t_tex2DById::const_iterator tex2Di(m_tex2DsById.find(textureUnit));
    return tex2Di == m_tex2DsById.end() ? NULL : tex2Di->second;
}




const std::string ParaboloidMappedHimmel::getFragmentShaderSource()
{
    return glsl_version_150()

    +   glsl_blendNormalExt()
    
    +   glsl_fakesun()
    +   glsl_hband() 

    +   PRAGMA_ONCE(main,

        ENABLE_IF(hBand, m_hBand)
        ENABLE_IF(fakeSun, m_fakeSun)

        "in vec4 m_ray;\n"
        "\n"
        // From AbstractMappedHimmel

        "uniform float srcAlpha;\n"
        "\n"
        "uniform sampler2D back;\n"
        "uniform sampler2D src;\n"
        "\n"
        // Color Retrieval


        // TODO: encapsulate
        IF_ENABLED(hBand,

        "uniform vec3 hbandParams;\n"
        "uniform vec4 hbandBackground;\n"
        "uniform vec4 hbandColor;")

        // TODO: encapsulate
        IF_ENABLED(fakeSun,

        "uniform mat4 razInverse;\n"
        "uniform vec3 sun;\n"
        "uniform vec4 sunCoeffs;\n"
        "uniform float sunScale;")


        "void main(void)\n"
        "{\n"
        "    vec3 stu = normalize(m_ray.xyz);\n"
        "\n"
        "    float m = 0.5 / (1.0 + stu.z);\n"
        "    vec2 uv = vec2(stu.x, stu.y) * m + 0.5;\n"
        "\n"
        "    vec4 fc = mix(texture2D(back, uv), texture2D(src, uv), srcAlpha);\n"

        IF_ELSE_ENABLED(hBand, "", "    if(stu.z < 0.0) discard;\n")

        "\n"

        IF_ENABLED(fakeSun,

        "    fc += fakeSun(\n"
        "        normalize(m_razInvariant.xyz)\n"
        "    ,   sun\n"
        "    ,   sunCoeffs\n"
        "    ,   sunScale\n"
        "    ,   fc.a);")

        IF_ELSE_ENABLED(hBand,

        "    gl_FragColor = hband(\n"
        "        stu.z\n"
        "    ,   hbandParams[0]\n"
        "    ,   hbandParams[1]\n"
        "    ,   hbandParams[2]\n"
        "    ,   hbandColor\n"
        "    ,   hbandBackground\n"
        "    ,   fc);"
        ,
        "    gl_FragColor = fc;")

        "}");
}

} // namespace osgHimmel