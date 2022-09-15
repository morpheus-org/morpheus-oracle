/**
 * run_first_tuner.cpp
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

#include <Morpheus_Oracle.hpp>

int main() {
  Morpheus::initialize();
  {
    Morpheus::Oracle::RunFirstTuner tuner(10, true);
    Morpheus::DynamicMatrix<double, Kokkos::HostSpace> A;
    Morpheus::CsrMatrix<double, Kokkos::HostSpace> Acsr(4, 3, 6);

    // initialize matrix entries
    Acsr.row_offsets(0) = 0;
    Acsr.row_offsets(1) = 2;
    Acsr.row_offsets(2) = 2;
    Acsr.row_offsets(3) = 3;
    Acsr.row_offsets(4) = 6;

    Acsr.column_indices(0) = 0;
    Acsr.values(0)         = 10;
    Acsr.column_indices(1) = 2;
    Acsr.values(1)         = 20;
    Acsr.column_indices(2) = 2;
    Acsr.values(2)         = 30;
    Acsr.column_indices(3) = 0;
    Acsr.values(3)         = 40;
    Acsr.column_indices(4) = 1;
    Acsr.values(4)         = 50;
    Acsr.column_indices(5) = 2;
    Acsr.values(5)         = 60;

    A = Acsr;

    Morpheus::Oracle::tune_multiply<Kokkos::Serial>(A, tuner);
    tuner.print();
  }
  Morpheus::finalize();
  return 0;
}