/**
 * Test_RunFirstTuner.hpp
 *
 * EPCC, The University of Edinburgh
 *
 * (c) 2022 The University of Edinburgh
 *
 * Contributing Authors:
 * Christodoulos Stylianou (c.stylianou@ed.ac.uk)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TEST_ORACLE_TEST_RUNFIRSTTUNER_HPP
#define TEST_ORACLE_TEST_RUNFIRSTTUNER_HPP

#include <Morpheus_Oracle.hpp>

TEST(RunFirstTunerTest, DefaultConstruction) {
  const int reps     = 10;
  const int nfmts    = Morpheus::NFORMATS;
  const bool verbose = false;

  Morpheus::Oracle::RunFirstTuner tuner;

  // Check timings shape
  EXPECT_EQ(tuner.timings().nrows(), nfmts);
  EXPECT_EQ(tuner.timings().ncols(), reps);
  // Check timings values
  for (auto i = 0; i < tuner.timings().nrows(); i++) {
    for (auto j = 0; j < tuner.timings().ncols(); j++) {
      EXPECT_EQ(tuner.timings()(i, j), 0);
    }
  }

  // Check maximum timings shape
  EXPECT_EQ(tuner.max_timings().size(), nfmts);
  // Check minimum timings shape
  EXPECT_EQ(tuner.min_timings().size(), nfmts);
  // Check average timings shape
  EXPECT_EQ(tuner.avg_timings().size(), nfmts);
  // Check values of each timing vector
  for (auto i = 0; i < nfmts; i++) {
    EXPECT_EQ(tuner.max_timings()(i), 0);
    EXPECT_EQ(tuner.min_timings()(i), 0);
    EXPECT_EQ(tuner.avg_timings()(i), 0);
  }

  EXPECT_EQ(tuner.format_id(),
            Morpheus::Oracle::RunFirstTuner::INVALID_FORMAT_STATE);

  EXPECT_EQ(tuner.format_count(), 0);
  EXPECT_EQ(tuner.nformats(), nfmts);

  EXPECT_EQ(tuner.repetition_count(), 0);
  EXPECT_EQ(tuner.repetition_limit(), reps);

  EXPECT_EQ(tuner.is_verbose(), verbose);
}

TEST(RunFirstTunerTest, Construction) {
  const int reps     = 5;
  const int nfmts    = Morpheus::NFORMATS;
  const bool verbose = true;

  Morpheus::Oracle::RunFirstTuner tuner(reps, verbose);

  // Check timings shape
  EXPECT_EQ(tuner.timings().nrows(), nfmts);
  EXPECT_EQ(tuner.timings().ncols(), reps);
  // Check timings values
  for (auto i = 0; i < tuner.timings().nrows(); i++) {
    for (auto j = 0; j < tuner.timings().ncols(); j++) {
      EXPECT_EQ(tuner.timings()(i, j), 0);
    }
  }

  // Check maximum timings shape
  EXPECT_EQ(tuner.max_timings().size(), nfmts);
  // Check minimum timings shape
  EXPECT_EQ(tuner.min_timings().size(), nfmts);
  // Check average timings shape
  EXPECT_EQ(tuner.avg_timings().size(), nfmts);
  // Check values of each timing vector
  for (auto i = 0; i < nfmts; i++) {
    EXPECT_EQ(tuner.max_timings()(i), 0);
    EXPECT_EQ(tuner.min_timings()(i), 0);
    EXPECT_EQ(tuner.avg_timings()(i), 0);
  }

  EXPECT_EQ(tuner.format_id(),
            Morpheus::Oracle::RunFirstTuner::INVALID_FORMAT_STATE);

  EXPECT_EQ(tuner.format_count(), 0);
  EXPECT_EQ(tuner.nformats(), nfmts);

  EXPECT_EQ(tuner.repetition_count(), 0);
  EXPECT_EQ(tuner.repetition_limit(), reps);

  EXPECT_EQ(tuner.is_verbose(), verbose);
}

TEST(RunFirstTunerTest, Step) {
  // Check the tuner advances first by each format and then by each repetition
  EXPECT_EQ(1, 1);
}

TEST(RunFirstTunerTest, NotFinish) {
  // Check the tuner does not finish prior to reaching the rep_limit and
  // nformats.
  EXPECT_EQ(1, 1);
}

TEST(RunFirstTunerTest, Finish) {
  // Check upon completion the tuner computes the appropriate statistics and
  // sets a storage format.
  EXPECT_EQ(1, 1);
}

TEST(RunFirstTunerTest, RegisterRun) {
  // Check if the tuner registers appropriately each format run
  EXPECT_EQ(1, 1);
}

TEST(RunFirstTunerTest, Reset) {
  // Check if the tuner resets to the correct state
  EXPECT_EQ(1, 1);
}

#endif  // TEST_ORACLE_TEST_RUNFIRSTTUNER_HPP