
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
#ifndef __MOONGEODE_H__
#define __MOONGEODE_H__

#include "declspec.h"

#include <osg/Geode>


namespace osgHimmel
{

class Himmel;
class HimmelQuad;


class OSGH_API MoonGeode : public osg::Geode
{
public:

    MoonGeode(const char* cubeMapFilePath);
    virtual ~MoonGeode();

    void update(const Himmel &himmel);

    // This is used to add this instances uniforms to e.g. moons glare geode.
    void addUniformsToVariousStateSate(osg::StateSet* stateSet);

    const float setScale(const float scale);
    const float getScale() const;
    static const float defaultScale();

    const osg::Vec3f setSunShineColor(const osg::Vec3f &color);
    const osg::Vec3f getSunShineColor() const;
    static const osg::Vec3f defaultSunShineColor();

    const float setSunShineIntensity(const float intensity);
    const float getSunShineIntensity() const;
    static const float defaultSunShineIntensity();

    const osg::Vec3f setEarthShineColor(const osg::Vec3f &color);
    const osg::Vec3f getEarthShineColor() const;
    static const osg::Vec3f defaultEarthShineColor();

    const float setEarthShineIntensity(const float intensity);
    const float getEarthShineIntensity() const;
    static const float defaultEarthShineIntensity();

protected:

    void setupUniforms(osg::StateSet* stateSet);
    void setupNode    (osg::StateSet* stateSet);

    void setupTextures(
        osg::StateSet* stateSet
    ,   const char* cubeMapFilePath);

    void setupMoonTextureCube(
        osg::StateSet* stateSet
    ,   const char* cubeMapFilePath);

    void setupEclipseTexture(osg::StateSet* stateSet);

    void setupShader(osg::StateSet* stateSet);

    const std::string getVertexShaderSource();
    const std::string getFragmentShaderSource();

protected:

    HimmelQuad *m_hquad;

    osg::Program *m_program;
    osg::Shader *m_vShader;
    osg::Shader *m_fShader;

    osg::ref_ptr<osg::Uniform> u_moon;
    osg::ref_ptr<osg::Uniform> u_moonr;

    osg::ref_ptr<osg::Uniform> u_moonCube;

    osg::ref_ptr<osg::Uniform> u_eclCoeffs;
    osg::ref_ptr<osg::Uniform> u_eclParams;

    osg::ref_ptr<osg::Uniform> u_R;
    osg::ref_ptr<osg::Uniform> u_q;
    osg::ref_ptr<osg::Uniform> u_sunShine;
    osg::ref_ptr<osg::Uniform> u_earthShine;

    float m_scale;

    float m_earthShineScale;
    osg::Vec3f m_earthShineColor;


#ifdef OSGHIMMEL_EXPOSE_SHADERS
public:
    osg::Shader *getVertexShader();
    osg::Shader *getGeometryShader();
    osg::Shader *getFragmentShader();
#endif // OSGHIMMEL_EXPOSE_SHADERS
};

} // namespace osgHimmel

#endif // __MOONGEODE_H__