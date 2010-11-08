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
#ifndef __otbImageToEnvelopeVectorDataFilter_h
#define __otbImageToEnvelopeVectorDataFilter_h

#include "otbVectorDataSource.h"
#include "otbGenericRSTransform.h"
#include "otbImageKeywordlist.h"

namespace otb
{

/** \class ImageToEnvelopeVectorDataFilter
  * \brief Build a vector data containing the polygon of the image envelope
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  *
  */

template <class TInputImage, class TOutputVectorData>
class ITK_EXPORT ImageToEnvelopeVectorDataFilter :
  public otb::VectorDataSource<TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef ImageToEnvelopeVectorDataFilter          Self;
  typedef otb::VectorDataSource<TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  typedef TInputImage                              InputImageType;
  typedef TOutputVectorData                        OutputVectorDataType;
  typedef typename TInputImage::ConstPointer       InputImagePointer;
  typedef typename TOutputVectorData::Pointer      OutputVectorDataPointer;

  /** Some typedefs. */
  typedef otb::GenericRSTransform<double, 2, 2>    InternalTransformType;
  typedef typename InternalTransformType::Pointer  InternalTransformPointerType;

  typedef typename OutputVectorDataType
                   ::DataNodeType                  OutputDataNodeType;
  typedef typename OutputVectorDataType
                   ::DataNodePointerType           OutputDataNodePointerType;
  typedef typename OutputVectorDataType::DataTreePointerType OutputDataTreePointerType;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType  InputInternalTreeNodeType;
  typedef typename OutputVectorDataType::DataTreeType::TreeNodeType OutputInternalTreeNodeType;
  typedef typename InputInternalTreeNodeType::ChildrenListType      InputChildrenListType;

  typedef typename OutputDataNodeType::PointType PointType;

  typedef typename OutputDataNodeType::LineType             LineType;
  typedef typename OutputDataNodeType::LineConstPointerType LineConstPointerType;
  typedef typename OutputDataNodeType::LinePointerType      LinePointerType;

  typedef typename OutputDataNodeType::PolygonType             PolygonType;

  typedef typename OutputDataNodeType::PolygonListType             PolygonListType;
  typedef typename OutputDataNodeType::PolygonListConstPointerType PolygonListConstPointerType;
  typedef typename OutputDataNodeType::PolygonListPointerType      PolygonListPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToEnvelopeVectorDataFilter, VectorDataSource);

  /** Set/Get output projection (default is WGS84)  */
  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  itkSetStringMacro(DEMDirectory);
  itkGetStringMacro(DEMDirectory);

  itkSetMacro(AverageElevation, double);
  itkGetMacro(AverageElevation, double);

protected:
  ImageToEnvelopeVectorDataFilter();
  virtual ~ImageToEnvelopeVectorDataFilter() {}

  void GenerateOutputInformation(void);

  void GenerateData(void);

private:
  ImageToEnvelopeVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InternalTransformPointerType m_Transform;
  std::string                  m_OutputProjectionRef;
  std::string                  m_DEMDirectory;
  double                       m_AverageElevation;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToEnvelopeVectorDataFilter.txx"
#endif

#endif
