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
#include <gtest/gtest.h>

TEST(RunFirstTunerTest, Construction) {
  const int reps     = 5;
  const int nstates  = 10;
  const bool verbose = true;
  int invalid_state  = Morpheus::Oracle::RunFirstTuner::INVALID_STATE;

  Morpheus::Oracle::RunFirstTuner tuner(nstates, reps, verbose);

  // Check timings shape
  EXPECT_EQ(tuner.timings().size(), nstates * reps);
  // Check timings values
  for (size_t i = 0; i < nstates; i++) {
    for (size_t j = 0; j < reps; j++) {
      EXPECT_EQ(tuner.timings()[i * reps + j], 0);
    }
  }

  // Check maximum timings shape
  EXPECT_EQ(tuner.max_timings().size(), nstates);
  // Check minimum timings shape
  EXPECT_EQ(tuner.min_timings().size(), nstates);
  // Check average timings shape
  EXPECT_EQ(tuner.avg_timings().size(), nstates);
  // Check values of each timing vector
  for (auto i = 0; i < nstates; i++) {
    EXPECT_EQ(tuner.max_timings()[i], 0);
    EXPECT_EQ(tuner.min_timings()[i], 0);
    EXPECT_EQ(tuner.avg_timings()[i], 0);
  }

  EXPECT_EQ(tuner.state_id(), invalid_state);

  EXPECT_EQ(tuner.state_count(), 0);
  EXPECT_EQ(tuner.nstates(), nstates);

  EXPECT_EQ(tuner.repetition_count(), 0);
  EXPECT_EQ(tuner.repetition_limit(), reps);

  EXPECT_EQ(tuner.is_verbose(), verbose);
}

TEST(RunFirstTunerTest, Step) {
  const int reps    = 5;
  const int nstates = 3;
  Morpheus::Oracle::RunFirstTuner tuner(nstates, reps);

  for (int state = 0; state < tuner.nstates(); state++) {
    for (size_t rep = 0; rep < tuner.repetition_limit(); rep++) {
      EXPECT_EQ(tuner.state_count(), state);
      EXPECT_EQ(tuner.repetition_count(), rep);
      ++tuner;
    }
  }
  EXPECT_EQ(tuner.state_count(), tuner.nstates());
  EXPECT_EQ(tuner.repetition_count(), 0);
}

TEST(RunFirstTunerTest, Finish) {
  const int reps    = 5;
  const int nstates = 3;
  int invalid_state = Morpheus::Oracle::RunFirstTuner::INVALID_STATE;

  Morpheus::Oracle::RunFirstTuner tuner(nstates, reps);

  for (int state = 0; state < tuner.nstates(); state++) {
    for (size_t rep = 0; rep < tuner.repetition_limit(); rep++) {
      EXPECT_FALSE(tuner.finished());
      ++tuner;
    }
  }
  EXPECT_TRUE(tuner.finished());
  EXPECT_NE(tuner.state_id(), invalid_state);
}

TEST(RunFirstTunerTest, RegisterRun) {
  const int reps    = 5;
  const int nstates = 3;
  Morpheus::Oracle::RunFirstTuner tuner(nstates, reps);

  for (int state = 0; state < tuner.nstates(); state++) {
    for (size_t rep = 0; rep < tuner.repetition_limit(); rep++) {
      double rt = tuner.repetition_count() + tuner.state_count();
      tuner.register_run(rt);
      ++tuner;
    }
  }

  for (int state = 0; state < tuner.nstates(); state++) {
    for (size_t rep = 0; rep < tuner.repetition_limit(); rep++) {
      EXPECT_EQ(tuner.timings()[state * tuner.repetition_limit() + rep],
                rep + state);
    }
  }
}

TEST(RunFirstTunerTest, FinishStats) {
  const int reps    = 5;
  const int nstates = 3;

  Morpheus::Oracle::RunFirstTuner tuner(nstates, reps);

  while (!tuner.finished()) {
    double rt = tuner.repetition_count() + tuner.state_count();
    tuner.register_run(rt);
    ++tuner;
  }

  // Get repetition sum to be used in figuring out the average timings
  double repsum = 0;
  for (size_t i = 0; i < tuner.repetition_limit(); i++) {
    repsum += i;
  }

  for (int state = 0; state < tuner.nstates(); state++) {
    EXPECT_EQ(tuner.min_timings()[state], state);
    EXPECT_EQ(tuner.max_timings()[state],
              (tuner.repetition_limit() - 1) + state);
    EXPECT_EQ(tuner.avg_timings()[state],
              (repsum + (tuner.repetition_limit() * state)) /
                  tuner.repetition_limit());
  }
}

TEST(RunFirstTunerTest, Reset) {
  const int reps     = 5;
  const int nstates  = 10;
  const bool verbose = false;
  int invalid_state  = Morpheus::Oracle::RunFirstTuner::INVALID_STATE;

  Morpheus::Oracle::RunFirstTuner tuner(nstates, reps, verbose);

  while (!tuner.finished()) {
    double rt = tuner.repetition_count() + tuner.state_count();
    tuner.register_run(rt);
    ++tuner;
  }

  tuner.reset();

  // Check timings shape
  EXPECT_EQ(tuner.timings().size(), nstates * reps);
  // Check timings values
  for (size_t i = 0; i < nstates; i++) {
    for (size_t j = 0; j < reps; j++) {
      EXPECT_EQ(tuner.timings()[i * reps + j], 0);
    }
  }

  // Check maximum timings shape
  EXPECT_EQ(tuner.max_timings().size(), nstates);
  // Check minimum timings shape
  EXPECT_EQ(tuner.min_timings().size(), nstates);
  // Check average timings shape
  EXPECT_EQ(tuner.avg_timings().size(), nstates);
  // Check values of each timing vector
  for (auto i = 0; i < nstates; i++) {
    EXPECT_EQ(tuner.max_timings()[i], -std::numeric_limits<double>::max());
    EXPECT_EQ(tuner.min_timings()[i], -std::numeric_limits<double>::max());
    EXPECT_EQ(tuner.avg_timings()[i], -std::numeric_limits<double>::max());
  }

  EXPECT_EQ(tuner.state_id(), invalid_state);

  EXPECT_EQ(tuner.state_count(), 0);
  EXPECT_EQ(tuner.nstates(), nstates);

  EXPECT_EQ(tuner.repetition_count(), 0);
  EXPECT_EQ(tuner.repetition_limit(), reps);

  EXPECT_EQ(tuner.is_verbose(), verbose);
}

#endif  // TEST_ORACLE_TEST_RUNFIRSTTUNER_HPP