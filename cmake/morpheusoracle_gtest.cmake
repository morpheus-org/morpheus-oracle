message(STATUS "Setting up MorpheusOracle_Gtest library")

find_package(GTest CONFIG)
if(NOT GTest_FOUND)
  message(STATUS "find_package could not find GTest - Downloading GTest")
  include(FetchContent)
  FetchContent_Declare(
    googletest
    # Specify the commit you depend on and update it regularly.
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker
  # settings
  set(gtest_force_shared_crt
      ON
      CACHE BOOL "" FORCE)
  set(INSTALL_GTEST
      OFF
      CACHE BOOL "" FORCE)
  set(BUILD_GMOCK
      OFF
      CACHE BOOL "" FORCE)
  set(gtest_disable_pthreads
      ON
      CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  add_library(morpheusoracle_gtest ALIAS gtest_main)
else()
  set_target_properties(GTest::gtest PROPERTIES IMPORTED_GLOBAL TRUE)
  add_library(morpheusoracle_gtest ALIAS GTest::gtest)
endif()

set(MorpheusOracle_ENABLE_INDIVIDUAL_TESTS
    OFF
    CACHE INTERNAL "Whether to enable individual unit tests. Default is OFF.")

message(STATUS "MorpheusOracle_Gtest Library configured.")
