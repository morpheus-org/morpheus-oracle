#!/bin/bash --login

SCRIPT_PATH=$1
EXPERIMENT_ROOT_PATH=$2
BUILD_PATH=$3
BACKEND=$4
MORPHEUSORACLE_PATH=$5
KOKKOS_PATH=$6

EXPERIMENT_INSTALL_DIR=$SCRIPT_PATH/installs/$BACKEND

# Load CMake and GCC
module load cmake gcc/10.2.0

BACKEND_SERIAL="ON"
BACKEND_OPENMP="OFF"
BACKEND_CUDA="OFF"

CXX_COMPILER=$(which g++)
CUDA_ROOT=
if [ "openmp" == "$BACKEND" ]; then
  BACKEND_OPENMP="ON"
  WITH_CUDA=""
elif [ "cuda" == "$BACKEND" ]; then
  BACKEND_CUDA="ON"
  module unload gcc
  module load nvidia/nvhpc-nompi/22.2
  CXX_COMPILER=$KOKKOS_PATH/bin/nvcc_wrapper
fi

mkdir -p $BUILD_PATH && cd $BUILD_PATH
cmake $EXPERIMENT_ROOT_PATH -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                           -DCMAKE_BUILD_TYPE=Release \
                           -DCMAKE_INSTALL_PREFIX=${EXPERIMENT_INSTALL_DIR} \
                           -DCMAKE_CXX_EXTENSIONS=Off \
                           -DMorpheusOracle_ROOT=${MORPHEUSORACLE_PATH} \
                           -DMorpheusOracleExperiments_ENABLE_SERIAL=${BACKEND_SERIAL} \
                           -DMorpheusOracleExperiments_ENABLE_OPENMP=${BACKEND_OPENMP} \
                           -DMorpheusOracleExperiments_ENABLE_CUDA=${BACKEND_CUDA} && make -j
