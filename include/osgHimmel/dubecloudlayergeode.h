
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
#ifndef __DUBECLOUDLAYERGEODE_H__
#define __DUBECLOUDLAYERGEODE_H__

#include "declspec.h"

#include <osg/Group>


namespace osg
{
    class Image;
    class Texture2D;
    class Texture3D;
}

namespace osgHimmel
{

class Himmel;
class HimmelQuad;


class OSGH_API DubeCloudLayerGeode : public osg::Group
{
public:

    DubeCloudLayerGeode(const int texSize = 2048);
    virtual ~DubeCloudLayerGeode();

    void update(const Himmel &himmel);



    const float setCoverage(const float coverage);
    const float getCoverage() const;

    const float setSharpness(const float sharpness);
    const float getSharpness() const;

    const float setAltitude(const float altitude);
    const float getAltitude() const;
    static const float defaultAltitude();

    const osg::Vec2f setScale(const osg::Vec2f &scale);
    const osg::Vec2f getScale() const;
    static const osg::Vec2f defaultScale();

    const float setChange(const float change);
    const float getChange() const;
    static const float defaultChange();

    const float setThickness(const float thickness);
    const float getThickness() const;

    const float setOffset(const float offset);
    const float getOffset() const;

    const osg::Vec3f setBottomColor(const osg::Vec3f &color);
    const osg::Vec3f getBottomColor() const;

    const osg::Vec3f setTopColor(const osg::Vec3f &color);
    const osg::Vec3f getTopColor() const;

    const osg::Vec2f setWind(const osg::Vec2f &wind);
    const osg::Vec2f getWind() const;

protected:

    virtual void setupUniforms(osg::StateSet* stateSet);
    virtual void setupNode    (osg::StateSet* stateSet);
    virtual void setupTextures(osg::StateSet* stateSet);
    virtual void setupShader  (osg::StateSet* stateSet);

    virtual const std::string getVertexShaderSource();
    virtual const std::string getFragmentShaderSource();

protected:

    HimmelQuad *m_hquad;
    
    osg::Texture2D *m_preNoise;
    osg::Texture3D *m_noise[4];
        
    int m_noiseSize;

    osg::Program *m_program;
    osg::Shader *m_vShader;
    osg::Shader *m_fShader;

    osg::ref_ptr<osg::Uniform> u_q;
    osg::ref_ptr<osg::Uniform> u_clouds;
    osg::ref_ptr<osg::Uniform> u_noise;

    osg::ref_ptr<osg::Uniform> u_time;

    osg::ref_ptr<osg::Uniform> u_noise0;
    osg::ref_ptr<osg::Uniform> u_noise1;
    osg::ref_ptr<osg::Uniform> u_noise2;
    osg::ref_ptr<osg::Uniform> u_noise3;
    
    osg::ref_ptr<osg::Uniform> u_coverage;
    osg::ref_ptr<osg::Uniform> u_sharpness;
    osg::ref_ptr<osg::Uniform> u_change;
    osg::ref_ptr<osg::Uniform> u_wind;
    osg::ref_ptr<osg::Uniform> u_altitude;
    osg::ref_ptr<osg::Uniform> u_scale;
    osg::ref_ptr<osg::Uniform> u_bcolor;
    osg::ref_ptr<osg::Uniform> u_tcolor;
    osg::ref_ptr<osg::Uniform> u_offset;
    osg::ref_ptr<osg::Uniform> u_thickness;


#ifdef OSGHIMMEL_EXPOSE_SHADERS
public:
    osg::Shader *getVertexShader();
    osg::Shader *getGeometryShader();
    osg::Shader *getFragmentShader();
#endif // OSGHIMMEL_EXPOSE_SHADERS
};

} // namespace osgHimmel

#endif // __DUBECLOUDLAYERGEODE_H__