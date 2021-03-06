# Sources of non-templated classes.

FILE(GLOB OTBOBIA_SRCS "*.cxx" )


ADD_LIBRARY(OTBOBIA ${OTBOBIA_SRCS})
#Apparently not supported by opensolaris at least
#SET_TARGET_PROPERTIES(OTBOBIA
#    PROPERTIES
#    LINK_INTERFACE_LIBRARIES ""
#)
TARGET_LINK_LIBRARIES (OTBOBIA OTBIO)
IF(OTB_USE_MAPNIK)
    TARGET_LINK_LIBRARIES(OTBOBIA ${MAPNIK_LIBRARY} ${ICUUC_LIBRARY})
ENDIF(OTB_USE_MAPNIK)

IF(OTB_USE_PQXX)
    TARGET_LINK_LIBRARIES(OTBOBIA ${PQ_LIBRARY} ${PQXX_LIBRARY})
ENDIF(OTB_USE_PQXX)

IF(OTB_I18N)
    TARGET_LINK_LIBRARIES(OTBOBIA ${GETTEXT_LIBRARY})
    IF(APPLE OR WIN32)
       TARGET_LINK_LIBRARIES(OTBOBIA ${GETTEXT_INTL_LIBRARY})
    ENDIF(APPLE OR WIN32)
ENDIF(OTB_I18N)

IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBOBIA PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS OTBOBIA
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  FILE(GLOB __files3 "${CMAKE_CURRENT_BINARY_DIR}/*.h")
  INSTALL(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/OBIA
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
