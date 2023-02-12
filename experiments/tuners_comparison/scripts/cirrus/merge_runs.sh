#!/bin/sh
# merge_runs.sh
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

if [ "$#" -lt "3" ]; then
  echo -e "Script requires 3 runtime arguments to run."
  echo -e "\t\$1 : Backend [hip]"
  echo -e "\t\$2 : Tuner [dt | rf]"
  echo -e "\t\$3 : Results Path"
  exit 0
fi

backend=$1
tuner=$2
RUN_PATH=$3

if [ "hip" != "$backend" ]; then
  echo "Invalid backend ($backend)!"
  echo -e "\tAvailable backends: [hip]"
  exit 1
fi

if [ ! -d $RUN_PATH ]; then
  echo "Results path ($RUN_PATH) does not exist!"
  exit 1
fi

PROCESSED_PATH=$SCRIPT_PATH/processed
OUTFILE="$PROCESSED_PATH/runtimes-$tuner-$backend.csv"
mkdir -p $PROCESSED_PATH


header=""
for MATRIX_DIR in $RUN_PATH/$tuner/*; do 
  if [ ! -d $MATRIX_DIR ]; then
    continue
  fi

  MATRIX=$(basename $MATRIX_DIR)
  FILE="$MATRIX_DIR/timings.csv"
  if [ -z "$header" ]; then
    header="matrix,$(head -n 1 $FILE)"
    echo "$header" > $OUTFILE
  fi

  for i in $(seq 2 3); do
    entry="$MATRIX,$(sed -n "$i p" $FILE)"
    echo "$entry" >> $OUTFILE
  done
done

