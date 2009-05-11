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
#ifndef __otbStandardImageViewerTwoLayers_txx
#define __otbStandardImageViewerTwoLayers_txx

#include "otbStandardImageViewerTwoLayers.h"
#include "otbForwardSensorModel.h"
#include "otbFltkFilterWatcher.h"

namespace otb
{

template <class TImage, class TSecondImage, class TVectorData,class TWidgetManager>
StandardImageViewerTwoLayers<TImage,TSecondImage,TVectorData,TWidgetManager>
::StandardImageViewerTwoLayers() : m_Label("Default label"), m_Image(), m_VectorData(),
                          m_ImageLayer(), m_RenderingModel(),m_PixelDescriptionModel(),
                          m_View(), m_PixelDescriptionView(), m_CurveWidget(),
                          m_Controller(), m_RenderingFunction(), m_DisplayWindow()
{
  // Build a new rendering model
  m_RenderingModel = RenderingModelType::New();
  m_PixelDescriptionModel = PixelDescriptionModelType::New();
  m_PixelDescriptionModel->SetLayers(m_RenderingModel->GetLayers());

  // Build a view
  m_View                 = ViewType::New();
  m_PixelDescriptionView = PixelDescriptionViewType::New();

  // Build a controller
  m_Controller     = WidgetControllerType::New();

  // Build the curve widget
  m_CurveWidget    = CurvesWidgetType::New();

  // Wire the MBC
  m_View->SetModel(m_RenderingModel);
  m_View->SetController(m_Controller);
  m_PixelDescriptionView->SetModel(m_PixelDescriptionModel);

  // Add the resizing handler
  ResizingHandlerType::Pointer resizingHandler = ResizingHandlerType::New();
  resizingHandler->SetModel(m_RenderingModel);
  resizingHandler->SetView(m_View);
  m_Controller->AddActionHandler(resizingHandler);

  // Add the change scaled region handler
  ChangeScaledRegionHandlerType::Pointer changeScaledHandler =ChangeScaledRegionHandlerType::New();
  changeScaledHandler->SetModel(m_RenderingModel);
  changeScaledHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeScaledHandler);

  // Add the change extract region handler
  ChangeRegionHandlerType::Pointer changeHandler =ChangeRegionHandlerType::New();
  changeHandler->SetModel(m_RenderingModel);
  changeHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeHandler);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer changeScaleHandler =ChangeScaleHandlerType::New();
  changeScaleHandler->SetModel(m_RenderingModel);
  changeScaleHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeScaleHandler);

// Add the pixel description action handler
  PixelDescriptionActionHandlerType::Pointer pixelActionHandler = PixelDescriptionActionHandlerType::New();
  pixelActionHandler->SetView(m_View);
  pixelActionHandler->SetModel(m_PixelDescriptionModel);
  m_Controller->AddActionHandler(pixelActionHandler);

  // Add the action handler for the arrow key
  ArrowKeyMoveActionHandlerType::Pointer arrowKeyMoveHandler = ArrowKeyMoveActionHandlerType::New();
  arrowKeyMoveHandler->SetModel(m_RenderingModel);
  arrowKeyMoveHandler->SetView(m_View);
  m_Controller->AddActionHandler(arrowKeyMoveHandler);

  /** Display Window*/
  m_DisplayWindow   = WidgetManagerType::New();
  m_DisplayWindow->RegisterFullWidget(m_View->GetFullWidget());
  m_DisplayWindow->RegisterScrollWidget(m_View->GetScrollWidget());
  m_DisplayWindow->RegisterZoomWidget(m_View->GetZoomWidget());
  m_DisplayWindow->RegisterPixelInformationWidget(m_PixelDescriptionView->GetPixelDescriptionWidget());
  m_DisplayWindow->RegisterHistogramWidget(m_CurveWidget);
}

template <class TImage, class TSecondImage, class TVectorData,class TWidgetManager>
StandardImageViewerTwoLayers<TImage,TSecondImage,TVectorData,TWidgetManager>
::~StandardImageViewerTwoLayers()
{}

