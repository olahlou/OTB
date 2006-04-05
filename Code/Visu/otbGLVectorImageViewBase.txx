#ifndef otbGLVectorImageViewBase_txx
#define otbGLVectorImageViewBase_txx

#include "itkExceptionObject.h"
#include "itkImageRegionConstIterator.h"

#include "otbGLVectorImageViewBase.h"

#include <math.h>

namespace otb
{

template <class TPixel, class OverlayPixelType>
GLVectorImageViewBase<TPixel, OverlayPixelType>::
GLVectorImageViewBase() : VectorImageView<TPixel>(), Fl_Gl_Window(0,0,0,0,0)
{
        when(FL_WHEN_NOT_CHANGED | FL_WHEN_ENTER_KEY);
        this->ClearSelectChannels();
        m_ViewRectangle = true;
}

template <class TPixel, class OverlayPixelType>
GLVectorImageViewBase<TPixel, OverlayPixelType>::
~GLVectorImageViewBase()
{

}


template <class TPixel, class OverlayPixelType>
void
GLVectorImageViewBase<TPixel, OverlayPixelType>::
Init(int x, int y, int w, int h, const char * l)
{
        //Methode sur Fl_Gl_Window
        this->resize(x,y,w,h);
        //Et le "l" !!! (THOMAS)
}

//
// Set the input image to be displayed
// Warning: the current overlay is destroyed if the size of the image
// is different from the size of the overlay.
//
// Initialise la class a partir du "m_ViewImageRegion" préalablement initialisé
template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
FinalizeInitialisation(void)
{
        m_CenterPointImage = GetCenterRegion(this->m_ViewImageRegion);

  SizeType   imSize   = this->GetInput()->GetRequestedRegion().GetSize();

  this->cDimSize[0]=imSize[0];
  this->cDimSize[1]=imSize[1];

  this->CalculeDataMinMax(      this->m_ViewImageRegion,  
                                this->cDataMin, 
                                this->cDataMax);

  this->cIWMin      = this->cDataMin;
  this->cIWMax      = this->cDataMax;

  this->cWinDataSizeX = imSize[0];
  if( this->cWinDataSizeX < imSize[1] )
  {
        this->cWinDataSizeX = imSize[1];
  }
  this->cWinDataSizeY = this->cWinDataSizeX;
  
  if(this->cWinImData != NULL)
    {
    delete [] this->cWinImData;
    }
  this->cWinImData = new unsigned char[ this->cWinDataSizeX * this->cWinDataSizeY * this->m_NbDim ];

  this->cViewImData  = true;
  this->cValidImData = true;
  
}


template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::IndexType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
WindowCoord2ImageCoord( const IndexType & index )const
{
        IndexType lCoord;
        RegionType lRegion = this->GetViewImageRegion();
        for( int i=0 ; i < 2 ; i++)
        {
                lCoord[i] = (unsigned long)(index[i]/this->cWinZoom) + lRegion.GetIndex()[i];
        }
        return lCoord;
} 
template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::SizeType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
WindowSize2ImageSize( const SizeType & size )const
{
        SizeType lSize;
        for( int i=0 ; i < 2 ; i++)
        {
                lSize[i] = (unsigned long)(size[i]/this->cWinZoom);
        }
        return lSize;
} 

template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::RegionType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
WindowZone2ImageRegion( const RegionType & zone )const
{
        RegionType lRegion;
        SizeType  lSize  = this->WindowSize2ImageSize(zone.GetSize());
        IndexType lIndex = this->WindowCoord2ImageCoord(zone.GetIndex());
        lRegion.SetIndex( lIndex );
        lRegion.SetSize( lSize );
        return lRegion;
} 


template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::IndexType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
GetCenterRegion( const RegionType & zone )
{
        IndexType lCenter;
        lCenter[0] = zone.GetIndex()[0] + zone.GetSize()[0] / 2; 
        lCenter[1] = zone.GetIndex()[1] + zone.GetSize()[1] / 2; 
        return lCenter;
} 

template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::RegionType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
ShrinkRegion( const RegionType & zone, const float shrinkfactor )
{
        RegionType lRegion;
        SizeType  lSize;
        IndexType lIndex;
        for( int i=0 ; i<2 ; i++)
        {
                lSize[i]  = (int) (shrinkfactor*zone.GetSize()[i]);
                lIndex[i] = (int) (shrinkfactor*zone.GetIndex()[i]);
        }
        lRegion.SetIndex( lIndex );
        lRegion.SetSize( lSize );
        return (lRegion);
}


template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::RegionType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
GetViewRegion( const IndexType & centerPointImage )
{
        return( GetViewRegion( this->GetInput()->GetRequestedRegion(), centerPointImage) );
}


template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::RegionType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
GetViewRegion( const RegionType & region, const IndexType & centerPointImage )
{
        RegionType lNewViewRegion;
        RegionType lImageRegionComplete = region;
        SizeType lSize;
        IndexType lIndex;
        double scale0 = this->cWinZoom;
        double scale1 = this->cWinZoom;
        lSize[0] = (int)( this->cW / scale0 );
        lSize[1] = (int)( this->cH / scale1 );
        //Reajuste le centre si la zone zoom est sur les bords
        for( int i=0 ; i < 2 ; i++)
        {
                lIndex[i] = centerPointImage[i] - lSize[i]/2;
                if ( lIndex[i] < lImageRegionComplete.GetIndex()[i] ) 
                {
                        lIndex[i] = lImageRegionComplete.GetIndex()[i];
                }
                else if( lIndex[i] + lSize[i] > lImageRegionComplete.GetIndex()[i] + lImageRegionComplete.GetSize()[i] )
                {
                        lIndex[i] = lImageRegionComplete.GetIndex()[i] + lImageRegionComplete.GetSize()[i] - lSize[i];
                }
                //Si la fenetre est plus grande en pixel) que l'image reelle, on réajuste la taille de l'image
                if( lSize[i] >  lImageRegionComplete.GetSize()[i] )
                {
                        lSize[i] = lImageRegionComplete.GetSize()[i];
                }
        }
        lNewViewRegion.SetIndex( lIndex );
        lNewViewRegion.SetSize(  lSize );
        return(lNewViewRegion);
}
template <class TPixel, class OverlayPixelType>
typename GLVectorImageViewBase<TPixel, OverlayPixelType>::RegionType
GLVectorImageViewBase<TPixel, OverlayPixelType>::
TranslateImageRegionToWindowRegion( const RegionType & imageRegion, const  RegionType & sousimageRegion, const int windowWidth, const int windowHeight)
{
        RegionType lRegion;
        SizeType   lSize;
        IndexType  lIndex ;
        for( int i=0 ; i < 2 ; i++)
        {
                lIndex[i] = sousimageRegion.GetIndex()[i] - imageRegion.GetIndex()[i];
                lSize[i] = sousimageRegion.GetSize()[i];
        }
        lRegion.SetIndex( lIndex );
        lRegion.SetSize(  lSize );
        return(lRegion);
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageViewBase<TPixel, OverlayPixelType>::
MajViewRegion(void)
{
        //Position du centre de l'image en coordonnees image
        this->SetViewImageRegion( GetViewRegion(m_CenterPointImage) );
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageViewBase<TPixel, OverlayPixelType>::
BuildWithImageRegion()
{
        this->SetViewImageRegion( this->GetInput()->GetRequestedRegion() );
        this->cWinZoom = 1.;
        //Termine l'initialisation 
        FinalizeInitialisation();
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageViewBase<TPixel, OverlayPixelType>::
BuildWithWindowRegion(const int zoom)
{
        RegionType lRegion;
        RegionType lWindow;
        SizeType lSize;
        IndexType lIndex;
        
        lSize[0] = this->cW;
        lSize[1] = this->cH;
        lWindow.SetSize(lSize);
        for( int i=0 ; i < 2 ; i++)
        {
                lSize[i]  =   lWindow.GetSize()[i] / zoom;
                lIndex[i] =   this->GetInput()->GetRequestedRegion().GetIndex()[i]
                            + this->GetInput()->GetRequestedRegion().GetSize()[i] / 2 
                            - lSize[i] / 2;
        }
        lRegion.SetIndex(lIndex);
        lRegion.SetSize(lSize);
        this->m_ViewImageRegion = lRegion;
        this->cWinZoom = (float)zoom;

        //Termine l'initialisation 
        FinalizeInitialisation();
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageViewBase<TPixel, OverlayPixelType>::
PrintInfos(void)
{
std::cout << this->GetNameOfClass()<< std::endl;
std::cout << "- cW, cH               :   ["<<this->cW<<","<<this->cH<<"]"<< std::endl;
std::cout << "- zoom                 :   "<<this->winZoom()<< std::endl;
std::cout << "- zoom                 :   "<<this->cWinZoom<< std::endl;
std::cout << "- m_CenterPointImage   :   "<<m_CenterPointImage<< std::endl;
std::cout << "- m_ViewImageRegion    :   "<<this->m_ViewImageRegion<< std::endl;
}

//
//
template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
update()
{
        if( this->cValidImData == false ) 
        {
                return;
        }

        //Mise a jour de la fenetre 
        this->MajViewRegion();

        //Update d'ITK
        this->Update();

        memset( this->cWinImData, 0, this->cWinDataSizeX*this->cWinDataSizeY*this->m_NbDim );
    
        //Lecture des pixels et initialisation du buffer
        this->SetWinImData( this->m_ViewImageRegion );

        this->redraw();
}

template <class TPixel, class OverlayPixelType>
void GLVectorImageViewBase<TPixel, OverlayPixelType>::size(int w, int h)
  {
  VectorImageView<TPixel>::size(w, h);
  Fl_Gl_Window::size(w, h);
  this->update();
  this->redraw();
  }


template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
resize(int x, int y, int w, int h)
  {
  VectorImageView<TPixel>::resize(x, y, w, h);
  Fl_Gl_Window::resize(x, y, w, h);
  this->update();
  this->redraw();
  }

template <class TPixel, class OverlayPixelType>
void GLVectorImageViewBase<TPixel, OverlayPixelType>::draw(void)
{
  if( !this->valid() )
    {
    glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);          
    glShadeModel(GL_FLAT);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //if you don't include this
                                            //image size differences distort
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    }
  else
    {
    glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
    
    glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    this->ortho();
    
    if( !this->GetInput() ) 
      {
      return;
      }
    
    double scale0 = this->cWinZoom;
    double scale1 = this->cWinZoom;
    
    int originX = 0;
    int originY = 0;
//THOMAS
    if( this->GetViewImageRegion().GetSize()[0]*scale0 < this->cW )
    {
        originX = static_cast<int>( (this->cW - this->GetViewImageRegion().GetSize()[0]*scale0)/2);
    }
    if( this->GetViewImageRegion().GetSize()[1]*scale1 < this->cH )
    {
        originY = static_cast<int>( (this->cH - this->GetViewImageRegion().GetSize()[1]*scale1)/2);
    }

    glRasterPos2i(      originX ,  this->cH - originY );  
    glPixelZoom(        scale0 , - scale1 );
    
    if( (this->cValidImData==true) && (this->cViewImData == true) )
      {
                if ( this->m_ModeView == GRAY_LEVEL )
                { 
                        glDrawPixels(           this->cWinDataSizeX, 
                                                this->cWinDataSizeY, 
                                                GL_LUMINANCE,
                                                GL_UNSIGNED_BYTE, 
                                                this->cWinImData );
                }
                else
                { 
                        glDrawPixels(           this->cWinDataSizeX,
                                                this->cWinDataSizeY, 
                                                GL_RGB,
                                                GL_UNSIGNED_BYTE, 
                                                this->cWinImData );
                }
      }

    if( m_ViewRectangle == true ) 
      {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(1., 0., 0., (float)0.75);
                gl_font(FL_TIMES_BOLD, 12);
                RegionType lDrawRectangle = TranslateImageRegionToWindowRegion( this->GetViewImageRegion(), m_DrawViewRectangle, this->cW, this->cH);
                gl_rect(        lDrawRectangle.GetIndex()[0], 
                                this->cH - lDrawRectangle.GetIndex()[1] - lDrawRectangle.GetSize() [1], 
                                lDrawRectangle.GetSize() [0], 
                                lDrawRectangle.GetSize() [1]);
                glDisable(GL_BLEND);
      }

   }    
}


template <class TPixel, class OverlayPixelType>
void
GLVectorImageViewBase<TPixel, OverlayPixelType>::
ClearSelectChannels(void)
{ 
        m_RedChannel            = -1;
        m_GreenChannel          = -1;
        m_BlueChannel           = -1;
        m_GrayLevelChannel      = -1;
        m_ChannelsWorks.clear();
 
}



template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
CalculeDataMinMax(const RegionType & region, double & pMin, double & pMax)
{
        typedef itk::ImageRegionConstIterator<ImageType> InputIterator;
        InputIterator inIt(this->GetInput(), region);

        // Parcours des canaux a traiter
        unsigned int channelIn(0);
        unsigned int channelOut(0);
        unsigned int nbChannels(0);

        PixelType  pixelInput;
        pixelInput = inIt.Get();
        channelOut = 0;
        //Initialisation du Max/Min
        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        {
                channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                pMax = static_cast<double>(pixelInput[channelIn]);
                pMin = pMax;
        }
        ++inIt;
        while( !inIt.IsAtEnd() )
        {
                pixelInput = inIt.Get();
                channelOut = 0;
                for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
                {
                        channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                        if ( pixelInput[channelIn] > pMax )
                        {
                                pMax = static_cast<double>(pixelInput[channelIn]);
                        }
                        else if ( pixelInput[channelIn] < pMin )
                        {
                                pMin = static_cast<double>(pixelInput[channelIn]);
                        }
                        channelOut++;
                }
                ++inIt; 
        }
}
  
template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
SetWinImData(const RegionType & zone) 
{
  IndexType ind;
  int l, m;
  float tf;

  int lWinMinX = zone.GetIndex()[0];
  int lWinMinY = zone.GetIndex()[1];
  int lWinMaxX = lWinMinX + zone.GetSize()[0] - 1;
  int lWinMaxY = lWinMinY + zone.GetSize()[1] - 1;

  int startK = zone.GetIndex()[1];
    if(startK<0)
    startK = 0;
  int startJ = zone.GetIndex()[0];
  if(startJ<0)
    startJ = 0;
    
  float lCoef = (float)(255./(this->cIWMax-this->cIWMin));
  ImagePointer lInpuPtr = this->GetInput();
  PixelType  pixelInput;
  itk::VariableLengthVector<float> pixelOutput;
  int nbChannels(0);
  int channelIn(0);
  for(int k=startK; k <= lWinMaxY; k++)
    {
    ind[1] = k;
    
    if(k-lWinMinY >= (int)this->cWinDataSizeY)
      continue;

    for(int j=startJ; j <= lWinMaxX; j++) 
      {
      ind[0] = j;
      
      if(j-lWinMinX >= (int)this->cWinDataSizeX)
         continue;

      switch( this->cImageMode ) 
        {
        default:
        case IMG_VAL:
                pixelInput = lInpuPtr->GetPixel(ind);
                pixelOutput.Reserve( lInpuPtr->GetVectorLength() );
                // Traitement sur les canaux selectionnes.
                for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
                {
                        channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                        pixelOutput[channelIn] = static_cast<float>(pixelInput[channelIn]);
                        pixelOutput[channelIn] = (float)( (pixelOutput[channelIn]-this->cIWMin) * lCoef);
                }
          break;
        }
        // On s'assure que les valeurs sont dans [0,255]
        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        {
                channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                if ( pixelOutput[channelIn] > 255. ) pixelOutput[channelIn] = 255.;
                else if ( pixelOutput[channelIn] < 0. ) pixelOutput[channelIn] = 0.;

        }
      
      l = (j-lWinMinX)*this->m_NbDim + (k-lWinMinY)*this->cWinDataSizeX*this->m_NbDim;
      
      if( this->m_ModeView == GRAY_LEVEL )
      {
                this->cWinImData[l] = (unsigned char)pixelOutput[this->m_GrayLevelChannel - 1];
       }
       else
       {
                if ( this->m_RedChannel != -1 )         this->cWinImData[l+0] = (unsigned char)pixelOutput[this->m_RedChannel - 1];
                else                                    this->cWinImData[l+0] = (unsigned char)(0);
                if ( this->m_GreenChannel != -1 )       this->cWinImData[l+1] = (unsigned char)pixelOutput[this->m_GreenChannel - 1];
                else                                    this->cWinImData[l+1] = (unsigned char)(0);
                if ( this->m_BlueChannel != -1 )        this->cWinImData[l+2] = (unsigned char)pixelOutput[this->m_BlueChannel - 1];
                else                                    this->cWinImData[l+2] = (unsigned char)(0);
       }
     }

   }
}

}; //namespace

#endif

