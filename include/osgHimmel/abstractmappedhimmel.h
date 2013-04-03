
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
#ifndef __ABSTRACTMAPPEDHIMMEL_H__
#define __ABSTRACTMAPPEDHIMMEL_H__

#include "declspec.h"
#include "abstracthimmel.h"
#include "twounitschanger.h"
#include "pragmanote.h"
#include "himmelquad.h"

#ifdef OSGHIMMEL_EXPORTS
#include <osg/MatrixTransform>
#else // OSGHIMMEL_EXPORTS
namespace osg
{
    class MatrixTransform;
}
#endif // OSGHIMMEL_EXPORTS


namespace osgHimmel
{

class TimeF;


class OSGH_API AbstractMappedHimmel : public AbstractHimmel
{
public:

    enum e_RazDirection
    {
        // RD_NorthEastSouthWest and RD_NorthWestSouthEast were used to avoid misinterpretations of CW and CCW.

        RD_NorthWestSouthEast
    ,   RD_NorthEastSouthWest
    };

public:

    AbstractMappedHimmel(const bool fakeSun = false);
    virtual ~AbstractMappedHimmel();


    // When assigning textures to an instance of this class, a TimeF has to be 
    // specified, that is used for internal keypoints. Keypoints define point 
    // in times where one texture gets replaced (abruptly or by smooth fading) 
    // by the following one. The transition duration can be specified in 
    // floating time and should be smaller than the minimum time distance between 
    // two adjacent keypoints.

    // These calls get redirected to a TwoUnitsChanger instance (see comment there).

    void setTransitionDuration(const float duration);
    const float getTransitionDuration() const;

    // This call gets redirected to a TwoUnitsChanger instance  (see comment there).
    void pushTextureUnit(
        const GLint textureUnit
    ,   const float time = 1.f);


    // Rotation around Zenith (RaZ), if used discreetly, can greatly influence the 
    // dynamic of a scene, by slightly rotating the given texture mapped himmel 
    // around the up vector. It accesses its own TimeF object and thereby remains 
    // independent from the himmels timef.

    void setSecondsPerRAZ(const float secondsPerRAZ); // reasonable values should be around 2000+
    const float getSecondsPerRAZ() const;

    void setRazDirection(const e_RazDirection razDirection);
    const e_RazDirection getRazDirection() const;


    const float setSunScale(const float scale);
    const float getSunScale() const;

    const osg::Vec4f setSunCoeffs(const osg::Vec4f &coeffs);
    const osg::Vec4f getSunCoeffs() const;
    static const osg::Vec4f defaultSunCoeffs();


protected:

    // Inherited by AbstractHimmel. Call this first when inherited!
    virtual void update();

    void assignUnit(
        const GLint textureUnit
    ,   const GLint targetIndex);

    // For convenience in subclasses.
    void assignBackUnit(const GLint textureUnit);
    void assignSrcUnit(const GLint textureUnit);

    const float updateSrcAlpha() const; // Accesses TwoUnitsChanger.

    // Shader

    void setupProgram(osg::StateSet *stateSet);
    void setupUniforms(osg::StateSet *stateSet);

    void makeVertexShader();
    void unmakeVertexShader();

    void makeFragmentShader();
    void unmakeFragmentShader();

    void reassignShader();

    // abstract interface

    virtual const std::string getVertexShaderSource();
    virtual const std::string getFragmentShaderSource() = 0;

    // getter

    inline osg::Program &program()
    {
        return *m_program;
    }


    // Interface

    virtual osg::StateAttribute *getTextureAttribute(const GLint textureUnit) const = 0;

    // AbstractHimmel

    virtual void postInitialize();

protected:

    osg::ref_ptr<HimmelQuad> m_hquad;

    TwoUnitsChanger m_changer;

    // shader

    osg::Program *m_program;

    osg::Shader *m_vShader;
    osg::Shader *m_fShader;

    // uniforms

    osg::ref_ptr<osg::Uniform> u_back; // type depends on subclasses
    osg::ref_ptr<osg::Uniform> u_src;  // type depends on subclasses

    osg::ref_ptr<osg::Uniform> u_razInverse; // inverse of the m_razTransform
    osg::ref_ptr<osg::Uniform> u_sun;
    osg::ref_ptr<osg::Uniform> u_sunCoeffs;
    osg::ref_ptr<osg::Uniform> u_sunScale;

    osg::ref_ptr<osg::Uniform> u_srcAlpha; // float


    GLint m_activeBackUnit;
    GLint m_activeSrcUnit;

    osg::ref_ptr<osg::MatrixTransform> m_razTransform;
    e_RazDirection m_razDirection;

    TimeF *m_razTimef;    

    const bool m_fakeSun;


#ifdef OSGHIMMEL_EXPOSE_SHADERS
public:
    osg::Shader *getVertexShader();
    osg::Shader *getGeometryShader();
    osg::Shader *getFragmentShader();
#endif // OSGHIMMEL_EXPOSE_SHADERS
};

} // namespace osgHimmel

#endif // __ABSTRACTMAPPEDHIMMEL_H__