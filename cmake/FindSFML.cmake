# - Try to find SFML
# Once done this will define
#
#  SFML_FOUND - system has SFML
#  SFML_INCLUDE_DIRS - the SFML include directory
#  SFML_LIBRARIES - Link these to use SFML
#  SFML_DEFINITIONS - Compiler switches required for using SFML
#
#  Copyright (c) 2006-2023 SFML Team
#
#  Redistribution and use is allowed according to the terms of the BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(FindPackageHandleStandardArgs)

# Поиск заголовочных файлов
find_path(SFML_INCLUDE_DIR SFML/System.hpp
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  ${SFML_ROOT}
)

# Поиск библиотек
set(SFML_LIBRARY_COMPONENTS
  audio
  graphics
  network
  system
  window
)

foreach(COMPONENT ${SFML_LIBRARY_COMPONENTS})
  string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
  
  find_library(SFML_${UPPERCOMPONENT}_LIBRARY
    NAMES sfml-${COMPONENT}
    PATH_SUFFIXES lib64 lib bin
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    ${SFML_ROOT}
  )
  
  list(APPEND SFML_LIBRARIES ${SFML_${UPPERCOMPONENT}_LIBRARY})
endforeach()

# Проверка версии
if(SFML_INCLUDE_DIR AND EXISTS "${SFML_INCLUDE_DIR}/SFML/Config.hpp")
  file(STRINGS "${SFML_INCLUDE_DIR}/SFML/Config.hpp" SFML_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SFML_VERSION_MAJOR[ \t]+[0-9]+$")
  file(STRINGS "${SFML_INCLUDE_DIR}/SFML/Config.hpp" SFML_VERSION_MINOR_LINE REGEX "^#define[ \t]+SFML_VERSION_MINOR[ \t]+[0-9]+$")
  file(STRINGS "${SFML_INCLUDE_DIR}/SFML/Config.hpp" SFML_VERSION_PATCH_LINE REGEX "^#define[ \t]+SFML_VERSION_PATCH[ \t]+[0-9]+$")
  
  string(REGEX REPLACE "^#define[ \t]+SFML_VERSION_MAJOR[ \t]+([0-9]+)$" "\\1" SFML_VERSION_MAJOR "${SFML_VERSION_MAJOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SFML_VERSION_MINOR[ \t]+([0-9]+)$" "\\1" SFML_VERSION_MINOR "${SFML_VERSION_MINOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SFML_VERSION_PATCH[ \t]+([0-9]+)$" "\\1" SFML_VERSION_PATCH "${SFML_VERSION_PATCH_LINE}")
  
  set(SFML_VERSION_STRING "${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}.${SFML_VERSION_PATCH}")
endif()

# Обработка найденных компонентов
set(SFML_INCLUDE_DIRS ${SFML_INCLUDE_DIR})
set(SFML_DEFINITIONS "")

find_package_handle_standard_args(SFML
  REQUIRED_VARS SFML_INCLUDE_DIR SFML_LIBRARIES
  VERSION_VAR SFML_VERSION_STRING
  HANDLE_COMPONENTS
)

mark_as_advanced(
  SFML_INCLUDE_DIR
  SFML_LIBRARIES
  SFML_audio_LIBRARY
  SFML_graphics_LIBRARY
  SFML_network_LIBRARY
  SFML_system_LIBRARY
  SFML_window_LIBRARY
)