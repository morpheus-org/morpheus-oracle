get_directory_property(MORPHEUSORACLE_HAS_PARENT PARENT_DIRECTORY)

set(MORPHEUSORACLE_TOP_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(MORPHEUSORACLE_TOP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

if(NOT DEFINED ${PROJECT_NAME})
  project(MorpheusOracle CXX)
endif()
set(MorpheusOracle_VERSION_MAJOR 0)
set(MorpheusOracle_VERSION_MINOR 1)
set(MorpheusOracle_VERSION_PATCH 0)
set(MorpheusOracle_VERSION
    "${MorpheusOracle_VERSION_MAJOR}.${MorpheusOracle_VERSION_MINOR}.${MorpheusOracle_VERSION_PATCH}"
)
math(
  EXPR
  MORPHEUSORACLE_VERSION
  "${MorpheusOracle_VERSION_MAJOR} * 10000 + ${MorpheusOracle_VERSION_MINOR} * 100 + ${MorpheusOracle_VERSION_PATCH}"
)

message(STATUS "Setting policy CMP0074 to use <Package>_ROOT variables")
cmake_policy(SET CMP0074 NEW)

include(GNUInstallDirs)
if(MorpheusOracle_HAS_PARENT)
  set(MORPHEUS_HEADER_INSTALL_DIR "include/morpheus")
  set(MORPHEUS_IS_SUBDIRECTORY TRUE)
else()
  set(MORPHEUS_HEADER_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/morpheus")
  set(MORPHEUS_IS_SUBDIRECTORY FALSE)
endif()

include(cmake/morpheusoracle_utils.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

morpheusoracle_package()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
morpheusoracle_add_option("ENABLE_EXAMPLES" OFF BOOL
                          "Whether to build examples. Default: OFF")
morpheusoracle_add_option("ENABLE_TESTS" OFF BOOL
                          "Whether to build tests. Default: OFF")
morpheusoracle_add_option("ENABLE_DOCS" OFF BOOL
                          "Whether to build docs. Default: OFF")

set(MorpheusOracle_INSTALL_TESTING
    OFF
    CACHE INTERNAL "Whether to build tests and examples against installation")
if(MorpheusOracle_INSTALL_TESTING)
  # Force testing on if we are doing intall testing
  set(MORPHEUSORACLE_ENABLE_TESTS ON)
  set(MORPHEUSORACLE_ENABLE_EXAMPLES ON)
  # Don't build, load installed kernels
  find_package(MorpheusOracle REQUIRED)

  include(cmake/morpheusoracle_gtest.cmake)
  # Only build the tests
  morpheusoracle_add_test_directories(tests)
  morpheusoracle_add_example_directories(examples)
else()
  # Regular build, not install testing Do all the regular option processing
  if(NOT MORPHEUSORACLE_HAS_PARENT)
    # This is a standalone build
    find_package(Morpheus REQUIRED)
    message(STATUS "Found Morpheus at ${Morpheus_DIR}")
  endif()

  add_subdirectory(src)

  if(MorpheusOracle_ENABLE_TESTS OR MorpheusOracle_ENABLE_EXAMPLES)
    include(cmake/morpheusoracle_gtest.cmake)
  endif()

  morpheusoracle_add_test_directories(tests)
  morpheusoracle_add_example_directories(examples)

  morpheusoracle_package_postprocess()
  if(MorpheusOracle_ENABLE_DOCS)
    add_subdirectory(docs)
  endif()
endif()
