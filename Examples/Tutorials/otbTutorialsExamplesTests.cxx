/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


    Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
    See OTBCopyright.txt for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests

#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(TutorialsHelloWorldOTBTest);
  REGISTER_TEST(TutorialsPipelineTest);
  REGISTER_TEST(TutorialsFilteringPipelineTest);
  REGISTER_TEST(TutorialsScalingPipelineTest);
  REGISTER_TEST(TutorialsMultispectralTest);
  REGISTER_TEST(TutorialsSmarterFilteringPipelineTest);
  REGISTER_TEST(TutorialsOrthoFusionTest);
}

#undef main
#define main TutorialsHelloWorldOTBTest
#include "HelloWorldOTB.cxx"

#undef main
#define main TutorialsPipelineTest
#include "Pipeline.cxx"

#undef main
#define main TutorialsFilteringPipelineTest
#include "FilteringPipeline.cxx"

#undef main
#define main TutorialsScalingPipelineTest
#include "ScalingPipeline.cxx"

#undef main
#define main TutorialsMultispectralTest
#include "Multispectral.cxx"

#undef main
#define main TutorialsSmarterFilteringPipelineTest
#include "SmarterFilteringPipeline.cxx"

#undef main
#define main TutorialsOrthoFusionTest
#include "OrthoFusion.cxx"
