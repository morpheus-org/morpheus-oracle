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

if [ "$#" -lt "5" ]; then
  echo -e "Script requires 5 runtime arguments to run."
  echo -e "\t\$1 : Backend [cuda]"
  echo -e "\t\$2 : Filename including the test set paths"
  echo -e "\t\$3 : Tuner Name [dt | rf]"
  echo -e "\t\$4 : Baseline Tuner"
  echo -e "\t\$5 : Tuned Tuner"
  exit 0
fi

backend=$1
fmat=$2
tuner=$3
base_tuner=$4
tuned_tuner=$5
queue="ampereq"

if [ "cuda" == "$backend" ]; then
  queue="ampereq"
else
 echo "Invalid backend ($backend)!"
 echo -e "\tAvailable backends: [cuda]"
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

RUN_PATH=$SCRIPT_PATH/run/$backend
EXE=$SCRIPT_PATH/build/$backend/tuners_comparison/src/$EXE_NAME

echo -e "Building Feature Extraction routine:"
echo -e "\tScript Path : $SCRIPT_PATH"
echo -e "\tRun Path    : $RUN_PATH"
echo -e "\tData Path   : $fmat"
echo -e "\tExecutable  : $EXE"
echo -e "\tBackend : $backend"
echo -e "\tTuner : $tuner"
echo -e "\tQueue   : $queue"

mkdir -p $RUN_PATH

TOTAL_MATRICES=$(( $(wc -l < $fmat) - 1 ))
INCREMENT=$(( $TOTAL_MATRICES / 15 ))
LOW_BOUND=0
UPPER_BOUND=$(( $LOW_BOUND + $INCREMENT ))

while [ $LOW_BOUND -lt $TOTAL_MATRICES ]; do
  qsub -q $queue -l select=1:mem=128gb:ncpus=32:ngpus=1 -l place=excl -l walltime=24:00:00 \
      -o $RUN_PATH/$queue-report-$tuner-$LOW_BOUND-$UPPER_BOUND.out -e $RUN_PATH/$queue-report-$tuner.err \
      -N "tuners_comparison-$tuner-$queue-run" \
      -v RUNPATH=$RUN_PATH,FMAT=$fmat,EXE=$EXE,BACKEND=$backend,BASE_TUNER=$base_tuner,TUNED_TUNER=$tuned_tuner,LB=$LOW_BOUND,UB=$UPPER_BOUND,TUNER=$tuner \
      $SCRIPT_PATH/run.comparison.pbs

  LOW_BOUND=$(( $LOW_BOUND + $INCREMENT ))
  UPPER_BOUND=$(( $UPPER_BOUND + $INCREMENT ))
  if [ $UPPER_BOUND -gt $TOTAL_MATRICES ]; then
    UPPER_BOUND=$TOTAL_MATRICES
  fi
done