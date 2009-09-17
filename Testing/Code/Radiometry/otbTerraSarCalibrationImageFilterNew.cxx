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
#include "itkExceptionObject.h"

#include "otbTerraSarCalibrationImageFilter.h"
#include "otbImage.h"

int otbTerraSarCalibrationImageFilterNew(int argc, char * argv[])
{
  typedef otb::Image<double, 2>                                     ImageType;
  typedef otb::TerraSarCalibrationImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();
  
  return EXIT_SUCCESS;
}