# Downloaded 24 December 2009
# from http://code.google.com/p/inc/source/browse/parser/trunk/cmake/FindGoogleTest.cmake

# Locate and configure the Google Test libraries.
#
# Defines the following variable:
#
#   GTEST_FOUND - Found the Google Test libraries
#   GTEST_INCLUDE_DIRS - The directories needed on the include paths
#   GTEST_LIBRARIES - The libraries to link to test executables
#   GTEST_MAIN_LIBRARIES - The libraries to link for automatic main() provision
#
#	Copyright 2008 Chandler Carruth
#
#	Licensed under the Apache License, Version 2.0 (the "License"); you may not
#	use this file except in compliance with the License.  You may obtain a copy
#	of the License at
#
#		http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#	WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
#	License for the specific language governing permissions and limitations
#	under the License.

if(GTEST_INCLUDE_DIRS AND GTEST_LIBRARIES AND GTEST_MAIN_LIBRARIES)
  set(GTEST_FOUND true)
else(GTEST_INCLUDE_DIRS AND GTEST_LIBRARIES AND GTEST_MAIN_LIBRARIES)
  set(GTEST_PREFIX "" CACHE PATH "Installation prefix for Google Test")
  if(GTEST_PREFIX)
    find_path(_GTEST_INCLUDE_DIR gtest.h
      PATHS "${GTEST_PREFIX}/include"
      PATH_SUFFIXES "gtest"
      NO_DEFAULT_PATH)
    find_library(_GTEST_LIBRARY gtest
      PATHS "${GTEST_PREFIX}/lib"
      NO_DEFAULT_PATH)
    find_library(_GTEST_MAIN_LIBRARY gtest_main
      PATHS "${GTEST_PREFIX}/lib"
      NO_DEFAULT_PATH)
  else(GTEST_PREFIX)
    find_path(_GTEST_INCLUDE_DIR gtest.h
      PATH_SUFFIXES "gtest")
    find_library(_GTEST_LIBRARY gtest)
    find_library(_GTEST_MAIN_LIBRARY gtest_main)
  endif(GTEST_PREFIX)
  if(_GTEST_INCLUDE_DIR AND _GTEST_LIBRARY AND _GTEST_MAIN_LIBRARY)
    set(GTEST_FOUND true)
    set(GTEST_INCLUDE_DIRS ${_GTEST_INCLUDE_DIR} CACHE PATH
      "Include directories for Google Test framework")
    set(GTEST_LIBRARIES ${_GTEST_LIBRARY} CACHE FILEPATH
      "Libraries to link for Google Test framework")
    set(GTEST_MAIN_LIBRARIES ${_GTEST_MAIN_LIBRARY} CACHE FILEPATH
      "Libraries to link for Google Test automatic main() definition")
    mark_as_advanced(GTEST_INCLUDE_DIRS GTEST_LIBRARIES GTEST_MAIN_LIBRARIES)
    if(NOT GoogleTest_FIND_QUIETLY)
      message(STATUS "Found Google Test: ${GTEST_LIBRARIES}")
    endif(NOT GoogleTest_FIND_QUIETLY)
  else(_GTEST_INCLUDE_DIR AND _GTEST_LIBRARY AND _GTEST_MAIN_LIBRARY)
    if(GoogleTest_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find the Google Test framework")
    endif(GoogleTest_FIND_REQUIRED)
  endif(_GTEST_INCLUDE_DIR AND _GTEST_LIBRARY AND _GTEST_MAIN_LIBRARY)
endif(GTEST_INCLUDE_DIRS AND GTEST_LIBRARIES AND GTEST_MAIN_LIBRARIES)
