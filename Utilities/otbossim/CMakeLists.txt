PROJECT(otbossim)

SET(OSSIM_MAJOR_VERSION_NUMBER "1")
SET(OSSIM_MINOR_VERSION_NUMBER "8")
SET(OSSIM_PATCH_VERSION_NUMBER "10")
SET(OSSIM_RELEASE_NUMBER "?")
SET(OSSIM_VERSION "${OSSIM_MAJOR_VERSION_NUMBER}-${OSSIM_MINOR_VERSION_NUMBER}-${OSSIM_PATCH_VERSION_NUMBER}" )

# svn revision number of this ossim fork (used for .so versioning)
SET(OSSIM_SVN_REVISION "19791")
# patch number for OTB modifications applied on top of OSSIM_SVN_REVISION (used for .so versioning)
SET(OSSIM_OTB_PATCH "1")

INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE("dirent.h" CMAKE_HAVE_DIRENT_H)
CHECK_INCLUDE_FILE("unistd.h" CMAKE_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE("getopt.h" CMAKE_HAVE_GETOPT_H)
CHECK_INCLUDE_FILE("fcntl.h" CMAKE_HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("dbmalloc.h" CMAKE_HAVE_DBMALLOC_H)
CHECK_INCLUDE_FILE("malloc.h" CMAKE_HAVE_MALLOC_H)
CHECK_INCLUDE_FILE("dlfcn.h" CMAKE_HAVE_DLFCN_H)

INCLUDE_DIRECTORIES(${GEOTIFF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Utilities/otbossim/include)
INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/otbossim/include)

CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h.in
	       ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h)



# SUBDIRS(include)
# ADDED BY OTB DEVELOPPERS TO AVOID INTENSIVE RECOMPILATION
IF(NOT OTB_DISABLE_UTILITIES_COMPILATION)

  FILE(GLOB_RECURSE ossim_init_SRCS "src/ossim/init/*.cpp" "src/ossim/init/*.c")
  FILE(GLOB_RECURSE ossim_base_SRCS "src/ossim/base/*.cpp" "src/ossim/base/*.c")
  FILE(GLOB_RECURSE ossim_elevation_SRCS "src/ossim/elevation/*.cpp" "src/ossim/elevation/*.c")
  FILE(GLOB_RECURSE ossim_font_SRCS "src/ossim/font/*.cpp" "src/ossim/font/*.c")
  FILE(GLOB_RECURSE ossim_imaging_SRCS "src/ossim/imaging/*.cpp" "src/ossim/imaging/*.c")
  FILE(GLOB_RECURSE ossim_matrix_SRCS "src/ossim/matrix/*.cpp" "src/ossim/matrix/*.c")
  FILE(GLOB_RECURSE ossim_parallel_SRCS "src/ossim/parallel/*.cpp" "src/ossim/parallel/*.c")
  FILE(GLOB_RECURSE ossim_projection_SRCS "src/ossim/projection/*.cpp" "src/ossim/projection/*.c")
  FILE(GLOB_RECURSE ossim_support_data_SRCS "src/ossim/support_data/*.cpp" "src/ossim/support_data/*.c")
  FILE(GLOB_RECURSE ossim_util_SRCS "src/ossim/util/*.cpp" "src/ossim/util/*.c")
  FILE(GLOB_RECURSE ossim_vec_SRCS "src/ossim/vec/*.cpp" "src/ossim/vec/*.c")
  FILE(GLOB_RECURSE ossim_vpfutil_SRCS "src/ossim/vpfutil/*.cpp" "src/ossim/vpfutil/*.c")
  FILE(GLOB_RECURSE ossim_plugin_SRCS "src/ossim/plugin/*.cpp" "src/ossim/plugin/*.c")
  FILE(GLOB_RECURSE ossim_kbool_SRCS "src/ossim/kbool/*.cpp" "src/ossim/kbool/*.c")
  FILE(GLOB_RECURSE ossim_dll_main_SRCS "src/ossim/dll_main/*.cpp")

  # Removing empty source file causing compilation warning on visual
  LIST(REMOVE_ITEM ossim_support_data_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/support_data/ossimDemPoint.cpp")
  LIST(REMOVE_ITEM ossim_plugin_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/plugin/ossimSharedObjectBridge.cpp")
  LIST(REMOVE_ITEM ossim_base_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/base/ossimFactoryBaseTemplate.cpp")

  # Removing sources not in use any more (switched to ossim plugins)
  LIST(REMOVE_ITEM ossim_projection_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/projection/ossimCosmoSkymedModel.cpp")


  IF(NOT OTB_BUILD_SHARED_LIB)
    IF(WIN32)
      IF(NOT CYGWIN)
        IF(NOT MINGW)
        SET(ossim_init_SRCS ${ossim_init_SRCS} ${ossim_dll_main_SRCS})
        ENDIF(NOT MINGW)
      ENDIF(NOT CYGWIN)
    ENDIF(WIN32)
  ENDIF(NOT OTB_BUILD_SHARED_LIB)


  ADD_EXECUTABLE(version-config src/ossim/version-config.cpp)

  ADD_CUSTOM_COMMAND(
          OUTPUT ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h
          COMMAND version-config
          ARGS "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h" "${OSSIM_VERSION}"
          DEPENDS version-config
          COMMENT "Generating ossimVersion.h" )

  SET_SOURCE_FILES_PROPERTIES(
      src/ossim/init/ossimInit.cpp PROPERTIES
      OBJECT_DEPENDS "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimVersion.h"
      )

  #Specify that we are making DLL here
  IF(NOT BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DNOMINMAX -DOSSIMMAKINGDLL)
  ENDIF(NOT BUILD_SHARED_LIBS)

  ADD_LIBRARY(otbossim
  ${ossim_init_SRCS}
  ${ossim_base_SRCS}
  ${ossim_kbool_SRCS}
  ${ossim_matrix_SRCS}
  ${ossim_util_SRCS}
  ${ossim_vec_SRCS}
  ${ossim_vpfutil_SRCS}
  ${ossim_plugin_SRCS}
  ${ossim_font_SRCS}
  ${ossim_support_data_SRCS}
  ${ossim_projection_SRCS}
  ${ossim_imaging_SRCS}
  ${ossim_parallel_SRCS}
  ${ossim_elevation_SRCS})

  #Apparently not supported by opensolaris at least
#  SET_TARGET_PROPERTIES(otbossim
#    PROPERTIES
#    LINK_INTERFACE_LIBRARIES ""
#  )
  #TARGET_LINK_LIBRARIES(otbossim ${GDAL_LIBRARY})#To make sure that gdal appear before geotiff
  TARGET_LINK_LIBRARIES(otbossim  ${GEOTIFF_LIBRARY} ${TIFF_LIBRARY} ${JPEG_LIBRARY} ${GDAL_LIBRARY} ${OPENTHREADS_LIBRARY})
  SET_TARGET_PROPERTIES(otbossim 
                        PROPERTIES VERSION "${OSSIM_SVN_REVISION}.${OSSIM_OTB_PATCH}.0"
                                   SOVERSION "${OSSIM_SVN_REVISION}.${OSSIM_OTB_PATCH}")

  IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbossim
          RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
          LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
          ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
  ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

ENDIF(NOT OTB_DISABLE_UTILITIES_COMPILATION)

FILE(GLOB ossim_base_HDRS "include/ossim/base/*.h")
FILE(GLOB ossim_elevation_HDRS "include/ossim/elevation/*.h")
FILE(GLOB ossim_font_HDRS "include/ossim/font/*.h")
FILE(GLOB_RECURSE ossim_imaging_HDRS "include/ossim/imaging/*.h")
FILE(GLOB ossim_init_HDRS "include/ossim/init/*.h")
FILE(GLOB ossim_kbool_HDRS "include/ossim/kbool/*.h")
FILE(GLOB ossim_matrix_HDRS "include/ossim/matrix/*.h")
FILE(GLOB ossim_parallel_HDRS "include/ossim/parallel/*.h")
FILE(GLOB ossim_plugin_HDRS "include/ossim/plugin/*.h")
FILE(GLOB ossim_projection_HDRS "include/ossim/projection/*.h")
FILE(GLOB ossim_support_data_HDRS "include/ossim/support_data/*.h")
FILE(GLOB ossim_util_HDRS "include/ossim/util/*.h")
FILE(GLOB ossim_vec_HDRS "include/ossim/vec/*.h")
FILE(GLOB ossim_vpfutil_HDRS "include/ossim/vpfutil/*.h")

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)

INSTALL(FILES ${ossim_base_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/base
    COMPONENT Development)
INSTALL(FILES ${ossim_elevation_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/elevation
    COMPONENT Development)
INSTALL(FILES ${ossim_font_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/font
    COMPONENT Development)
INSTALL(FILES ${ossim_imaging_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/imaging
    COMPONENT Development)
INSTALL(FILES ${ossim_init_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/init
    COMPONENT Development)
INSTALL(FILES ${ossim_kbool_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/kbool
    COMPONENT Development)
INSTALL(FILES ${ossim_matrix_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/matrix
    COMPONENT Development)
INSTALL(FILES ${ossim_parallel_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/parallel
    COMPONENT Development)
INSTALL(FILES ${ossim_plugin_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/plugin
    COMPONENT Development)
INSTALL(FILES ${ossim_projection_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/projection
    COMPONENT Development)
INSTALL(FILES ${ossim_support_data_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/support_data
    COMPONENT Development)
INSTALL(FILES ${ossim_util_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/util
    COMPONENT Development)
INSTALL(FILES ${ossim_vec_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/vec
    COMPONENT Development)
INSTALL(FILES ${ossim_vpfutil_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim/vpfutil
    COMPONENT Development)

SET(ossim_config_HDR "${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h" )

INSTALL(FILES ${ossim_config_HDR}
        DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbossim/include/ossim
        COMPONENT Development)

ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
