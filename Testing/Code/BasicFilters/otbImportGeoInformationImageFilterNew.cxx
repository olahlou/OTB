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

#include "otbImportGeoInformationImageFilter.h"
#include "otbImage.h"

int otbImportGeoInformationImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                                              PixelType;
  typedef otb::Image<PixelType, Dimension>                           ImageType;
  typedef otb::ImportGeoInformationImageFilter<ImageType, ImageType> ImportGeoInformationImageFilterType;

  // Instantiating object
  ImportGeoInformationImageFilterType::Pointer filter = ImportGeoInformationImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
