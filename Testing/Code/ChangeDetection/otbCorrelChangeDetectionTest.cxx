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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkImage.h>
#include "otbCorrelationChangeDetector.h"
#include "otbCommandProgressUpdate.h"

int otbCorrelChangeDetectionTest(int argc, char* argv[])
{

  if (argc < 5)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile1 inputImageFile2  radius outputImageFile " << std::endl;
    return -1;
    }

  // Define the dimension of the images
  const unsigned int Dimension = 2;

  // Declare the types of the images
  typedef float                                    InternalPixelType;
  typedef unsigned char                            OutputPixelType;
  typedef itk::Image<InternalPixelType, Dimension> InputImageType1;
  typedef itk::Image<InternalPixelType, Dimension> InputImageType2;
  typedef itk::Image<InternalPixelType, Dimension> ChangeImageType;
  typedef itk::Image<OutputPixelType, Dimension>   OutputImageType;

  typedef otb::ImageFileReader<InputImageType1> ReaderType1;
  typedef otb::ImageFileReader<InputImageType2> ReaderType2;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef itk::RescaleIntensityImageFilter<ChangeImageType,
      OutputImageType> RescalerType;

  // Declare the type for the filter
  typedef otb::CorrelationChangeDetector<
      InputImageType1,
      InputImageType2,
      ChangeImageType>       FilterType;

  ReaderType1::Pointer  reader1 = ReaderType1::New();
  ReaderType2::Pointer  reader2 = ReaderType2::New();
  WriterType::Pointer   writer = WriterType::New();
  FilterType::Pointer   filter = FilterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();

  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * outputFilename = argv[4];

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);
  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());

  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());
  filter->SetRadius(atoi(argv[3]));

  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());

  typedef otb::CommandProgressUpdate<FilterType> CommandType;

  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver(itk::ProgressEvent(), observer);

  writer->Update();

  return EXIT_SUCCESS;
}
