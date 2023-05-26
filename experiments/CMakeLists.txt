project(MorpheusOracleExperiments CXX)

cmake_minimum_required(VERSION 3.16)

find_package(MorpheusOracle REQUIRED)
message(STATUS "Found Morpheus at ${MorpheusOracle_DIR}")

option(MorpheusOracleExperiments_ENABLE_GENERIC "Enable Morpheus Generic backends" OFF)
option(MorpheusOracleExperiments_ENABLE_SERIAL "Enable Serial Execution Space" OFF)
option(MorpheusOracleExperiments_ENABLE_OPENMP "Enable OpenMP Execution Space" OFF)
option(MorpheusOracleExperiments_ENABLE_CUDA "Enable Cuda Execution Space" OFF)
option(MorpheusOracleExperiments_ENABLE_HIP "Enable HIP Execution Space" OFF)

add_subdirectory(tuners_comparison)