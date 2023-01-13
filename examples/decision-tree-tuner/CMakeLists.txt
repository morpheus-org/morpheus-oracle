morpheusoracle_include_directories(${CMAKE_CURRENT_BINARY_DIR})
morpheusoracle_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(Morpheus_ENABLE_SERIAL)
  morpheusoracle_add_executable(decision_tree_tuner_SERIAL SOURCES
  decision_tree_tuner.cpp)
  target_compile_definitions(decision_tree_tuner_SERIAL
                             PRIVATE EXAMPLE_ENABLE_SERIAL)
elseif(Morpheus_ENABLE_OPENMP)
  morpheusoracle_add_executable(decision_tree_tuner_OPENMP SOURCES
                                decision_tree_tuner.cpp)
  target_compile_definitions(decision_tree_tuner_OPENMP
                             PRIVATE EXAMPLE_ENABLE_OPENMP)
elseif(Morpheus_ENABLE_CUDA)
  morpheusoracle_add_executable(decision_tree_tuner_CUDA SOURCES
                                decision_tree_tuner.cpp)
  target_compile_definitions(decision_tree_tuner_CUDA PRIVATE EXAMPLE_ENABLE_CUDA)
elseif(Morpheus_ENABLE_HIP)
  morpheusoracle_add_executable(decision_tree_tuner_HIP SOURCES decision_tree_tuner.cpp)
  target_compile_definitions(decision_tree_tuner_HIP PRIVATE EXAMPLE_ENABLE_HIP)
endif()