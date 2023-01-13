/**
 * decision_tree_tuner.cpp
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

#if defined(EXAMPLE_ENABLE_SERIAL)
using Space = Morpheus::Serial;
#elif defined(EXAMPLE_ENABLE_OPENMP)
using Space = Morpheus::OpenMP;
#elif defined(EXAMPLE_ENABLE_CUDA)
using Space = Morpheus::Cuda;
#elif defined(EXAMPLE_ENABLE_HIP)
using Space = Morpheus::HIP;
#endif

using backend       = typename Space::backend;
using DynamicMatrix = Morpheus::DynamicMatrix<double, backend>;

int main(int argc, char* argv[]) {
  Morpheus::initialize();
  {
    if (argc != 3) {
      std::stringstream rt_error_msg;
      rt_error_msg << "Benchmark requires 2 runtime input arguments:\n";
      rt_error_msg << "\tfmatrix  : Matrix Market file to be used.\n";
      rt_error_msg << "\tftree    : Decision Tree file to be used.\n";
      rt_error_msg << " Received " << argc - 1 << " argument(s) !\n ";

      std::cout << rt_error_msg.str() << std::endl;
      exit(-1);
    }

    std::string fmatrix = argv[1], ftree = argv[2];

    std::cout << "\nRunning decision_tree_tuner example with:\n";
    std::cout << "\tMatrix Filename  : " << fmatrix << "\n";
    std::cout << "\tTree Filename    : " << ftree << "\n\n";

    typename DynamicMatrix::HostMirror Ah;
    try {
      Morpheus::IO::read_matrix_market_file(Ah, fmatrix);
    } catch (Morpheus::NotImplementedException& e) {
      std::cerr << "Exception Raised:: " << e.what() << std::endl;
      exit(0);
    }

    DynamicMatrix A = Morpheus::create_mirror<Space>(Ah);
    Morpheus::copy(Ah, A);

    Morpheus::Oracle::DecisionTreeTuner tuner(ftree, true);
    Morpheus::Oracle::tune_multiply<Morpheus::Serial>(A, tuner);
    tuner.print();
  }
  Morpheus::finalize();
  return 0;
}