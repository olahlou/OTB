/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSubsampleImageFilter_txx
#define __otbSubsampleImageFilter_txx

#include "otbSubsampleImageFilter.h"

#include "otbMacro.h"
#include "otbSubsampledImageRegionConstIterator.h"
#include "otbSubsampledImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"


namespace otb {

template < class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::PrintSelf( std::ostream & os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "SubsampleFactor = " << m_SubsampleFactor << "\n";
}

template < class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if ( GetSubsampleFactor() > 1 )
  {
    OutputImageRegionType newRegion;
    this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput()->GetLargestPossibleRegion() );
    this->GetOutput()->SetRegions( newRegion );
  }
}

template <class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void 
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::CallCopyOutputRegionToInputRegion 
( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  if ( static_cast<int>( DirectionOfTransformation ) == INVERSE )
  {
    typename OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename OutputImageRegionType::SizeType srcSize = srcRegion.GetSize();

    typename InputImageRegionType::IndexType destIndex;
    typename InputImageRegionType::SizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      destIndex[i] = srcIndex[i] / GetSubsampleFactor();
      destSize[i] = ( srcSize[i] - 1 ) / GetSubsampleFactor() + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template <class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void 
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::CallCopyInputRegionToOutputRegion
( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )	 
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  if ( static_cast<int>( DirectionOfTransformation ) == INVERSE )
  {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      destIndex[i] = srcIndex[i] * GetSubsampleFactor();
      destSize[i] = ( srcSize[i] - 1 ) * GetSubsampleFactor() + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template <class TInputImage, class TOutputImage,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
SubsampleImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  InputImageRegionType inputRegionForThread;
	this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  SubsampledImageRegionConstIterator< InputImageType > inputIter 
    ( this->GetInput(), inputRegionForThread );
  if ( static_cast<int>( DirectionOfTransformation ) == FORWARD )
    inputIter.SetSubsampleFactor( GetSubsampleFactor() );
  inputIter.GoToBegin();

  SubsampledImageRegionIterator< OutputImageType > outputIter
    ( this->GetOutput(), outputRegionForThread );
  //outputIter.SetSubsampleFactor(1);
  outputIter.GoToBegin();

  std::cerr << "thread=" << threadId 
    << " inputRegionIndex=[" << inputRegionForThread.GetIndex()[0] << "," << inputRegionForThread.GetIndex()[1] 
    << "] inputRegionSize=[" << inputRegionForThread.GetSize()[0] << "," << inputRegionForThread.GetSize()[1]
    << "]\n\toutputRegionIndex=[" << outputRegionForThread.GetIndex()[0] << "," << outputRegionForThread.GetIndex()[1]
    << "] outputRegionSize=[" << outputRegionForThread.GetSize()[0] << "," << outputRegionForThread.GetSize()[1]
    << "]\n";

  while ( !inputIter.IsAtEnd() && !outputIter.IsAtEnd() )
  {
    InputImageIndexType inputIndex = inputIter.GetLocationIndex();
    OutputImageIndexType outputIndex;
    for ( unsigned int i = 0; i < OutputImageDimension; i++ )
    {
      outputIndex[i] = inputIndex[i] * GetSubsampleFactor();
    }

    outputIter.SetLocation( outputIndex );

    outputIter.Set( static_cast< OutputPixelType >( inputIter.Get() ) );

    ++inputIter;
  }
}


} // end of namespace otb

#endif


