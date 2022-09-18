/**
 * Test_TypeTraits.hpp
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

#ifndef TEST_ORACLE_TEST_TYPETRAITS_HPP
#define TEST_ORACLE_TEST_TYPETRAITS_HPP

#include <Morpheus_Oracle.hpp>

TEST(TypeTraits, IsRunFirstTuner) {
  int res = Morpheus::Oracle::is_run_first_tuner<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_run_first_tuner<A>::value;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_run_first_tuner<
      Morpheus::Oracle::RunFirstTuner>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_run_first_tuner_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_run_first_tuner_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_run_first_tuner_v<Morpheus::Oracle::RunFirstTuner>;
  EXPECT_EQ(res, 1);
}

#endif  // TEST_ORACLE_TEST_TYPETRAITS_HPP