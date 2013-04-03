
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


#include "test_twounitschanger.h"

#include "test.h"

#include "osgHimmel/twounitschanger.h"


using namespace osgHimmel;

void test_twounitschanger()
{
    TwoUnitsChanger tuc;

    // Test case shown in documentation on wiki.

    tuc.setTransitionDuration(0.4f);
    tuc.pushUnit(0, 0.0f);
    tuc.pushUnit(1, 0.5f);

    ASSERT_EQ(int, 0, tuc.getSrcUnit(0.0f));
    ASSERT_EQ(int, 0, tuc.getSrcUnit(0.8f));
    ASSERT_EQ(int, 1, tuc.getSrcUnit(0.2f));

    ASSERT_EQ(int, 1, tuc.getSrcUnit(0.5f));
    ASSERT_EQ(int, 1, tuc.getSrcUnit(0.3f));
    ASSERT_EQ(int, 0, tuc.getSrcUnit(0.7f));

    ASSERT_AB(float, 0.00, tuc.getSrcAlpha(0.8f), 0.0001);
    ASSERT_AB(float, 0.25, tuc.getSrcAlpha(0.9f), 0.0001);
    ASSERT_AB(float, 0.50, tuc.getSrcAlpha(0.0f), 0.0001);
    ASSERT_AB(float, 0.75, tuc.getSrcAlpha(0.1f), 0.0001);
    ASSERT_AB(float, 0.00, tuc.getSrcAlpha(0.2f), 0.0001);

    ASSERT_AB(float, 0.00, tuc.getSrcAlpha(0.3f), 0.0001);
    ASSERT_AB(float, 0.25, tuc.getSrcAlpha(0.4f), 0.0001);
    ASSERT_AB(float, 0.50, tuc.getSrcAlpha(0.5f), 0.0001);
    ASSERT_AB(float, 0.75, tuc.getSrcAlpha(0.6f), 0.0001);
    ASSERT_AB(float, 0.00, tuc.getSrcAlpha(0.7f), 0.0001);

    TEST_REPORT();
}
