
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

#include "scene_cubemappedhimmel.h"

#include "utils/tr.h"
#include "utils/shadermodifier.h"

#include "osgHimmel/cubemappedhimmel.h"

#include <osg/TextureCubeMap>
#include <osgDB/ReadFile>


namespace
{
    // Properties

    const QString GROUP_CUBEMAPPED (TR("Cube Mapped"));

    const QString PROPERTY_RAZSPEED(TR("RAZ Speed"));
    const QString PROPERTY_CUBIFY(TR("Cubify Mapping"));
}

using namespace osgHimmel;


Scene_CubeMappedHimmel::Scene_CubeMappedHimmel(osg::Camera *camera)
:   AbstractHimmelScene(camera)
,   m_himmel(NULL)
{
    initializeProperties();

    m_himmel = new CubeMappedHimmel();

    m_himmel->setTransitionDuration(0.05f);


    std::string name[] = { "0", "1", "2", "3" };
    osg::TextureCubeMap *tcm[4];

    const int n = 4;
    for(int i = 0; i < n; ++i)
    {
        tcm[i] = m_himmel->getOrCreateTextureCubeMap(i);

        tcm[i]->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_px.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_nx.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_py.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_ny.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_pz.jpg"));
        tcm[i]->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("resources/cube_gen_" + name[i] + "_nz.jpg"));

        m_himmel->pushTextureUnit(i, (i * 1.f) / (n * 1.f));
    }

    addChild(m_himmel);
}


Scene_CubeMappedHimmel::~Scene_CubeMappedHimmel()
{
}


AbstractHimmel *Scene_CubeMappedHimmel::himmel()
{
    return m_himmel;
}


void Scene_CubeMappedHimmel::postInitialize()
{
    shaderModifier()->registerShader(m_himmel->getName(), m_himmel->getVertexShader());
    shaderModifier()->registerShader(m_himmel->getName(), m_himmel->getGeometryShader());
    shaderModifier()->registerShader(m_himmel->getName(), m_himmel->getFragmentShader());
}


void Scene_CubeMappedHimmel::registerProperties()
{
    AbstractHimmelScene::registerProperties();

    QtProperty *cubeGroup = createGroup(GROUP_CUBEMAPPED);

    createProperty(*cubeGroup, PROPERTY_RAZSPEED, 0.0, -99999.0, 99999.0, 10.0); 
    createProperty(*cubeGroup, PROPERTY_CUBIFY, false);
}


void Scene_CubeMappedHimmel::propertyChanged(
    QtProperty *p
,   const QString &name)
{
    if(PROPERTY_RAZSPEED == name)
    {
        const double secondsPerRAZ(doubleValue(PROPERTY_RAZSPEED));

        m_himmel->setSecondsPerRAZ(secondsPerRAZ);
        m_himmel->setRazDirection(secondsPerRAZ < 0 ? 
            AbstractMappedHimmel::RD_NorthEastSouthWest : AbstractMappedHimmel::RD_NorthWestSouthEast);
    }
    else if(PROPERTY_CUBIFY == name)
    {
        m_himmel->setCubify(!m_himmel->isCubified());
    }
}
