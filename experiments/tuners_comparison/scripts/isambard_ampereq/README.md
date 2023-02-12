# Build
```
WORK_PATH=/lustre/home/ri-cstylianou
BACKEND=cuda
MORPHEUSORACLE_PATH=$WORK_PATH/morpheus-oracle/installs/cuda-11.7-on-on
KOKKOS_PATH=$WORK_PATH/kokkos/installs/cuda-11.7-on-on
./build.sh $BACKEND $MORPHEUSORACLE_PATH $KOKKOS_PATH
```

# Run

```
BACKEND=cuda
EXP=ampere-square_set-cuda-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-matrices-$EXP.txt
TUNER=dt
BASELINE=$DATA_PATH/trees/extract_clf/$EXP/10/baseline_tree.txt
TUNED=$DATA_PATH/trees/extract_clf/$EXP/10/tuned_tree.txt

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```

```
BACKEND=cuda
EXP=ampere-square_set-cuda-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-matrices-$EXP.txt
TUNER=rf
BASELINE=$DATA_PATH/forests/extract_clf/$EXP/10/baseline
TUNED=$DATA_PATH/forests/extract_clf/$EXP/10/tuned

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```