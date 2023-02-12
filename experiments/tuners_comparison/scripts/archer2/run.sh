#!/bin/sh
# run.sh
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

SCRIPT_PATH="$(
  cd -- "$(dirname "$0")" >/dev/null 2>&1
  pwd -P
)"

if [ "$#" -lt "6" ]; then
  echo -e "Script requires 5 runtime arguments to run."
  echo -e "\t\$1 : Backend [serial | openmp]"
  echo -e "\t\$2 : Filename including the test set paths"
  echo -e "\t\$3 : Tuner Name [dt | rf]"
  echo -e "\t\$4 : Baseline Tuner"
  echo -e "\t\$5 : Tuned Tuner"
  echo -e "\t\$6 : Budget  - Account budget to be charged for the run."
  exit 0
fi

backend=$1
fmat=$2
tuner=$3
base_tuner=$4
tuned_tuner=$5
budget=$6
queue="standard"
qos="lowpriority"

if [ "serial" == "$backend" ]; then
  queue="standard"
  # qos="lowpriority"
  qos="standard"
  ncpus="--tasks-per-node=1 --cpus-per-task=128"
  ngpus=""
elif [ "openmp" == "$backend" ]; then
  queue="standard"
  # qos="lowpriority"
  qos="standard"
  ncpus="--tasks-per-node=1 --cpus-per-task=128"
  ngpus=""
else
  echo "Invalid backend ($backend)!"
  echo -e "\tAvailable backends: [serial | openmp ]"
  exit 1
fi

if [ "dt" == "$tuner" ]; then
EXE_NAME="MorpheusOracleExperiments_decision-tree"
elif [ "rf" == "$tuner" ]; then
EXE_NAME="MorpheusOracleExperiments_random-forest"
else
 echo "Invalid tuner name ($tuner)!"
 echo -e "\tAvailable tuners: [dt | rf]"
 exit 1
fi

if [ -z "$6" ]; then
  ACCOUNT=e609
else
  ACCOUNT=$budget
fi

RUN_PATH=$SCRIPT_PATH/run/$backend
EXE=$SCRIPT_PATH/build/$backend/tuners_comparison/src/$EXE_NAME

echo -e "Building Feature Extraction routine:"
echo -e "\tScript Path : $SCRIPT_PATH"
echo -e "\tRun Path    : $RUN_PATH"
echo -e "\tData Path   : $fmat"
echo -e "\tExecutable  : $EXE"
echo -e "\tBackend : $backend"
echo -e "\tBudget  : $budget"
echo -e "\tTuner : $tuner"
echo -e "\tQueue   : $queue"

mkdir -p $RUN_PATH

TOTAL_MATRICES=$(( $(wc -l < $fmat) - 1 ))
INCREMENT=$(( $TOTAL_MATRICES / 31 ))
LOW_BOUND=0
UPPER_BOUND=$(( $LOW_BOUND + $INCREMENT ))

while [ $LOW_BOUND -lt $TOTAL_MATRICES ]; do
  sbatch --exclusive --nodes=1 --time=24:00:00 --partition=$queue \
    --qos=$qos $ncpus $ngpus --account=$ACCOUNT --job-name=tuners_comparison-$tuner-$backend-run \
    --output=$RUN_PATH/$queue-$tuner-$LOW_BOUND-$UPPER_BOUND-report.out \
    --error=$RUN_PATH/$queue-$tuner-$LOW_BOUND-$UPPER_BOUND-report.err \
    $SCRIPT_PATH/run.comparison.slurm $RUN_PATH $fmat $EXE $backend $base_tuner $tuned_tuner $LOW_BOUND $UPPER_BOUND


  LOW_BOUND=$(( $LOW_BOUND + $INCREMENT ))
  UPPER_BOUND=$(( $UPPER_BOUND + $INCREMENT ))
  if [ $UPPER_BOUND -gt $TOTAL_MATRICES ]; then
    UPPER_BOUND=$TOTAL_MATRICES
  fi
done