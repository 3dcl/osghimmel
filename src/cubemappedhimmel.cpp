
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

#include "cubemappedhimmel.h"

#include "shaderfragment/common.h"
#include "shaderfragment/blend_normal.h"
#include "shaderfragment/fakesun.h"

#include <osg/TextureCubeMap>


namespace osgHimmel
{

CubeMappedHimmel::CubeMappedHimmel(
    const bool fakeSun
,   const bool cubify)
:   AbstractMappedHimmel(fakeSun)
,   m_cubify(cubify)
{
    setName("CubeMappedHimmel");
};


CubeMappedHimmel::~CubeMappedHimmel()
{
};


osg::TextureCubeMap* CubeMappedHimmel::getOrCreateTextureCubeMap(const GLint textureUnit)
{
    // Retrieve an existing texture.

    const t_tcmById::iterator existingTCM(m_tcmsById.find(textureUnit));
    if(existingTCM != m_tcmsById.end())
        return existingTCM->second;

    // Create and configure new texture object.

    osg::ref_ptr<osg::TextureCubeMap> newTCM(new osg::TextureCubeMap);

    newTCM->setUnRefImageDataAfterApply(true);

    newTCM->setInternalFormat(GL_RGBA);

    newTCM->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    newTCM->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    newTCM->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

    newTCM->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    newTCM->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    m_tcmsById[textureUnit] = newTCM;

    // Assign some textures if there are none.

    if(m_tcmsById.size() == 1)
        assignBackUnit(textureUnit);
    if(m_tcmsById.size() == 2)
        assignSrcUnit(textureUnit);

    return newTCM;
}


osg::StateAttribute *CubeMappedHimmel::getTextureAttribute(const GLint textureUnit) const
{
    const t_tcmById::const_iterator tex2Di(m_tcmsById.find(textureUnit));
    return tex2Di == m_tcmsById.end() ? NULL : tex2Di->second;
}


void CubeMappedHimmel::setCubify(const bool cubify)
{
    if(cubify == m_cubify)
        return;

    m_cubify = cubify;
    reassignShader();
}


const bool CubeMappedHimmel::isCubified() const
{
    return m_cubify;
}




const std::string CubeMappedHimmel::getFragmentShaderSource()
{
    return glsl_version_150()

//  +   glsl_f_blendNormalExt // using mix

    +   (m_fakeSun ? glsl_fakesun() : "")

        // This is based on the math presented on: http://petrocket.blogspot.de/2010/04/sphere-to-cube-mapping.html
        // It maps a sphere to a cube and can be used to overcome the texel per samplingfield-angle issue of cube mapping.

    +   (!m_cubify ? "" :

        PRAGMA_ONCE(sphere2cube,

        "const float isqrt2 = 0.70710678118654752440084436210485;\n"
        "\n"
        "vec3 cubify(const in vec3 s)\n"
        "{\n"
        "	 float xx2 = s.x * s.x * 2.0;\n"
        "	 float yy2 = s.y * s.y * 2.0;\n"
        "\n"
        "    vec2 v = vec2(xx2 - yy2, yy2 - xx2);\n"
        "\n"
        "    float ii = v.y - 3.0;\n"
        "    ii *= ii;\n"
        "\n"
        "    float isqrt = -sqrt(ii - 12.0 * xx2) + 3.0;\n"
        "\n"
        "    v  = sqrt(v + isqrt);\n"
        "    v *= isqrt2;\n"
        "\n"
        "    return sign(s) * vec3(v, 1.0);\n"
        "}\n"
        "\n"
        "vec3 sphere2cube(const in vec3 sphere)\n"
        "{\n"
        "    vec3 f = abs(sphere);\n"
        "\n"
        "    bool a = f.y >= f.x && f.y >= f.z;\n"
        "    bool b = f.x >= f.z;\n"
        "\n"
        "    return a ? cubify(sphere.xzy).xzy : b ? cubify(sphere.yzx).zxy : cubify(sphere);\n"
        "}"))

    +   PRAGMA_ONCE(main,

        "in vec4 m_ray;\n"
        "\n"
        // From AbstractMappedHimmel

        "uniform float srcAlpha;\n"
        "\n"
        "uniform samplerCube back;\n"
        "uniform samplerCube src;\n"
        "\n"
        // Color Retrieval

        "void main(void)\n"
        "{\n"
        "    vec3 stu = normalize(m_ray.xyz);\n"
    +   (m_cubify ? "    stu = sphere2cube(stu);\n" : "")
    +   "\n"
        "    vec4 fc = mix(\n"
        "        textureCube(back, stu), textureCube(src, stu), srcAlpha);\n"
        "\n"
        "    gl_FragColor = " + (m_fakeSun ? "fc + fakeSun(fc.a)" : "fc") + ";\n"
        "}");
}

} // namespace osgHimmel