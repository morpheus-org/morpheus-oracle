# Build

```
WORK_PATH=/lustre/home/ri-cstylianou
BACKEND=serial
MORPHEUSORACLE_PATH=$WORK_PATH/morpheus-oracle/installs/xci-gnu-10.3-on-off
KOKKOS_PATH=$WORK_PATH/kokkos/installs/xci-gnu-10.3-on-off
./build.sh $BACKEND $MORPHEUSORACLE_PATH $KOKKOS_PATH
```

```
WORK_PATH=/lustre/home/ri-cstylianou
BACKEND=openmp
MORPHEUSORACLE_PATH=$WORK_PATH/morpheus-oracle/installs/xci-gnu-10.3-on-on
KOKKOS_PATH=$WORK_PATH/kokkos/installs/xci-gnu-10.3-on-on
./build.sh $BACKEND $MORPHEUSORACLE_PATH $KOKKOS_PATH
```

# Run - DT

```
BACKEND=serial
EXP=xci-square_set-serial-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-matrices-$EXP.txt
TUNER=dt
BASELINE=$DATA_PATH/trees/extract_clf/$EXP/10/baseline_tree.txt
TUNED=$DATA_PATH/trees/extract_clf/$EXP/10/tuned_tree.txt

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```

```
BACKEND=openmp
EXP=xci-square_set-openmp-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-matrices-$EXP.txt
TUNER=dt
BASELINE=$DATA_PATH/trees/extract_clf/$EXP/10/baseline_tree.txt
TUNED=$DATA_PATH/trees/extract_clf/$EXP/10/tuned_tree.txt

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```

# Run - RF

```
BACKEND=serial
EXP=xci-square_set-serial-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-matrices-$EXP.txt
TUNER=rf
BASELINE=$DATA_PATH/forests/extract_clf/$EXP/10/baseline
TUNED=$DATA_PATH/trees/extract_clf/$EXP/10/tuned

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```

```
BACKEND=openmp
EXP=xci-square_set-openmp-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-matrices-$EXP.txt
TUNER=rf
BASELINE=$DATA_PATH/forests/extract_clf/$EXP/10/baseline
TUNED=$DATA_PATH/trees/extract_clf/$EXP/10/tuned

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```

