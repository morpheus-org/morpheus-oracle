# P3 - Instinct
## GPU
1. To start an Interactive Job:
```sh
qsub -I -q instinctq -l select=1:ncpus=32 -l place=excl
```
*WARNING:* that compilation for the AMD GPUS can only be done on the login nodes of P3 system.

2. Load Environment (LOGIN NODE):
```sh
module use -a /lustre/projects/bristol/modules/modulefiles
module load PrgEnv-amd
module load cmake
WORK_DIR=/lustre/home/ri-cstylianou
CXX_COMPILER=$(which hipcc)
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/hip-4.5-on-on
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/hip-4.5-on-on
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/hip-4.5-on-on
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/hip-4.5-on-on
MORPHEUSORACLE_ROOT_DIR=$WORK_DIR/morpheus-oracle
MORPHEUSORACLE_BUILD_DIR=$WORK_DIR/morpheus-oracle/builds/hip-4.5-on-on
MORPHEUSORACLE_INSTALL_DIR=$WORK_DIR/morpheus-oracle/installs/hip-4.5-on-on

export CRAYPE_LINK_TYPE=dynamic
```

## Kokkos
```sh
rm -rf $KOKKOS_BUILD_DIR && mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DKokkos_ENABLE_HIP=ON \
                       -DKokkos_ENABLE_CUDA=OFF \
                       -DKokkos_ENABLE_OPENMP=OFF \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_ZEN3=ON \
                       -DKokkos_ARCH_VEGA908=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_CUDA_CONSTEXPR=OFF \
                       -DKokkos_ENABLE_CUDA_LAMBDA=OFF \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_DEBUG_BOUNDS_CHECK=OFF && make -j && make install
```

## Morpheus
``` sh
rm -rf $MORPHEUS_BUILD_DIR && mkdir -p $MORPHEUS_BUILD_DIR && cd $MORPHEUS_BUILD_DIR
cmake $MORPHEUS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUS_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DKokkos_ROOT=${KOKKOS_INSTALL_DIR} \
                         -DMorpheus_ENABLE_EXAMPLES=OFF \
                         -DMorpheus_ENABLE_TESTS=OFF && make -j && make install
```

## Morpheus-Oracle
``` sh
rm -rf $MORPHEUSORACLE_BUILD_DIR && mkdir -p $MORPHEUSORACLE_BUILD_DIR && cd $MORPHEUSORACLE_BUILD_DIR
cmake $MORPHEUSORACLE_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUSORACLE_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DMorpheus_ROOT=${MORPHEUS_INSTALL_DIR} \
                         -DMorpheusOracle_ENABLE_EXAMPLES=OFF \
                         -DMorpheusOracle_ENABLE_TESTS=OFF && make -j && make install
```

# P3 - Ampere
## GPU
1. To start an Interactive Job:
```sh
qsub -I -q ampereq -l select=1:mem=128gb:ncpus=32:ngpus=1 -l place=excl
```
*WARNING:* that compilation for the AMD GPUS can only be done on the login nodes of P3 system.

2. Load Environment (LOGIN NODE):
```sh
module use -a /lustre/projects/bristol/modules/modulefiles
module load cmake
module use /software/x86/tools/nvidia/hpc_sdk/modulefiles
module load nvhpc/22.9 gcc/10.3.0
WORK_DIR=/lustre/home/ri-cstylianou
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/cuda-11.7-on-on
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/cuda-11.7-on-on
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/cuda-11.7-on-on
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/cuda-11.7-on-on
MORPHEUSORACLE_ROOT_DIR=$WORK_DIR/morpheus-oracle
MORPHEUSORACLE_BUILD_DIR=$WORK_DIR/morpheus-oracle/builds/cuda-11.7-on-on
MORPHEUSORACLE_INSTALL_DIR=$WORK_DIR/morpheus-oracle/installs/cuda-11.7-on-on
CXX_COMPILER=$KOKKOS_ROOT_DIR/bin/nvcc_wrapper
CUDA_ROOT=/software/x86/tools/nvidia/hpc_sdk/Linux_x86_64/22.9/cuda

export CRAYPE_LINK_TYPE=dynamic
```

