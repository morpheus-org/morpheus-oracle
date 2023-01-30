/**
 * decision-tree.cpp
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

#include <chrono>
#include <limits>
#include <sys/stat.h>

#if defined(EXAMPLE_ENABLE_CUDA)
using Space = Morpheus::Cuda;
#elif defined(EXAMPLE_ENABLE_HIP)
using Space = Morpheus::HIP;
#elif defined(EXAMPLE_ENABLE_OPENMP)
using Space = Morpheus::OpenMP;
#else
using Space = Morpheus::Serial;
#endif

using backend       = typename Space::backend;
using DynamicMatrix = Morpheus::DynamicMatrix<double, backend>;
using Timings_t = Morpheus::DenseVector<double, Morpheus::HostSpace>;
using ns        = std::chrono::nanoseconds;

int main(int argc, char* argv[]) {
  Morpheus::initialize(argc, argv);
  {
    if (argc != 5) {
      std::stringstream rt_error_msg;
      rt_error_msg << "Benchmark requires 4 runtime input arguments:\n";
      rt_error_msg << "\tfmatrix     : Matrix Market file to be used.\n";
      rt_error_msg << "\tftree_base  : Baseline Decision Tree file to be used.\n";
      rt_error_msg << "\tftree_tuned : Tuned Decision Tree file to be used.\n";
      rt_error_msg << "\toutdir      : Output Directory to write the timings in.\n";
      rt_error_msg << " Received " << argc - 1 << " argument(s) !\n ";

      std::cout << rt_error_msg.str() << std::endl;
      exit(-1);
    }

    std::string fmatrix = argv[1], ftree_base = argv[2], ftree_tuned = argv[3], outdir = argv[4];

    std::cout << "\nRunning decision_tree_tuner example with:\n";
    std::cout << "\tMatrix Filename  : " << fmatrix << "\n";
    std::cout << "\tBaseline Tree Filename    : " << ftree_base << "\n";
    std::cout << "\tTuned Tree Filename    : " << ftree_tuned << "\n";
    std::cout << "\tOutput Directory    : " << outdir << "\n" << std::endl;

    typename DynamicMatrix::HostMirror Ah;
    try {
      Morpheus::IO::read_matrix_market_file(Ah, fmatrix);
    } catch (Morpheus::NotImplementedException& e) {
      std::cerr << "Exception Raised:: " << e.what() << std::endl;
      exit(0);
    }

    DynamicMatrix A = Morpheus::create_mirror<Space>(Ah);
    Morpheus::copy(Ah, A);

    std::stringstream ss;
    ss << "Version,Initialization,FeatureExtraction,Inference" << std::endl;

    double tinit_base=0, tinit_tuned=0;
    // Baseline
    { 
      auto start = std::chrono::steady_clock::now();
      Morpheus::Oracle::DecisionTreeTuner tuner(ftree_base, true);
      auto end = std::chrono::steady_clock::now();
      tinit_base = std::chrono::duration_cast<ns>(end - start).count() * 1e-9;

      Morpheus::Oracle::tune_multiply<backend>(A, tuner);

      ss << "baseline," << tinit_base << "," << tuner.timings()[0] << "," << tuner.timings()[1] << std::endl;
      tuner.print();
    }

    // Tuned
    {
      auto start = std::chrono::steady_clock::now();
      Morpheus::Oracle::DecisionTreeTuner tuner(ftree_tuned, true);
      auto end = std::chrono::steady_clock::now();
      tinit_tuned = std::chrono::duration_cast<ns>(end - start).count() * 1e-9;

      Morpheus::Oracle::tune_multiply<backend>(A, tuner);

      ss << "tuned," << tinit_tuned << "," << tuner.timings()[0] << "," << tuner.timings()[1] << std::endl;
      tuner.print();
    }

    struct stat buffer;
    if (stat(outdir.c_str(), &buffer) != 0) {
      if (mkdir(outdir.c_str(), 0777) != 0) {
        std::cout << "Output Directory (" << outdir << ") was NOT created!"
                  << std::endl;
        exit(1);
      }
    }

    std::ofstream outFile;
    outFile.open(outdir + "/timings.csv");
    outFile << ss.str();
    outFile.close();
  }
  Morpheus::finalize();
  return 0;
}