template <class TImage, class TSecondImage, class TVectorData,class TWidgetManager>
void
StandardImageViewerTwoLayers<TImage,TSecondImage,TVectorData,TWidgetManager>
::Update()
{
  // First check if there is actually an input image
  if(m_Image.IsNull())
    {
    itkExceptionMacro(<<"The image pointer is null, there is nothing to display. You probably forget to set the image.");
    }

  // Update image info for further use
  m_Image->UpdateOutputInformation();


  typename VectorDataProjectionFilterType::Pointer vproj;
  typename VectorDataExtractROIType::Pointer vdextract;

  // Colors
  typename HistogramCurveType::ColorType red,green,blue;
  red.Fill(0);
  red[0]=1.;
  red[3]=0.5;

  green.Fill(0);
  green[1]=1.;
  green[3]=0.5;

  blue.Fill(0);
  blue[2]=1.;
  blue[3]=0.5;


  // If there is a VectorData
  if(m_VectorData.IsNotNull())
    {
    // Extract The part of the VectorData that actually overlaps with
    // the image extent
    vdextract = VectorDataExtractROIType::New();
    vdextract->SetInput(m_VectorData);

    // Find the geographic region of interest

    // Ge the index of the corner of the image
    typename ImageType::IndexType ul, ur, ll, lr;
    typename ImageType::PointType pul,pur,pll,plr;
    ul = m_Image->GetLargestPossibleRegion().GetIndex();
    ur = ul;
    ll = ul;
    lr = ul;
    ur[0]+=m_Image->GetLargestPossibleRegion().GetSize()[0];
    lr[0]+=m_Image->GetLargestPossibleRegion().GetSize()[0];
    lr[1]+=m_Image->GetLargestPossibleRegion().GetSize()[1];
    ll[1]+=m_Image->GetLargestPossibleRegion().GetSize()[1];

    // Transform to physical point
    m_Image->TransformIndexToPhysicalPoint(ul,pul);
    m_Image->TransformIndexToPhysicalPoint(ur,pur);
    m_Image->TransformIndexToPhysicalPoint(ll,pll);
    m_Image->TransformIndexToPhysicalPoint(lr,plr);

    // Build the cartographic region
    RemoteSensingRegionType rsRegion;
    typename RemoteSensingRegionType::IndexType rsOrigin;
    typename RemoteSensingRegionType::SizeType  rsSize;
    rsOrigin[0]= min(pul[0],plr[0]);
    rsOrigin[1]= min(pul[1],plr[1]);
    rsSize[0]=vcl_abs(pul[0]-plr[0]);
    rsSize[1]=vcl_abs(pul[1]-plr[1]);

    rsRegion.SetOrigin(rsOrigin);
    rsRegion.SetSize(rsSize);
    rsRegion.SetRegionProjection(m_Image->GetProjectionRef());
    rsRegion.SetKeywordList(m_Image->GetImageKeywordlist());

    // Set the cartographic region to the extract roi filter
    vdextract->SetRegion(rsRegion);
    vdextract->SetDEMDirectory(m_DEMDirectory);

    // Reproject VectorData in image projection
    vproj = VectorDataProjectionFilterType::New();
    vproj->SetInput(vdextract->GetOutput());
    vproj->SetInputProjectionRef(m_VectorData->GetProjectionRef());
    vproj->SetOutputKeywordList(m_Image->GetImageKeywordlist());
    vproj->SetOutputProjectionRef(m_Image->GetProjectionRef());
    vproj->SetOutputOrigin(m_Image->GetOrigin());
    vproj->SetOutputSpacing(m_Image->GetSpacing());
    vproj->SetDEMDirectory(m_DEMDirectory);
    vproj->Update();

     // Create a VectorData gl component
     typename VectorDataGlComponentType::Pointer vgl = VectorDataGlComponentType::New();
     vgl->SetVectorData(vproj->GetOutput());
     vgl->SetColor(blue);
     // Add it to the image view
      m_View->GetScrollWidget()->AddGlComponent(vgl);
      m_View->GetFullWidget()->AddGlComponent(vgl);
      m_View->GetZoomWidget()->AddGlComponent(vgl);
    }

  // Generate the layer
  ImageLayerGeneratorPointerType generator = ImageLayerGeneratorType::New();
  generator->SetImage(m_Image);
  FltkFilterWatcher qlwatcher(generator->GetResampler(),0,0,200,20,"Generating QuickLook ...");
  generator->GenerateLayer();
  m_ImageLayer = generator->GetLayer();
  m_RenderingFunction = generator->GetDefaultRenderingFunction();

  // Set the window and layer label
  m_DisplayWindow->SetLabel(m_Label.c_str());
  m_ImageLayer->SetName(m_Label);

  // Add the generated layer to the rendering model
  m_RenderingModel->AddLayer(generator->GetLayer());

  if (!m_SecondImage.IsNull())
  {
    // Generate the second layer
    SecondImageLayerGeneratorPointerType generator2 = SecondImageLayerGeneratorType::New();
    generator2->SetImage(m_SecondImage);
    FltkFilterWatcher qlwatcher(generator2->GetResampler(),0,0,200,20,"Generating QuickLook ...");
    generator2->GenerateLayer();
//     m_SecondImageLayer = generator2->GetLayer();
//     m_RenderingFunction = generator2->GetDefaultRenderingFunction();

    // Set the window and layer label
//     m_DisplayWindow->SetLabel(m_Label.c_str());
//     m_ImageLayer->SetName(m_Label);

    // Add the generated layer to the rendering model
    m_RenderingModel->AddLayer(generator2->GetLayer());

  }




  // Show everything
  m_DisplayWindow->Show();

  // Update the rendering model
  m_RenderingModel->Update();

  // adding histograms rendering
  typename HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
  rhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(m_RenderingFunction->GetRedChannelIndex()));
  rhistogram->SetHistogramColor(red);
  rhistogram->SetLabelColor(red);

  typename HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
  ghistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(m_RenderingFunction->GetGreenChannelIndex()));
  ghistogram->SetHistogramColor(green);
  ghistogram->SetLabelColor(green);

  typename HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
  bhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(m_RenderingFunction->GetBlueChannelIndex()));
  bhistogram->SetHistogramColor(blue);
  bhistogram->SetLabelColor(blue);
  m_CurveWidget->AddCurve(rhistogram);
  m_CurveWidget->AddCurve(ghistogram);
  m_CurveWidget->AddCurve(bhistogram);
  m_CurveWidget->SetXAxisLabel("Pixels");
  m_CurveWidget->SetYAxisLabel("Frequency");
}
}
#endif