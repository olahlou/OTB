/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageSeriesWriter.h,v $
  Language:  C++
  Date:      $Date: 2009-05-11 19:26:54 $
  Version:   $Revision: 1.13 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageSeriesWriter_h
#define __itkImageSeriesWriter_h

#include "itkProcessObject.h"
#include "itkImageIOBase.h"
#include "itkExceptionObject.h"
#include "itkSize.h"
#include "itkImageRegion.h"
#include "itkImageFileWriter.h"
#include <vector>
#include <string>

namespace itk
{

/** \class ImageSeriesWriterException
 *  \brief Base exception class for IO problems during writing. */
class ImageSeriesWriterException : public ExceptionObject 
{
public:
  /** Run-time information. */
  itkTypeMacro( ImageSeriesWriterException, ExceptionObject );

  /** Constructor. */
  ImageSeriesWriterException(char *file, unsigned int line, 
                             const char* message = "Error in IO") : 
    ExceptionObject(file, line)
    {
    SetDescription(message);
    }

  /** Constructor. */
  ImageSeriesWriterException(const std::string &file, unsigned int line, 
                             const char* message = "Error in IO") : 
    ExceptionObject(file, line)
    {
    SetDescription(message);
    }
};


/** \class ImageSeriesWriter
 * \brief Writes image data to a series of data files.
 *
 * ImageSeriesWriter writes its input data to a series of output files.
 * The writer is templated over an input image type and an output
 * image type. Usually, the output image type will have fewer
 * dimensions than the input image type. Each file has a name created
 * using the SeriesFormat. This string is used as a sprintf argument
 * to build a filename. The string should contain zero or one  "%d" or
 * equivalent. The "%d" is an incremental file number that starts at
 * StartIndex and is incremented by IncrementIndex.
 * Since this writer uses an internal instance of an ImageFileWriter,
 * the type of file is determined by either the file extension or an
 * ImageIO class if specified.
 *
 * \sa ImageFileWriter
 * \sa ImageIOBase
 * \sa ImageSeriesReader
 *
 * \ingroup IOFilters 
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ImageSeriesWriter : public ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ImageSeriesWriter         Self;
  typedef ProcessObject             Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageSeriesWriter,ProcessObject);

  /** Some convenient typedefs. */
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::RegionType  InputImageRegionType; 
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType; 
  typedef ImageFileWriter<TOutputImage>        WriterType;
  typedef std::vector< std::string >           FileNamesContainer;

  /** The pixel type of the output image. */
  typedef MetaDataDictionary                   DictionaryType;
  typedef MetaDataDictionary *                 DictionaryRawPointer;
  typedef std::vector< DictionaryRawPointer >  DictionaryArrayType;
  typedef const DictionaryArrayType *          DictionaryArrayRawPointer;
 
  /** Set/Get the image input of this writer.  */
  void SetInput(const InputImageType *input);
  const InputImageType *GetInput(void);
  const InputImageType *GetInput(unsigned int idx);
  
  /** Set/Get the ImageIO helper class. Usually this is created via
   * the object factory mechanism that determines whether a particular
   * ImageIO can write a certain file. This method provides a way to
   * get the ImageIO instance that is created, or to manually set one
   * when the factory mechanism may not work (e.g., for raw files or
   * for non-standard file suffix). */
  itkSetObjectMacro(ImageIO,ImageIOBase);
  itkGetObjectMacro(ImageIO,ImageIOBase);
  
  /** A special version of the Update() method for writers.  It
   * invokes start and end events and handles releasing data. It
   * eventually calls GenerateData() which does the actual writing.
   * The whole image is written. */
  virtual void Write(void);

  /** Aliased to the Write() method to be consistent with the rest of the
   * pipeline. */
  virtual void Update()
    {
    this->Write();
    }
  
  /** Use this method to set the starting index of the series.
   * The default value is 1. */
  itkSetMacro(StartIndex,unsigned long);
  itkGetConstMacro(StartIndex,unsigned long);

  /** Set the increment of the index of the series. The
   * default value is 1.  */
  itkSetMacro(IncrementIndex,unsigned long);
  itkGetConstMacro(IncrementIndex,unsigned long);

  /** The format string used to generate each filename in the
   * series. The filename is built with sprintf(filename, SeriesFormat,
   * number) where number starts at StartIndex and is incremented by
   * IncrementIndex. */
  itkSetStringMacro(SeriesFormat);
  itkGetStringMacro(SeriesFormat);
  
  /** Set/Get the vector of strings that contains the file names. Files
   *  are processed in sequential order. */
  void SetFileNames (const FileNamesContainer &name)
    {
    if ( m_FileNames != name)
      {
      m_FileNames = name;
      this->Modified();
      }
    }
  const FileNamesContainer  & GetFileNames() const
    {
    return m_FileNames;
    }

  /** Set the first file name to be processed. This deletes previous
   * filenames. */
  void SetFileName (std::string const &name)
    {
    m_FileNames.clear();
    m_FileNames.push_back(name);
    this->Modified();
    }

  /** Add a single filename to the list of files. To add a vector of
   * filenames, use the AddFileNames method. */
  void AddFileName (std::string const &name)
    {
    m_FileNames.push_back(name);
    this->Modified();
    }

  /** Set the array of MetaDataDictionaries this is an optinal entry,
   *  mostly intended to be used when writing DICOM slices.  */
  itkSetMacro( MetaDataDictionaryArray, DictionaryArrayRawPointer);
      
  /** Set the compression On or Off */
  itkSetMacro(UseCompression,bool);
  itkGetConstReferenceMacro(UseCompression,bool);
  itkBooleanMacro(UseCompression);

protected:
  ImageSeriesWriter();
  ~ImageSeriesWriter();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Does the real work. */
  void GenerateData(void);

  /** Transition method used for DEPRECATING old functionality.
   *  This method should be removed after release ITK 1.8 */
  void GenerateNumericFileNamesAndWrite(void);
  
  ImageIOBase::Pointer m_ImageIO;
  bool                 m_UserSpecifiedImageIO; //track whether the ImageIO is
                                               // user specified
  
private:
  ImageSeriesWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** A list of filenames to be processed. */
  FileNamesContainer                   m_FileNames;

  /** These variables are used for generating filenames using a numeric
   * approach This functionality is being DEPRECATED since it belongs to a
   * NumericSeriesFileNames class. Removing this functionality from here allows
   * to use additional SeriesFileNames such as the DICOM filenames generators.
   * */
  std::string     m_SeriesFormat;
  unsigned long   m_StartIndex;
  unsigned long   m_IncrementIndex;

  bool m_UseCompression;

  /** Array of MetaDataDictionary used for passing information to each slice */
  DictionaryArrayRawPointer  m_MetaDataDictionaryArray;

  // These two methods provide now a common implementation for the
  // GenerateNumericFileNamesAndWrite() and avoid the duplication of code that
  // was leaving one of the code branches out of date.
  void GenerateNumericFileNames(void);
  void WriteFiles();
};

  
} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageSeriesWriter.txx"
#endif

#endif // __itkImageSeriesWriter_h
