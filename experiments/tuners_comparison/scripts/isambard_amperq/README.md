# Build
```
WORK_PATH=/lustre/home/ri-cstylianou
BACKEND=openmp
MORPHEUSORACLE_PATH=$WORK_PATH/morpheus-oracle/installs/cuda-11.7-on-on
KOKKOS_PATH=$WORK_PATH/kokkos/installs/cuda-11.7-on-on
./build.sh $BACKEND $MORPHEUSORACLE_PATH $KOKKOS_PATH
```

# Run

```
BACKEND=cuda
EXP=ampere-square_set-cuda-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-test-$EXP.txt
TUNER=dt
BASELINE=$DATA_PATH/trees/extract_clf/$EXP/baseline_tree.txt
TUNED=$DATA_PATH/trees/extract_clf/$EXP/tuned_tree.txt

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```

/lustre/home/ri-cstylianou/morpheus-oracle/experiments/tuners_comparison/scripts/isambard_amperq/build/cuda/tuners_comparison/src/MorpheusOracleExperiments_decision-tree /lustre/home/ri-cstylianou/sparse.tree/data/square_set/matrices/1000_1050/Hardesty1/Hardesty1.mtx /lustre/home/ri-cstylianou/morpheus-oracle/experiments/data/trees/extract_clf/ampere-square_set-cuda-1000/baseline_tree.txt /lustre/home/ri-cstylianou/morpheus-oracle/experiments/data/trees/extract_clf/ampere-square_set-cuda-1000/tuned_tree.txt /lustre/home/ri-cstylianou/morpheus-oracle/experiments/tuners_comparison/scripts/isambard_amperq/run/cuda/matrices/Hardesty1