#!/bin/sh
# build.sh
#
# EPCC, The University of Edinburgh
#
# (c) 2021 - 2023 The University of Edinburgh
#
# Contributing Authors:
# Christodoulos Stylianou (c.stylianou@ed.ac.uk)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Builds Morpheus Benchmark on the available systems and queues.
SCRIPT_PATH="$(
  cd -- "$(dirname "$0")" >/dev/null 2>&1
  pwd -P
)"

if [ "$#" -lt "3" ]; then
  echo -e "Script requires 3 runtime arguments to run."
  echo -e "\t\$1 : Backend [cuda]"
  echo -e "\t\$2 : A valid path to Morpheus-Oracle Installation"
  echo -e "\t\$3 : A valid path to Kokkos Installation"
  exit 0
fi

backend=$1
morpheus_oracle_dir=$2
kokkos_dir=$3
queue="instinctq"

if [ ! -d "$morpheus_oracle_dir" ];then
  echo "Path to Morpheus-Oracle Installation ($morpheus_oracle_dir) does not exist!"
  exit 1
fi

if [ ! -d "$kokkos_dir" ];then
  echo "Path to Kokkos Installation ($kokkos_dir) does not exist!"
  exit 1
fi

EXPERIMENT_ROOT_DIR=$SCRIPT_PATH/../../..
BUILD_PATH=$SCRIPT_PATH/build/$backend

echo -e "Building tuners_comparison routine:"
echo -e "\tScript Path              : $SCRIPT_PATH"
echo -e "\tProfiling Run Root Path  : $EXPERIMENT_ROOT_DIR"
echo -e "\tBuild Path               : $BUILD_PATH"
echo -e "\tBackend : $backend"
echo -e "\tQueue   : $queue"
echo -e "\tMorpheus-Oracle Installation : $morpheus_oracle_dir"
echo -e "\tKokkos Installation : $kokkos_dir"

rm -rf $BUILD_PATH && mkdir -p $BUILD_PATH

EXPERIMENT_INSTALL_DIR=$SCRIPT_PATH/installs/$backend

module use /software/x86/tools/nvidia/hpc_sdk/modulefiles
module load nvhpc/22.9 gcc/10.3.0
module use -a /lustre/projects/bristol/modules/modulefiles
module load cmake
CXX_COMPILER=$kokkos_dir/bin/nvcc_wrapper

BACKEND_SERIAL="ON"
BACKEND_CUDA="ON"

mkdir -p $BUILD_PATH && cd $BUILD_PATH
cmake $EXPERIMENT_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                           -DCMAKE_BUILD_TYPE=Release \
                           -DCMAKE_INSTALL_PREFIX=${EXPERIMENT_INSTALL_DIR} \
                           -DCMAKE_CXX_EXTENSIONS=Off \
                           -DMorpheusOracle_ROOT=${morpheus_oracle_dir} \
                           -DMorpheusOracleExperiments_ENABLE_SERIAL=${BACKEND_SERIAL} \
                           -DMorpheusOracleExperiments_ENABLE_CUDA=${BACKEND_CUDA} && make -j