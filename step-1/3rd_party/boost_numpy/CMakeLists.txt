cmake_minimum_required(VERSION 3.15)

project( boost.numpy )

set(BUILD_TESTS ON CACHE BOOL "Build Boost.NumPy Tests")
set(BUILD_EXAMPLES ON CACHE BOOL "Build Boost.NumPy Examples")

# put our local cmake find scripts at the beginning of the cmake
# module search path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/libs/numpy/cmake ${CMAKE_MODULE_PATH})

IF (NOT DEFINED LIB_SUFFIX)
	EXECUTE_PROCESS(
		COMMAND uname -m
		COMMAND tr -d '\n'
		OUTPUT_VARIABLE ARCH
	)
	message(STATUS "Detected architecture '"  ${ARCH}  "'")
	IF (ARCH STREQUAL x86_64)
		SET(LIB_SUFFIX 64)
	ENDIF (ARCH STREQUAL x86_64)
ENDIF (NOT DEFINED LIB_SUFFIX)

# configure output folders so artifacts are built into a single set of
# top-level folders rather than the default cmake build structure that
# matches the source tree.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib 
    CACHE PATH "Output directory for static libraries.")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for shared libraries.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CACHE PATH "Output directory for executables and DLL's.")

# find required python packages
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(NumPy REQUIRED)

# find boost
# 
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_MACOSX_RPATH 1)

if(${PYTHON_VERSION_STRING} GREATER 3.0)
  message(STATUS "Using Python3")
  find_package(Boost COMPONENTS python3 REQUIRED)
else()
  message(STATUS "Using Python2")
  find_package(Boost COMPONENTS python REQUIRED)
endif()

message( STATUS "Boost Paths:")
message( STATUS "Include  : " ${Boost_INCLUDE_DIRS})
message( STATUS "Libraries: " ${Boost_LIBRARIES})

if(MSVC)
  # With Visual Studio remove the automatic linking
  # feature
  add_definitions( -DBOOST_ALL_NO_LIB )
endif()

if(Boost_USE_STATIC_LIBS)
  # if the user specified he wants to link to static libraries
  # force linking to static boost python
  add_definitions( -DBOOST_PYTHON_STATIC_LIB )
endif()
# In some cases, you may need to explicitly specify that a dynamic Boost is used; if so use:
add_definitions( -DBOOST_ALL_DYN_LINK )

# variable controlling whether the boost_numpy is a shared or static library
if (WIN32)
  set(LIBRARY_TYPE STATIC CACHE STRING "type of library to make for boost_numpy")
else()
  set(LIBRARY_TYPE SHARED CACHE STRING "type of library to make for boost_numpy")
endif()

# variable controlling building of documentation
set(BUILD_DOCS OFF CACHE BOOL "Build Boost.NumPy Documentation")

# logic for configuring documentation generation
if(BUILD_DOCS)
  # find sphinx
  EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sphinx; print sphinx.__version__"
    RESULT_VARIABLE SPHINX_PROCESS
    OUTPUT_VARIABLE SPHINX_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  
  set(HAVE_SPHINX 0)
  if(SPHINX_PROCESS EQUAL 0)
    FIND_PROGRAM(SPHINX_BUILD sphinx-build)
    if(SPHINX_BUILD)
      set(HAVE_SPHINX 1)
      message(STATUS "    Found Sphinx ${SPHINX_VERSION}: ${SPHINX_BUILD}")
    else()
      # sphinx is required to generate documention, so it is an error
      # if we cannot find it
      MESSAGE(SEND_ERROR "must be able to find sphinx-build when BUILD_DOCS is enabled")
    endif()
  endif()
  
  # find pdflatex, which is only required for doc-pdf
  FIND_PACKAGE(LATEX)
  if (PDFLATEX_COMPILER)
    message( STATUS "Found PDFLATEX_COMPILER=${PDFLATEX_COMPILER}" )
  else()
    message( STATUS "Found PDFLATEX_COMPILER NOT found" )
  endif()
endif()

# compiler definitions for non-windows builds
if (NOT WIN32)
  add_definitions(-fPIC)
endif()

if (BUILD_TESTS)
  # enable ctest targets
  ENABLE_TESTING()
endif()

# turn on visual studio solution folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# global settings for include paths
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${NUMPY_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS} 
  )

# install headers
install(DIRECTORY boost
    DESTINATION "include"
    FILES_MATCHING
    PATTERN "*.hpp"
    ${INSTALL_PERMISSIONS_SRC}
  )

# add submodules
ADD_SUBDIRECTORY(libs/numpy/src)
if (BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(libs/numpy/example)
endif()
if (BUILD_TESTS)
  ADD_SUBDIRECTORY(libs/numpy/test)
endif()

if (BUILD_DOCS)
  ADD_SUBDIRECTORY(libs/numpy/doc)
endif()


