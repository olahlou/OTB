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
#ifndef __otbBandName_h
#define __otbBandName_h

namespace otb
{
namespace BandName
{

/**
* Provides a way to identify bands when passing the parameters
* to the radiometric functors.*
*/
enum BandName {BLUE, GREEN, RED, NIR, MIR};
enum LandsatTMBandNames {TM1, TM2, TM3, TM4, TM5, TM60, TM61, TM62, TM7};

// Note for landsat equivalence
// http://landsat.gsfc.nasa.gov/news/news-archive/sci_0017.html
// TM1 = Coastal
// TM2 = Blue
// TM3 = Green
// TM4 = Red
// TM5 = NIR
// TM6 = SWIR1 = MIR (SPOT5)
// TM7 = SWIR2
// TM8 = Panchro
// TM9 = Cirrus
//
// Spot at
// http://www.spotimage.com/web/en/233-resolution-and-spectral-bands.php

// Band Number        Wavelength Interval        Spectral Response
// 1                0.45-0.52 µm                Blue-Green
// 2                0.52-0.60 µm                Green
// 3                0.63-0.69 µm                Red
// 4                0.76-0.90 µm                Near IR
// 5                1.55-1.75 µm                Mid-IR
// 6              10.40-12.50 µm                Thermal IR
// 7                2.08-2.35 µm                Mid-IR

}

}

#endif
