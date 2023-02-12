# Build

```
WORK_PATH=/work/e609/e609/cstyl
BACKEND=serial
BUDGET=e609
MORPHEUSORACLE_PATH=$WORK_PATH/morpheus-oracle/installs/gnu-10.2-off-on
KOKKOS_PATH=$WORK_PATH/kokkos/installs/gnu-10.2-off-on
./build.sh $BACKEND $MORPHEUSORACLE_PATH $KOKKOS_PATH $BUDGET
```

```
WORK_PATH=/work/e609/e609/cstyl
BACKEND=openmp
BUDGET=e609
MORPHEUSORACLE_PATH=$WORK_PATH/morpheus-oracle/installs/gnu-10.2-on-on
KOKKOS_PATH=$WORK_PATH/kokkos/installs/gnu-10.2-on-on
./build.sh $BACKEND $MORPHEUSORACLE_PATH $KOKKOS_PATH $BUDGET
```

# Run

```
BACKEND=serial
BUDGET=e609
EXP=archer2-square_set-serial-1000
DATA_PATH=/work/e609/e609/cstyl/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-test-$EXP.txt
TUNER=dt
BASELINE=$DATA_PATH/trees/extract_clf/$EXP/baseline_tree.txt
TUNED=$DATA_PATH/trees/extract_clf/$EXP/tuned_tree.txt

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED $BUDGET
```

```
BACKEND=openmp
BUDGET=e609
EXP=archer2-square_set-openmp-1000
DATA_PATH=/work/e609/e609/cstyl/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-test-$EXP.txt
TUNER=dt
BASELINE=$DATA_PATH/trees/extract_clf/$EXP/baseline_tree.txt
TUNED=$DATA_PATH/trees/extract_clf/$EXP/tuned_tree.txt

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED $BUDGET
```

