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

#if defined(EXAMPLE_ENABLE_SERIAL)
using Space = Kokkos::Serial;
#elif defined(EXAMPLE_ENABLE_OPENMP)
using Space = Kokkos::OpenMP;
#elif defined(EXAMPLE_ENABLE_CUDA)
using Space = Kokkos::Cuda;
#elif defined(EXAMPLE_ENABLE_HIP)
using Space = Kokkos::HIP;
#endif

using memory_space  = typename Space::memory_space;
using DynamicMatrix = Morpheus::DynamicMatrix<double, memory_space>;
using CooMatrix     = Morpheus::CooMatrix<double, memory_space>;

int main(int argc, char* argv[]) {
  Morpheus::initialize();
  {
    if (argc != 4) {
      std::stringstream rt_error_msg;
      rt_error_msg << "Benchmark requires 3 runtime input arguments:\n";
      rt_error_msg << "\tfilename   : Matrix Market file to be used.\n";
      rt_error_msg << "\treps : How many times to tuner will run.\n";
      rt_error_msg << "\tVerbosity  : Whether to enable verbose messages.\n";
      rt_error_msg << " Received " << argc - 1 << " arguments !\n ";

      std::cout << rt_error_msg.str() << std::endl;
      exit(-1);
    }

    std::string filename = argv[1];
    size_t reps          = atoi(argv[2]);
    bool verbose         = atoi(argv[3]) ? true : false;

    std::cout << "\nRunning run_first_tuner example with:\n";
    std::cout << "\tFilename    : " << filename << "\n";
    std::cout << "\tVerbosity   : " << (verbose ? "ON" : "OFF") << "\n\n";

    typename CooMatrix::HostMirror Acoo_h;
    try {
      Morpheus::IO::read_matrix_market_file(Acoo_h, filename);
    } catch (Morpheus::NotImplementedException& e) {
      std::cerr << "Exception Raised:: " << e.what() << std::endl;
      exit(0);
    }

    typename DynamicMatrix::HostMirror A_h = Acoo_h;
    DynamicMatrix A = Morpheus::create_mirror<Space>(A_h);
    Morpheus::copy(A_h, A);

    Morpheus::Oracle::RunFirstTuner tuner(reps, verbose);

    Morpheus::Oracle::tune_multiply<Kokkos::Serial>(A, tuner);
    tuner.print();
  }
  Morpheus::finalize();
  return 0;
}