## Kokkos
```sh
rm -rf $KOKKOS_BUILD_DIR && mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DKokkos_ENABLE_HIP=OFF \
                       -DKokkos_ENABLE_CUDA=ON \
                       -DKokkos_ENABLE_OPENMP=OFF \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_ZEN3=ON \
                       -DKokkos_ARCH_AMPERE80=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_CUDA_CONSTEXPR=ON \
                       -DKokkos_ENABLE_CUDA_LAMBDA=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_DEBUG_BOUNDS_CHECK=OFF \
                       -DCUDA_ROOT=$CUDA_ROOT && make -j && make install
```

## Morpheus
``` sh
rm -rf $MORPHEUS_BUILD_DIR && mkdir -p $MORPHEUS_BUILD_DIR && cd $MORPHEUS_BUILD_DIR
cmake $MORPHEUS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUS_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DKokkos_ROOT=${KOKKOS_INSTALL_DIR} \
                         -DMorpheus_ENABLE_EXAMPLES=OFF \
                         -DMorpheus_ENABLE_TESTS=OFF && make -j && make install
```

## Morpheus-Oracle
``` sh
rm -rf $MORPHEUSORACLE_BUILD_DIR && mkdir -p $MORPHEUSORACLE_BUILD_DIR && cd $MORPHEUSORACLE_BUILD_DIR
cmake $MORPHEUSORACLE_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUSORACLE_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DMorpheus_ROOT=${MORPHEUS_INSTALL_DIR} \
                         -DMorpheusOracle_ENABLE_EXAMPLES=OFF \
                         -DMorpheusOracle_ENABLE_TESTS=OFF && make -j && make install
```

# A64FX
## Serial
1. Start an Interactive Job:
```sh
qsub -I -q a64fx -l select=1:ncpus=48 -l place=excl
```
2. Load Environment:
```sh
module use -a /lustre/projects/bristol/modules-a64fx/modulefiles
module load cmake
module load gcc/10.3.0
WORK_DIR=/lustre/home/ri-cstylianou
CXX_COMPILER=$(which g++)
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/a64fx-gnu-10.3-on-off
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/a64fx-gnu-10.3-on-off
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/a64fx-gnu-10.3-on-off
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/a64fx-gnu-10.3-on-off
MORPHEUSORACLE_ROOT_DIR=$WORK_DIR/morpheus-oracle
MORPHEUSORACLE_BUILD_DIR=$WORK_DIR/morpheus-oracle/builds/a64fx-gnu-10.3-on-off
MORPHEUSORACLE_INSTALL_DIR=$WORK_DIR/morpheus-oracle/installs/a64fx-gnu-10.3-on-off

export CRAYPE_LINK_TYPE=dynamic
```

## Kokkos
```sh
rm -rf $KOKKOS_BUILD_DIR && mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DKokkos_ENABLE_HIP=OFF \
                       -DKokkos_ENABLE_CUDA=OFF \
                       -DKokkos_ENABLE_OPENMP=OFF \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_A64FX=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_DEBUG_BOUNDS_CHECK=OFF && make -j && make install
```

## Morpheus
``` sh
mkdir -p $MORPHEUS_BUILD_DIR && cd $MORPHEUS_BUILD_DIR
cmake $MORPHEUS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUS_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DKokkos_ROOT=${KOKKOS_INSTALL_DIR} \
                         -DMorpheus_ENABLE_EXAMPLES=OFF \
                         -DMorpheus_ENABLE_TESTS=OFF && make -j && make install
```

