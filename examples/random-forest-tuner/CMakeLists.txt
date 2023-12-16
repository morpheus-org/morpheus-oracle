morpheusoracle_include_directories(${CMAKE_CURRENT_BINARY_DIR})
morpheusoracle_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(Morpheus_ENABLE_CUDA)
  morpheusoracle_add_test_executable(random_forest_tuner_CUDA SOURCES
                                random_forest_tuner.cpp)
  target_compile_definitions(MorpheusOracle_random_forest_tuner_CUDA PRIVATE EXAMPLE_ENABLE_CUDA)
  target_link_libraries(MorpheusOracle_random_forest_tuner_CUDA PUBLIC Morpheus::morpheus)
elseif(Morpheus_ENABLE_HIP)
  morpheusoracle_add_test_executable(random_forest_tuner_HIP SOURCES random_forest_tuner.cpp)
  target_compile_definitions(MorpheusOracle_random_forest_tuner_HIP PRIVATE EXAMPLE_ENABLE_HIP)
  target_link_libraries(MorpheusOracle_random_forest_tuner_HIP PUBLIC Morpheus::morpheus)
elseif(Morpheus_ENABLE_OPENMP)
  morpheusoracle_add_test_executable(random_forest_tuner_OPENMP SOURCES
                                random_forest_tuner.cpp)
  target_compile_definitions(MorpheusOracle_random_forest_tuner_OPENMP
                             PRIVATE EXAMPLE_ENABLE_OPENMP)
  target_link_libraries(MorpheusOracle_random_forest_tuner_OPENMP PUBLIC Morpheus::morpheus)
else()
  morpheusoracle_add_test_executable(random_forest_tuner_SERIAL SOURCES
  random_forest_tuner.cpp)
  target_compile_definitions(MorpheusOracle_random_forest_tuner_SERIAL
                            PRIVATE EXAMPLE_ENABLE_SERIAL)
  target_link_libraries(MorpheusOracle_random_forest_tuner_SERIAL PUBLIC Morpheus::morpheus)
endif()
