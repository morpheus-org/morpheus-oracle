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

TEST(RunFirstTunerTest, Construction) {
  // Check the tuner constructs in the correct way
  EXPECT_EQ(1, 1);
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