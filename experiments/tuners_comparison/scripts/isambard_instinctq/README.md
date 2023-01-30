# Run

```
BACKEND=hip
EXP=instinct-square_set-hip-1000
DATA_PATH=/lustre/home/ri-cstylianou/morpheus-oracle/experiments/data
FMAT=$DATA_PATH/test_lists/list-test-$EXP.txt
TUNER=dt
BASELINE=$DATA_PATH/trees/extract_clf/$EXP/baseline_tree.txt
TUNED=$DATA_PATH/trees/extract_clf/$EXP/tuned_tree.txt

./run.sh $BACKEND $FMAT $TUNER $BASELINE $TUNED
```