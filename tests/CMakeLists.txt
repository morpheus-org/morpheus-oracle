message(STATUS "Morpheus-Oracle Tests are enabled")

morpheusoracle_include_directories(${CMAKE_CURRENT_BINARY_DIR})
morpheusoracle_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

global_set(TEST_SOURCES)

set(testdir ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${testdir})

foreach(Name RunFirstTuner TuneRunFirst TypeTraits)
  set(file ${testdir}/Test_${Name}.cpp)
  # Write to a temporary intermediate file and call configure_file to avoid
  # updating timestamps triggering unnecessary rebuilds on subsequent cmake
  # runs.
  file(WRITE ${testdir}/dummy.cpp "#include <Test_${Name}.hpp>\n")
  configure_file(${testdir}/dummy.cpp ${file})
  global_append(TEST_SOURCES ${file})
endforeach()

morpheusoracle_add_executable_and_test(
  UnitTest SOURCES TestMain.cpp ${TEST_SOURCES} TESTONLYLIBS
  morpheusoracle_gtest)

