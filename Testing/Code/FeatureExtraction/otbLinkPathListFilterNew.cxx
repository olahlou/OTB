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
#include "otbLinkPathListFilter.h"
#include "itkPolyLineParametricPath.h"
#include <cstdlib>

int otbLinkPathListFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::LinkPathListFilter<PathType>      LinkPathListFilterType;

  // Instantiating object
  LinkPathListFilterType::Pointer filter = LinkPathListFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
