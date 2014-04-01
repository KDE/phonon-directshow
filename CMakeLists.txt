project(PhononDirectShow)
cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Phonon REQUIRED)
macro_ensure_version("4.7.0" ${PHONON_VERSION} PHONON_FOUND)
macro_log_feature(PHONON_FOUND "Phonon" "Core Library" "git://anongit.kde.org/phonon.git" TRUE "4.7.0")

add_definitions(${QT_DEFINITIONS})
include_directories(${PHONON_INCLUDES} ${QT_INCLUDES})

set(PHONON_DS9_MAJOR_VERSION "4")
set(PHONON_DS9_MINOR_VERSION "5")
set(PHONON_DS9_PATCH_VERSION "0")
set(PHONON_DS9_VERSION "${PHONON_DS9_MAJOR_VERSION}.${PHONON_DS9_MINOR_VERSION}.${PHONON_DS9_PATCH_VERSION}")
add_definitions(-DPHONON_DS9_VERSION="${PHONON_DS9_VERSION}")

add_subdirectory(ds9)


macro_display_feature_log()
