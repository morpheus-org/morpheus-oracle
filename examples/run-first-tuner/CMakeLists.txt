morpheusoracle_include_directories(${CMAKE_CURRENT_BINARY_DIR})
morpheusoracle_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(Morpheus_ENABLE_CUDA)
  morpheusoracle_add_test_executable(run_first_tuner_CUDA SOURCES
                                run_first_tuner.cpp)
  target_compile_definitions(MorpheusOracle_run_first_tuner_CUDA PRIVATE EXAMPLE_ENABLE_CUDA)
elseif(Morpheus_ENABLE_HIP)
  morpheusoracle_add_test_executable(run_first_tuner_HIP SOURCES run_first_tuner.cpp)
  target_compile_definitions(MorpheusOracle_run_first_tuner_HIP PRIVATE EXAMPLE_ENABLE_HIP)
elseif(Morpheus_ENABLE_OPENMP)
  morpheusoracle_add_test_executable(run_first_tuner_OPENMP SOURCES
                                run_first_tuner.cpp)
  target_compile_definitions(MorpheusOracle_run_first_tuner_OPENMP
                             PRIVATE EXAMPLE_ENABLE_OPENMP)
else()
  morpheusoracle_add_test_executable(run_first_tuner_SERIAL SOURCES
                                run_first_tuner.cpp)
  target_compile_definitions(MorpheusOracle_run_first_tuner_SERIAL
                             PRIVATE EXAMPLE_ENABLE_SERIAL)
endif()