## Morpheus-Oracle
``` sh
rm -rf $MORPHEUSORACLE_BUILD_DIR && mkdir -p $MORPHEUSORACLE_BUILD_DIR && cd $MORPHEUSORACLE_BUILD_DIR
cmake $MORPHEUSORACLE_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUSORACLE_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DMorpheus_ROOT=${MORPHEUS_INSTALL_DIR} \
                         -DMorpheusOracle_ENABLE_EXAMPLES=OFF \
                         -DMorpheusOracle_ENABLE_TESTS=OFF && make -j && make install
```

## OpenMP
1. Start an Interactive Job:
```sh
qsub -I -q a64fx -l select=1:ncpus=48 -l place=excl
```
2. Load Environment:
```sh
module use -a /lustre/projects/bristol/modules-a64fx/modulefiles
module load cmake
module load gcc/10.3.0
WORK_DIR=/lustre/home/ri-cstylianou
CXX_COMPILER=$(which g++)
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/a64fx-gnu-10.3-on-on
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/a64fx-gnu-10.3-on-on
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/a64fx-gnu-10.3-on-on
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/a64fx-gnu-10.3-on-on
MORPHEUSORACLE_ROOT_DIR=$WORK_DIR/morpheus-oracle
MORPHEUSORACLE_BUILD_DIR=$WORK_DIR/morpheus-oracle/builds/a64fx-gnu-10.3-on-on
MORPHEUSORACLE_INSTALL_DIR=$WORK_DIR/morpheus-oracle/installs/a64fx-gnu-10.3-on-on

export CRAYPE_LINK_TYPE=dynamic
```

## Kokkos
```sh
rm -rf $KOKKOS_BUILD_DIR && mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DKokkos_ENABLE_HIP=OFF \
                       -DKokkos_ENABLE_CUDA=OFF \
                       -DKokkos_ENABLE_OPENMP=ON \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_A64FX=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_DEBUG_BOUNDS_CHECK=OFF && make -j && make install
```

## Morpheus
``` sh
mkdir -p $MORPHEUS_BUILD_DIR && cd $MORPHEUS_BUILD_DIR
cmake $MORPHEUS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUS_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DKokkos_ROOT=${KOKKOS_INSTALL_DIR} \
                         -DMorpheus_ENABLE_EXAMPLES=OFF \
                         -DMorpheus_ENABLE_TESTS=OFF && make -j && make install
```

## Morpheus-Oracle
``` sh
rm -rf $MORPHEUSORACLE_BUILD_DIR && mkdir -p $MORPHEUSORACLE_BUILD_DIR && cd $MORPHEUSORACLE_BUILD_DIR
cmake $MORPHEUSORACLE_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUSORACLE_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DMorpheus_ROOT=${MORPHEUS_INSTALL_DIR} \
                         -DMorpheusOracle_ENABLE_EXAMPLES=OFF \
                         -DMorpheusOracle_ENABLE_TESTS=OFF && make -j && make install
```


# XCI
## Serial
1. Start an Interactive Job:
```sh
qsub -I -q arm-dev -l select=1 -l place=excl
```
2. Load Environment:
```sh
module use -a /lustre/projects/bristol/modules-arm-phase2/modulefiles
module load cmake/3.23.2
module load craype-arm-thunderx2
module load gcc/10.3.0
WORK_DIR=/lustre/home/ri-cstylianou
CXX_COMPILER=$(which g++)
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/xci-gnu-10.3-on-off
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/xci-gnu-10.3-on-off
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/xci-gnu-10.3-on-off
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/xci-gnu-10.3-on-off
MORPHEUSORACLE_ROOT_DIR=$WORK_DIR/morpheus-oracle
MORPHEUSORACLE_BUILD_DIR=$WORK_DIR/morpheus-oracle/builds/xci-gnu-10.3-on-off
MORPHEUSORACLE_INSTALL_DIR=$WORK_DIR/morpheus-oracle/installs/xci-gnu-10.3-on-off

export CRAYPE_LINK_TYPE=dynamic
```

