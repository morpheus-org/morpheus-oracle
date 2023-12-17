/**
 * Test_TuneMultiply.hpp
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

#ifndef TEST_ORACLE_TEST_TUNE_RUNFIRST_HPP
#define TEST_ORACLE_TEST_TUNE_RUNFIRST_HPP

#include <Morpheus_Oracle.hpp>
#include <gtest/gtest.h>

#include <variant>

TEST(TuneMultiply, RunFirstTuner) {
  using Data = std::variant<double, float, int>;

  struct RunFirstFunctor
      : public Morpheus::Oracle::RunFirstFunctorBase<RunFirstFunctor> {
    RunFirstFunctor() {}

    auto clone(const Data& data) {
      auto mirror = data;

      return mirror;
    }

    auto clone_host(Data& dev) {
      auto mirror_h = dev;

      return mirror_h;
    }

    bool state_transition(const Morpheus::Oracle::RunFirstTuner& tuner,
                          Data& dev, Data& host, int& current_state) {
      switch (current_state) {
        case 0: host = 0.55f; break;
        case 1: host = 2; break;
        case 2: host = 6.76; break;
        default: host = 6.76; break;
      }
      dev           = host;
      current_state = tuner.state_count();

      return true;
    }

    void run(const Data& dev) {
      Data res;
      std::visit([&](auto&& arg1, auto&& arg2) { arg1 = arg2 * arg2; }, res,
                 dev);
    }

    double postprocess_runtime(double runtime) { return runtime; }
  };

  Morpheus::Oracle::RunFirstTuner tuner(3, 10, false);
  RunFirstFunctor f;

  Data input = 1.111;

  Morpheus::Oracle::tune(input, f, tuner);

  EXPECT_TRUE(tuner.finished());
  // Check average timings were recorded
  for (size_t i = 0; i < tuner.avg_timings().size(); i++) {
    EXPECT_GE(tuner.avg_timings()[i], 0.0);
  }

  // Figure out which format was the best on average
  double mint       = tuner.avg_timings()[0];
  int best_state_id = 0;
  for (size_t i = 0; i < tuner.avg_timings().size(); i++) {
    if (tuner.avg_timings()[i] < mint) {
      mint          = tuner.avg_timings()[i];
      best_state_id = i;
    }
  }
  // Check if the tuner also got the same format index
  EXPECT_EQ(best_state_id, tuner.state_id());
}

#endif  // TEST_ORACLE_TEST_TUNE_RUNFIRST_HPP