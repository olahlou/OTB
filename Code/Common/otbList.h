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
#ifndef __otbList_h
#define __otbList_h


namespace otb
{
//For backward compatibility only
// otb::List is deprecated and otb::ObjectList should be used instead
template <class TObject>
class ITK_EXPORT List : public ObjectList<TObject>
      {};
}

#endif