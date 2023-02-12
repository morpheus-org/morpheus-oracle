include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MorpheusOracleExperiments_decision-tree decision-tree.cpp)
add_executable(MorpheusOracleExperiments_random-forest random-forest.cpp)

if(MorpheusOracleExperiments_ENABLE_GENERIC)
  message(STATUS "MorpheusOracleExperiments: tuners_comparison are enabled with Generic Backend")
  target_compile_definitions(MorpheusOracleExperiments_decision-tree PRIVATE EXAMPLE_ENABLE_GENERIC)
  target_compile_definitions(MorpheusOracleExperiments_random-forest PRIVATE EXAMPLE_ENABLE_GENERIC)
endif()

if(MorpheusOracleExperiments_ENABLE_OPENMP)
  message(
    STATUS "MorpheusOracleExperiments: tuners_comparison are enabled with OpenMP Execution Space")
    target_compile_definitions(MorpheusOracleExperiments_decision-tree PRIVATE EXAMPLE_ENABLE_OPENMP)
    target_compile_definitions(MorpheusOracleExperiments_random-forest PRIVATE EXAMPLE_ENABLE_OPENMP)
elseif(MorpheusOracleExperiments_ENABLE_CUDA)
  message(
    STATUS "MorpheusOracleExperiments: tuners_comparison are enabled with Cuda Execution Space")
    target_compile_definitions(MorpheusOracleExperiments_decision-tree PRIVATE EXAMPLE_ENABLE_CUDA)
    target_compile_definitions(MorpheusOracleExperiments_random-forest PRIVATE EXAMPLE_ENABLE_CUDA)
elseif(MorpheusOracleExperiments_ENABLE_HIP)
  message(
    STATUS "MorpheusOracleExperiments: tuners_comparison are enabled with HIP Execution Space")
    target_compile_definitions(MorpheusOracleExperiments_decision-tree PRIVATE EXAMPLE_ENABLE_HIP)
    target_compile_definitions(MorpheusOracleExperiments_random-forest PRIVATE EXAMPLE_ENABLE_HIP)
else()
  message(
    STATUS "MorpheusOracleExperiments: tuners_comparison are enabled with Serial Execution Space")
    target_compile_definitions(MorpheusOracleExperiments_decision-tree PRIVATE EXAMPLE_ENABLE_SERIAL)
    target_compile_definitions(MorpheusOracleExperiments_random-forest PRIVATE EXAMPLE_ENABLE_SERIAL)
endif()

target_link_libraries(MorpheusOracleExperiments_decision-tree Morpheus::morpheus-oracle)
target_link_libraries(MorpheusOracleExperiments_random-forest Morpheus::morpheus-oracle)