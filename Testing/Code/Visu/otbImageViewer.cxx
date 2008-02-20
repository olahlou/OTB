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
#include "otbImageFileReader.h"
#include "otbImageViewer.h"
#include "otbMacro.h"

int otbImageViewer( int argc, char * argv[] )
{
  try 
    { 
      char * filename = argv[1];

      // Parse command line parameters
      typedef double PixelType;
      typedef otb::ImageViewer<PixelType>  ImageViewerType;
      typedef ImageViewerType::ImageType ImageType;
      typedef otb::ImageFileReader<ImageType> ReaderType;
      
      // instantiation
      ImageViewerType::Pointer viewer = ImageViewerType::New();
      
      // check for input images
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(filename);
      reader->GenerateOutputInformation();
      viewer->SetImage(reader->GetOutput());      
	
      // build the app
      viewer->Show();
      Fl::check();
     //Fl::run();
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
 catch( ... ) 
     { 
       std::cout << "Exception levee inconnue !" << std::endl; 
       return EXIT_FAILURE;
     } 
  return EXIT_SUCCESS;
}


