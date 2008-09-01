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
#ifndef __otbBinaryFunctorNeighborhoodImageFilter_txx
#define __otbBinaryFunctorNeighborhoodImageFilter_txx

#include "otbBinaryFunctorNeighborhoodImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction  >
BinaryFunctorNeighborhoodImageFilter<TInputImage1,TInputImage2,TOutputImage,TFunction>
::BinaryFunctorNeighborhoodImageFilter()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->InPlaceOff();
  m_Radius = 3;
}


/**
 * Connect one of the operands for neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction  >
void
BinaryFunctorNeighborhoodImageFilter<TInputImage1,TInputImage2,TOutputImage,TFunction>
::SetInput1( const TInputImage1 * image1 ) 
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(0, const_cast<TInputImage1 *>( image1 ));
}


/**
 * Connect one of the operands for neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction  >
void
BinaryFunctorNeighborhoodImageFilter<TInputImage1,TInputImage2,TOutputImage,TFunction>
::SetInput2( const TInputImage2 * image2 ) 
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(1, const_cast<TInputImage2 *>( image2 ));
}



/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage1, class TInputImage2, class TOutputImage, class TFunction  >
void
BinaryFunctorNeighborhoodImageFilter<TInputImage1, TInputImage2, TOutputImage, TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{

  //unsigned int i;
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage1> nbc1;
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage2> nbc2;

// We use dynamic_cast since inputs are stored as DataObjects.  The
  // ImageToImageFilter::GetInput(int) always returns a pointer to a
  // TInputImage1 so it cannot be used for the second input.
  Input1ImagePointer inputPtr1
	  = dynamic_cast<const TInputImage1*>(ProcessObjectType::GetInput(0));
  Input2ImagePointer inputPtr2
    = dynamic_cast<const TInputImage2*>(ProcessObjectType::GetInput(1));
  OutputImagePointer outputPtr = this->GetOutput(0);
  
  
  RadiusType1 r1;
  r1.Fill(m_Radius);
  NeighborhoodIteratorType1 neighInputIt1;

  RadiusType2 r2;
  r2.Fill(m_Radius);
  NeighborhoodIteratorType2 neighInputIt2;
    
  itk::ImageRegionIterator<TOutputImage> outputIt;


  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage1>::FaceListType faceList1;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage1> bC1;
  faceList1 = bC1(inputPtr1, outputRegionForThread, r1);

  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage2>::FaceListType faceList2;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage2> bC2;
  faceList2 = bC2(inputPtr2, outputRegionForThread, r2);

  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage1>::FaceListType::iterator fit1;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage2>::FaceListType::iterator fit2;

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit1=faceList1.begin(), fit2=faceList2.begin(); fit1 != faceList1.end(), fit2 != faceList2.end(); ++fit1, ++fit2)
    { 
    neighInputIt1 = itk::ConstNeighborhoodIterator<TInputImage1>(r1, inputPtr1, *fit1);
    neighInputIt2 = itk::ConstNeighborhoodIterator<TInputImage2>(r2, inputPtr2, *fit2);
    // outputIt = itk::ImageRegionIterator<TOutputImage>(outputPtr, outputRegionForThread);
      
    outputIt = itk::ImageRegionIterator<TOutputImage>(outputPtr, *fit1);
    neighInputIt1.OverrideBoundaryCondition(&nbc1);
    neighInputIt1.GoToBegin();
    neighInputIt2.OverrideBoundaryCondition(&nbc2);
    neighInputIt2.GoToBegin();

    while ( ! outputIt.IsAtEnd() )
      {

      outputIt.Set( m_Functor( neighInputIt1, neighInputIt2 ) );

      ++neighInputIt1;
      ++neighInputIt2;
      ++outputIt;
      progress.CompletedPixel();
      }
    }


  

}

} // end namespace otb

#endif
