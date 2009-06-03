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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"

#include <iostream>
#include "otbVectorImage.h"

#include "otbImageFileReader.h"
#include "otbImageMetadataInterface.h"


int otbImageMetadataInterfaceTest (int argc, char* argv[])
{


  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];

  typedef otb::VectorImage< double,  2 >                 InputImageType;
  typedef otb::ImageFileReader< InputImageType >         ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName( inputFilename  );
//   reader->GenerateOutputInformation();

  reader->UpdateOutputInformation();

  otb::ImageMetadataInterface::Pointer lImageMetadata = otb::ImageMetadataInterface::New();

  std::cout<<"GetSensorID:        "<<lImageMetadata->GetSensorID(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  std::cout<<"GetPhysicalGain:    "<<lImageMetadata->GetPhysicalGain(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  std::cout<<"GetPhysicalBias:    "<<lImageMetadata->GetPhysicalBias(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  std::cout<<"GetDay:             "<<lImageMetadata->GetDay(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  std::cout<<"GetMonth:           "<<lImageMetadata->GetMonth(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  std::cout<<"GetYear:            "<<lImageMetadata->GetYear(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  std::cout<<"GetSolarIrradiance: "<<lImageMetadata->GetSolarIrradiance(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;
  std::cout<<"GetSunElevation:    "<<lImageMetadata->GetSunElevation(reader->GetOutput()->GetMetaDataDictionary())<<std::endl;

  return EXIT_SUCCESS;

}