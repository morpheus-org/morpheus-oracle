#!/bin/bash --login

RUN_PATH=$1
FMAT=$2
EXE=$3
BACKEND=$4
BASE_TUNER=$5
TUNED_TUNER=$6
LOW=$7
UP=$8
TUNER=$9

module load gcc/10.2.0

if [ "serial" == "$BACKEND" ]; then
  export OMP_NUM_THREADS=16 
  ncpus="--hint=nomultithread"
  NTHREADS=1
  NGPUS=0
elif [ "openmp" == "$BACKEND" ]; then
  export OMP_NUM_THREADS=16 
  export OMP_PROC_BIND=close
  export OMP_PLACES=cores
  ncpus="--hint=nomultithread"
  ngpus=""
  NTHREADS=16
  NGPUS=0
fi

EXE_NAME=$(basename $EXE)
LOG_FILE="$RUN_PATH/$EXE_NAME-$LOW-$UP.log"
ctr=0
while IFS= read -r line
do 
  if [ $ctr -ge $LOW ] && [ $ctr -lt $UP ]; then
    MATRIX_NAME=$(basename $line)
    FMATRIX=$line/$MATRIX_NAME.mtx
    OUTDIR="$RUN_PATH/matrices/$TUNER/$MATRIX_NAME"

    mkdir -p $OUTDIR

    echo -e "\t$MATRIX"
    $EXE $FMATRIX $BASE_TUNER $TUNED_TUNER $OUTDIR --kokkos-threads=$NTHREADS --kokkos-num-devices=$NGPUS  2>&1 | tee -a $LOG_FILE
  fi
  ctr=$(( $ctr + 1 ))
done < "$FMAT"

