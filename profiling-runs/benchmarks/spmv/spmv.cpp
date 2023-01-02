/**
 * spmv.cpp
 *
 * EPCC, The University of Edinburgh
 *
 * (c) 2023 The University of Edinburgh
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

#include "Morpheus_Oracle.hpp"

#define MORPHEUS_START_SCOPE() {  // Open Morpheus Scope
#define MORPHEUS_END_SCOPE() }    // Close Morpheus Scope

using local_int_t = int;
using scalar_t    = double;

#if defined(Oracle_ENABLE_GENERIC)
namespace morpheus_space_t = Morpheus::Generic;
#else
namespace morpheus_space_t = Morpheus::Custom;
#endif

// Define Morpheus Execution, Memory Spaces and Backend
#if defined(Oracle_ENABLE_OPENMP)
using exec_space_t = typename morpheus_space_t::OpenMP::execution_space;
using mem_space_t  = typename morpheus_space_t::OpenMP::memory_space;
using backend_t    = typename morpheus_space_t::OpenMP::backend;
#elif defined(Oracle_ENABLE_CUDA)
using exec_space_t         = typename morpheus_space_t::Cuda::execution_space;
using mem_space_t          = typename morpheus_space_t::Cuda::memory_space;
using backend_t            = typename morpheus_space_t::Cuda::backend;
#elif defined(Oracle_ENABLE_HIP)
using exec_space_t = typename morpheus_space_t::HIP::execution_space;
using mem_space_t  = typename morpheus_space_t::HIP::memory_space;
using backend_t    = typename morpheus_space_t::HIP::backend;
#else
using exec_space_t = typename morpheus_space_t::Serial::execution_space;
using mem_space_t  = typename morpheus_space_t::Serial::memory_space;
using backend_t    = typename morpheus_space_t::Serial::backend;
#endif

using Matrix = Morpheus::DynamicMatrix<scalar_t, local_int_t, backend_t>;

int main(int argc, char* argv[]) {
  Morpheus::initialize(argc, argv, false);
  MORPHEUS_START_SCOPE();

  const int rt_args = 4;

  if (argc != rt_args) {
    std::stringstream rt_error_msg;
    rt_error_msg << "Benchmark requires " << rt_args - 1
                 << " runtime input arguments:\n";
    rt_error_msg << "\tfilename   : Matrix Market file to be used.\n";
    rt_error_msg << "\treps : How many times to tuner will run.\n";
    rt_error_msg << "\tVerbosity  : Whether to enable verbose messages.\n";
    rt_error_msg << "Received " << argc - 1 << " arguments !\n ";

    std::cout << rt_error_msg.str() << std::endl;
    exit(-1);
  }

  std::string filename = argv[1];
  size_t reps          = atoi(argv[2]);
  bool verbose         = atoi(argv[3]) ? true : false;

  std::cout << "\nRunning Morpheus-Oracle Benchmark with:\n";
  std::cout << "\tFilename    : " << filename << "\n";
  std::cout << "\tVerbosity   : " << (verbose ? "ON" : "OFF") << "\n\n";

  Matrix A;
  typename Matrix::HostMirror Ah;
  try {
    // Read matrix from a file using matrix market file-format
    Morpheus::IO::read_matrix_market_file(Ah, filename);
  } catch (Morpheus::NotImplementedException& e) {
    std::cerr << "Exception Raised:: " << e.what() << std::endl;
    exit(0);
  }

  // Setup local matrix on Device
  A.activate(Ah.active_index());
  A.resize(Ah);
  Morpheus::copy(Ah, A);

  // Tune Matrices
  Morpheus::Oracle::RunFirstTuner tuner(reps, verbose);

  Morpheus::Oracle::tune_multiply<backend_t>(A, tuner);
  tuner.print();

  MORPHEUS_END_SCOPE();
  Morpheus::finalize();

  return 0;
}