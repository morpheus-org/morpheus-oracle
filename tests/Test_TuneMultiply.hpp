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

#ifndef TEST_ORACLE_TEST_TUNEMULTIPLY_HPP
#define TEST_ORACLE_TEST_TUNEMULTIPLY_HPP

#include <Morpheus_Oracle.hpp>

TEST(TuneMultiply, RunFirstTuner) {
  using memory_space  = typename TEST_EXECSPACE::memory_space;
  using DynamicMatrix = Morpheus::DynamicMatrix<double, memory_space>;
  using CsrMatrix     = Morpheus::CsrMatrix<double, memory_space>;

  Morpheus::Oracle::RunFirstTuner tuner(10, false);
  DynamicMatrix A;
  typename CsrMatrix::HostMirror Acsr_h(4, 3, 6);

  // initialize matrix entries
  Acsr_h.row_offsets(0) = 0;
  Acsr_h.row_offsets(1) = 2;
  Acsr_h.row_offsets(2) = 2;
  Acsr_h.row_offsets(3) = 3;
  Acsr_h.row_offsets(4) = 6;

  Acsr_h.column_indices(0) = 0;
  Acsr_h.values(0)         = 10;
  Acsr_h.column_indices(1) = 2;
  Acsr_h.values(1)         = 20;
  Acsr_h.column_indices(2) = 2;
  Acsr_h.values(2)         = 30;
  Acsr_h.column_indices(3) = 0;
  Acsr_h.values(3)         = 40;
  Acsr_h.column_indices(4) = 1;
  Acsr_h.values(4)         = 50;
  Acsr_h.column_indices(5) = 2;
  Acsr_h.values(5)         = 60;

  auto Acsr = Morpheus::create_mirror_container<TEST_EXECSPACE>(Acsr_h);
  Morpheus::copy(Acsr_h, Acsr);

  A = Acsr;

  Morpheus::Oracle::tune_multiply<TEST_EXECSPACE>(A, tuner);

  EXPECT_TRUE(tuner.finished());
  // Check average timings were recorded
  for (size_t i = 0; i < tuner.avg_timings().size(); i++) {
    EXPECT_NE(tuner.avg_timings()(i), 0);
  }

  // Figure out which format was the best on average
  double mint        = tuner.avg_timings()(0);
  int best_format_id = 0;
  for (size_t i = 0; i < tuner.avg_timings().size(); i++) {
    if (tuner.avg_timings()(i) < mint) {
      mint           = tuner.avg_timings()(i);
      best_format_id = i;
    }
  }
  // Check if the tuner also got the same format index
  EXPECT_EQ(best_format_id, tuner.format_id());
}

#endif  // TEST_ORACLE_TEST_TUNEMULTIPLY_HPP