## Kokkos
```sh
rm -rf $KOKKOS_BUILD_DIR && mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DKokkos_ENABLE_HIP=OFF \
                       -DKokkos_ENABLE_CUDA=OFF \
                       -DKokkos_ENABLE_OPENMP=OFF \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_ARMV8_THUNDERX2=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_DEBUG_BOUNDS_CHECK=OFF && make -j && make install
```

## Morpheus
``` sh
mkdir -p $MORPHEUS_BUILD_DIR && cd $MORPHEUS_BUILD_DIR
cmake $MORPHEUS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUS_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DKokkos_ROOT=${KOKKOS_INSTALL_DIR} \
                         -DMorpheus_ENABLE_EXAMPLES=OFF \
                         -DMorpheus_ENABLE_TESTS=OFF && make -j && make install
```

## Morpheus-Oracle
``` sh
rm -rf $MORPHEUSORACLE_BUILD_DIR && mkdir -p $MORPHEUSORACLE_BUILD_DIR && cd $MORPHEUSORACLE_BUILD_DIR
cmake $MORPHEUSORACLE_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUSORACLE_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DMorpheus_ROOT=${MORPHEUS_INSTALL_DIR} \
                         -DMorpheusOracle_ENABLE_EXAMPLES=OFF \
                         -DMorpheusOracle_ENABLE_TESTS=OFF && make -j && make install
```

## OpenMP
1. Start an Interactive Job:
```sh
qsub -I -q arm-dev -l select=1 -l place=excl
```
2. Load Environment:
```sh
module use -a /lustre/projects/bristol/modules-arm-phase2/modulefiles
module load cmake/3.23.2
module load craype-arm-thunderx2
module load gcc/10.3.0
WORK_DIR=/lustre/home/ri-cstylianou
CXX_COMPILER=$(which g++)
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/xci-gnu-10.3-on-on
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/xci-gnu-10.3-on-on
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/xci-gnu-10.3-on-on
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/xci-gnu-10.3-on-on
MORPHEUSORACLE_ROOT_DIR=$WORK_DIR/morpheus-oracle
MORPHEUSORACLE_BUILD_DIR=$WORK_DIR/morpheus-oracle/builds/xci-gnu-10.3-on-on
MORPHEUSORACLE_INSTALL_DIR=$WORK_DIR/morpheus-oracle/installs/xci-gnu-10.3-on-on

export CRAYPE_LINK_TYPE=dynamic
```

## Kokkos
```sh
rm -rf $KOKKOS_BUILD_DIR && mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DKokkos_ENABLE_HIP=OFF \
                       -DKokkos_ENABLE_CUDA=OFF \
                       -DKokkos_ENABLE_OPENMP=ON \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_ARMV8_THUNDERX2=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_DEBUG_BOUNDS_CHECK=OFF && make -j && make install
```

## Morpheus
``` sh
mkdir -p $MORPHEUS_BUILD_DIR && cd $MORPHEUS_BUILD_DIR
cmake $MORPHEUS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUS_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DKokkos_ROOT=${KOKKOS_INSTALL_DIR} \
                         -DMorpheus_ENABLE_EXAMPLES=OFF \
                         -DMorpheus_ENABLE_TESTS=OFF && make -j && make install
```

## Morpheus-Oracle
``` sh
rm -rf $MORPHEUSORACLE_BUILD_DIR && mkdir -p $MORPHEUSORACLE_BUILD_DIR && cd $MORPHEUSORACLE_BUILD_DIR
cmake $MORPHEUSORACLE_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                         -DCMAKE_INSTALL_PREFIX=${MORPHEUSORACLE_INSTALL_DIR} \
                         -DCMAKE_BUILD_TYPE=Release \
                         -DMorpheus_ROOT=${MORPHEUS_INSTALL_DIR} \
                         -DMorpheusOracle_ENABLE_EXAMPLES=OFF \
                         -DMorpheusOracle_ENABLE_TESTS=OFF && make -j && make install
```
