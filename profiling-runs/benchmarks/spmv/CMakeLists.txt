include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

option(BENCHMARK_ENABLE_GENERIC "Enable Morpheus Generic backends" OFF)
option(BENCHMARK_ENABLE_SERIAL "Enable Serial Execution Space" OFF)
option(BENCHMARK_ENABLE_OPENMP "Enable OpenMP Execution Space" OFF)
option(BENCHMARK_ENABLE_CUDA "Enable Cuda Execution Space" OFF)
option(BENCHMARK_ENABLE_HIP "Enable HIP Execution Space" OFF)

morpheusoracle_add_executable_and_test(SpMV SOURCES spmv.cpp)

if(BENCHMARK_ENABLE_GENERIC)
  message(
    STATUS "Morpheus-Oracle: Profiling Runs are enabled with Generic Backend")
  target_compile_definitions(MorpheusOracle_SpMV PRIVATE Oracle_ENABLE_GENERIC)
endif()

if(BENCHMARK_ENABLE_OPENMP)
  message(
    STATUS
      "Morpheus-Oracle: Profiling Runs are enabled with OpenMP Execution Space")
  target_compile_definitions(MorpheusOracle_SpMV PRIVATE Oracle_ENABLE_OPENMP)
elseif(BENCHMARK_ENABLE_CUDA)
  message(
    STATUS
      "Morpheus-Oracle: Profiling Runs are enabled with Cuda Execution Space")
  target_compile_definitions(MorpheusOracle_SpMV PRIVATE Oracle_ENABLE_CUDA)
elseif(BENCHMARK_ENABLE_HIP)
  message(
    STATUS
      "Morpheus-Oracle: Profiling Runs are enabled with HIP Execution Space")
  target_compile_definitions(MorpheusOracle_SpMV PRIVATE Oracle_ENABLE_HIP)
else()
  message(
    STATUS
      "Morpheus-Oracle: Profiling Runs are enabled with Serial Execution Space")
  target_compile_definitions(MorpheusOracle_SpMV PRIVATE Oracle_ENABLE_SERIAL)
endif()
