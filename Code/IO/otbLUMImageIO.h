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
#ifndef __otbLUMImageIO_h
#define __otbLUMImageIO_h

#include "itkImageIOBase.h"
#include <fstream>
#include <string>
#include <vector>

namespace otb
{

/** \class LUMImageIO
 *
 * \brief ImageIO object for reading (not writing) LUM format images
 *
 * The streaming read is implemented.
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT LUMImageIO : public itk::ImageIOBase
{
public:

  /** Standard class typedefs. */
  typedef LUMImageIO              Self;
  typedef itk::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Byte order typedef */
  typedef Superclass::ByteOrder ByteOrder;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LUMImageIO, itk::ImageIOBase);

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead()
  {
    return true;
  }

  /** Set the spacing and dimention information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite()
  {
    return true;
  }

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);
  // JULIEN: NOT USED, NOT IMPLEMENTED
  //void SampleImage(void* buffer, int XBegin, int YBegin, int SizeXRead, int SizeYRead, int XSample, int YSample);

protected:
  /** Constructor.*/
  LUMImageIO();
  /** Destructor.*/
  virtual ~LUMImageIO();

  bool OpenOneraDataFileForReading(const char* filename);
  bool OpenOneraHeaderFileForReading(const char* filename);
  void InternalReadImageInformation();

  bool OpenOneraDataFileForWriting(const char* filename);
  bool OpenOneraHeaderFileForWriting(const char* filename);

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LUMImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Internal method to read header informations */
  bool InternalReadHeaderInformation(std::fstream& file, const bool reportError);
  /** This method get the LUM type */
  int CaiGetTypeLum(const char *          type_code,
                    std::string&   str_sens_code,
                    int&           inbbits,
                    std::string&   str_cod_pix);

#define otbSwappFileOrderToSystemOrderMacro(StrongType, buffer, buffer_size) \
    { \
    typedef itk::ByteSwapper<StrongType> InternalByteSwapperType; \
    if (m_ByteOrder != m_FileByteOrder) \
      { \
      if (m_ByteOrder == LittleEndian) \
        { \
        InternalByteSwapperType::SwapRangeFromSystemToBigEndian((StrongType *) buffer, buffer_size); \
        } \
      else if (m_ByteOrder == BigEndian) \
        { \
        InternalByteSwapperType::SwapRangeFromSystemToLittleEndian((StrongType *) buffer, buffer_size); \
        } \
      } \
    }

#define otbSwappFileToSystemMacro(StrongType, WeakType, buffer, buffer_size) \
  else if (this->GetComponentType() == WeakType) \
    { \
    otbSwappFileOrderToSystemOrderMacro(StrongType, buffer, buffer_size) \
    }

#define otbSetTypeLumMacro(WeakType, CAI_VALUE_BE, CAI_VALUE_LE) \
  else if (this->GetComponentType() == WeakType) \
    { \
    if (m_ByteOrder == LittleEndian) \
      { \
      m_TypeLum = CAI_VALUE_LE; \
      } \
    else \
      { \
      m_TypeLum = CAI_VALUE_BE; \
      } \
    }

  bool                        m_FlagWriteImageInformation;
  std::vector<std::string>    m_CaiLumTyp;     //used for read
  std::string                 m_TypeLum; //used for write
  itk::ImageIOBase::ByteOrder m_FileByteOrder;
  std::fstream                m_File;

};

} // end namespace otb

#endif // __otbLUMImageIO_h
