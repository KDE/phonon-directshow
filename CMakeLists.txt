project(PhononDirectShow)
cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Phonon REQUIRED)
add_definitions(${QT_DEFINITIONS})
include_directories(${PHONON_INCLUDES} ${QT_INCLUDES})

set(PHONON_DS9_MAJOR_VERSION "4")
set(PHONON_DS9_MINOR_VERSION "4")
set(PHONON_DS9_PATCH_VERSION "4")
set(PHONON_DS9_VERSION "${PHONON_DS9_MAJOR_VERSION}.${PHONON_DS9_MINOR_VERSION}.${PHONON_DS9_PATCH_VERSION}")
add_definitions(-DPHONON_DS9_VERSION="${PHONON_DS9_VERSION}")

if (Q_WS_WIN)
   add_subdirectory(ds9)
endif (Q_WS_WIN)

macro_display_feature_log()
