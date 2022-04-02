#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(de.mrh.launcher VERSION 1.0.0
                        DESCRIPTION "MRH application launcher"
                        LANGUAGES CXX)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Binary Paths
#  ------------
#  The paths for our created binary file(s).
###
set(BIN_DIR_PATH "${CMAKE_SOURCE_DIR}/bin/")
file(MAKE_DIRECTORY ${BIN_DIR_PATH})

###
#  Build Paths
#  -----------
#  The paths for the cmake build.
###
set(BUILD_DIR_PATH "${CMAKE_SOURCE_DIR}/build/")
file(MAKE_DIRECTORY ${BUILD_DIR_PATH})

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")
             
set(SRC_LIST_APP "${SRC_DIR_PATH}/Revision.h"
                 "${SRC_DIR_PATH}/Main.cpp")
                 
set(SRC_LIST_MODULE "${SRC_DIR_PATH}/Module/CheckService.cpp"
                    "${SRC_DIR_PATH}/Module/CheckService.h"
                    "${SRC_DIR_PATH}/Module/SpeechOutput.cpp"
                    "${SRC_DIR_PATH}/Module/SpeechOutput.h"
                    "${SRC_DIR_PATH}/Module/SpeechInput.cpp"
                    "${SRC_DIR_PATH}/Module/SpeechInput.h"
                    "${SRC_DIR_PATH}/Module/LaunchPackage.cpp"
                    "${SRC_DIR_PATH}/Module/LaunchPackage.h"
                    "${SRC_DIR_PATH}/Module/Launcher.cpp"
                    "${SRC_DIR_PATH}/Module/Launcher.h")
                    
set(SRC_LIST_PACKAGE "${SRC_DIR_PATH}/Package/PackageList.cpp"
                     "${SRC_DIR_PATH}/Package/PackageList.h"
                     "${SRC_DIR_PATH}/Package/Package.cpp"
                     "${SRC_DIR_PATH}/Package/Package.h")

#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
#
#  NOTE: 
#  MRH applications are required to be named App.so, 
#  and application services Service.so!
#  They are build as shared objects.
###
add_library(MRH_App SHARED ${SRC_LIST_APP}
                           ${SRC_LIST_MODULE}
                           ${SRC_LIST_PACKAGE})
set_target_properties(MRH_App
                      PROPERTIES
                      PREFIX ""
                      OUTPUT_NAME "App"
                      SUFFIX ".so"
                      ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
                      RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})

###
#  Required Libraries
#  ------------------
#  Libraries required by this application.
###
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)
find_library(libmrh NAMES mrh REQUIRED)
find_library(libmrhbf NAMES mrhbf REQUIRED)
find_library(libmrhevdata NAMES mrhevdata REQUIRED)
find_library(libmrhab NAMES mrhab REQUIRED)
find_library(libmrhvt NAMES mrhvt REQUIRED)

target_link_libraries(MRH_App PUBLIC Threads::Threads)
target_link_libraries(MRH_App PUBLIC mrh)
target_link_libraries(MRH_App PUBLIC mrhbf)
target_link_libraries(MRH_App PUBLIC mrhevdata)
target_link_libraries(MRH_App PUBLIC mrhab)
target_link_libraries(MRH_App PUBLIC mrhvt)

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###
target_compile_definitions(MRH_App PRIVATE PACKAGE_LIST_PATH="/usr/local/etc/mrh/MRH_PackageList.conf")
target_compile_definitions(MRH_App PRIVATE SPEECH_OUTPUT_DIR="Output")
target_compile_definitions(MRH_App PRIVATE SPEECH_OUTPUT_LIST_PACKAGE_FILE="ListPackages.mrhog")
target_compile_definitions(MRH_App PRIVATE SPEECH_OUTPUT_NO_PACKAGE_FILE="NoPackages.mrhog")