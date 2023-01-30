# SERIAL
1. Start an Interactive Job:
```sh
srun --exclusive --nodes=1 --time=02:00:00 \
     --partition=standard --qos=standard \
     --account=d403-cs --pty /usr/bin/bash --login
```
2. Load Environment:
```sh
module load cmake
module load gcc/10.2.0
WORK_DIR=/work/d403/d403/cstyl16
CXX_COMPILER=$(which g++)
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/gnu-10.2-off-on
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/gnu-10.2-off-on
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/gnu-10.2-off-on
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/gnu-10.2-off-on
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
                       -DKokkos_ARCH_BDW=ON \
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

# OPENMP
1. Start an Interactive Job:
```sh
srun --exclusive --nodes=1 --time=02:00:00 \
     --partition=standard --qos=standard \
     --account=d403-cs --pty /usr/bin/bash --login
```
2. Load Environment:
```sh
module load cmake
module load gcc/10.2.0
WORK_DIR=/work/d403/d403/cstyl16
CXX_COMPILER=$(which g++)
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/gnu-10.2-on-on
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/gnu-10.2-on-on
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/gnu-10.2-on-on
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/gnu-10.2-on-on
```

## Kokkos
```sh
mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DKokkos_ENABLE_HIP=OFF \
                       -DKokkos_ENABLE_CUDA=OFF \
                       -DKokkos_ENABLE_OPENMP=ON \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_BDW=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON && make -j && make install
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

# CUDA
1. Start an Interactive Job:
```sh
srun --exclusive --nodes=1 --time=1:0:0 \
     --partition=gpu --qos=gpu --gres=gpu:1  \
     --account=d403-cs --pty /bin/bash
```
2. Load Environment:
```sh
module load cmake
module load nvidia/nvhpc-nompi/22.2
WORK_DIR=/work/d403/d403/cstyl16
CXX_COMPILER=$WORK_DIR/kokkos/bin/nvcc_wrapper
KOKKOS_ROOT_DIR=$WORK_DIR/kokkos
KOKKOS_BUILD_DIR=$WORK_DIR/kokkos/builds/cuda-11.6-on-on
KOKKOS_INSTALL_DIR=$WORK_DIR/kokkos/installs/cuda-11.6-on-on
MORPHEUS_ROOT_DIR=$WORK_DIR/morpheus
MORPHEUS_BUILD_DIR=$WORK_DIR/morpheus/builds/cuda-11.6-on-on
MORPHEUS_INSTALL_DIR=$WORK_DIR/morpheus/installs/cuda-11.6-on-on
CUDA_ROOT=/scratch/sw/nvidia/hpcsdk-222/Linux_x86_64/22.2/cuda
```

## Kokkos
```sh
mkdir -p $KOKKOS_BUILD_DIR && cd $KOKKOS_BUILD_DIR
cmake $KOKKOS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                       -DCMAKE_INSTALL_PREFIX=${KOKKOS_INSTALL_DIR} \
                       -DCMAKE_BUILD_TYPE=Release \
                       -DCMAKE_CXX_EXTENSIONS=Off \
                       -DKokkos_ENABLE_HIP=OFF \
                       -DKokkos_ENABLE_CUDA=ON \
                       -DKokkos_ENABLE_OPENMP=OFF \
                       -DKokkos_ENABLE_SERIAL=ON \
                       -DKokkos_CXX_STANDARD=17 \
                       -DKokkos_ENABLE_COMPILER_WARNINGS=ON \
                       -DKokkos_ARCH_VOLTA70=ON \
                       -DKokkos_ARCH_SKX=ON \
                       -DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=ON \
                       -DKokkos_ENABLE_CUDA_CONSTEXPR=ON \
                       -DKokkos_ENABLE_CUDA_LAMBDA=ON \
                       -DCUDA_ROOT=$CUDA_ROOT && make -j && make install
```

## Morpheus
``` sh
mkdir -p $MORPHEUS_BUILD_DIR && cd $MORPHEUS_BUILD_DIR
cmake $MORPHEUS_ROOT_DIR -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
            -DCMAKE_INSTALL_PREFIX=${MORPHEUS_INSTALL_DIR} \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_CXX_EXTENSIONS=Off \
            -DKokkos_ROOT=${KOKKOS_INSTALL_DIR} \
            -DMorpheus_ENABLE_EXAMPLES=OFF \
            -DMorpheus_ENABLE_TESTS=OFF \
            -DCUDA_ROOT=$CUDA_ROOT && make -j && make install
```