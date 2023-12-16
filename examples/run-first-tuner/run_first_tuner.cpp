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
#include <Morpheus_Core.hpp>

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
using Vector        = Morpheus::DenseVector<double, backend>;

template <typename ExecSpace, typename Vector>
struct RunFirstFunctor : public Morpheus::Oracle::RunFirstFunctorBase<
                             RunFirstFunctor<ExecSpace, Vector>> {
  RunFirstFunctor(Vector& x, Vector& y, bool init = true)
      : _status(Morpheus::CONV_SUCCESS), _x(x), _y(y), _init(init) {}

  RunFirstFunctor(size_t nrows, size_t ncols, bool init = true)
      : _status(Morpheus::CONV_SUCCESS),
        _x(ncols, 2.0),
        _y(nrows, 0),
        _init(init) {}

  template <typename Data>
  auto clone(const Data& data,
             typename std::enable_if_t<
                 Morpheus::is_dynamic_matrix_container_v<Data>>* = nullptr) {
    using MemSpace = typename Data::memory_space;

    auto mirror = Morpheus::create_mirror<MemSpace>(data);
    Morpheus::copy(data, mirror);

    return mirror;
  }

  template <typename Dev>
  auto clone_host(
      Dev& dev,
      typename std::enable_if_t<Morpheus::is_dynamic_matrix_container_v<Dev>>* =
          nullptr) {
    auto mirror_h = Morpheus::create_mirror_container(dev);
    Morpheus::copy(dev, mirror_h);

    return mirror_h;
  }

  template <typename Tuner, typename Dev, typename Host>
  bool state_transition(const Tuner& tuner, Dev& dev, Host& host,
                        int& current_state) {
    if (((current_state == static_cast<int>(Morpheus::DIA_FORMAT)) ||
         (current_state == static_cast<int>(Morpheus::HDC_FORMAT))) &&
        (dev.nrows() != dev.ncols())) {
      _status = Morpheus::DYNAMIC_TO_PROXY;
    } else if (current_state != tuner.state_count()) {
      // Convert only when we start a new state_count
      _status = Morpheus::convert<Morpheus::Serial>(host, tuner.state_count());
      if (_status == Morpheus::CONV_SUCCESS) {
        dev.activate(host.active_index());
        dev.resize(host);
        Morpheus::copy(host, dev);
        current_state = tuner.state_count();
      }
    }

    return _status == Morpheus::CONV_SUCCESS ? true : false;
  }

  template <typename Dev>
  void run(const Dev& dev,
           typename std::enable_if_t<
               Morpheus::is_dynamic_matrix_container_v<Dev> &&
               Morpheus::has_access_v<ExecSpace, Dev, Vector>>* = nullptr) {
    Morpheus::multiply<ExecSpace>(dev, _x, _y, _init);
  }

  double postprocess_runtime(double runtime) { return runtime; }

  void set_multiply_init(bool init) { _init = init; }

 private:
  Morpheus::conversion_error_e _status;
  Vector _x, _y;
  bool _init;
};

int main(int argc, char* argv[]) {
  Morpheus::initialize(argc, argv);
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

    typename DynamicMatrix::HostMirror Ah;
    try {
      Morpheus::IO::read_matrix_market_file(Ah, filename);
    } catch (Morpheus::NotImplementedException& e) {
      std::cerr << "Exception Raised:: " << e.what() << std::endl;
      exit(0);
    }

    DynamicMatrix A = Morpheus::create_mirror<Space>(Ah);
    Morpheus::copy(Ah, A);

    Morpheus::Oracle::RunFirstTuner tuner(Morpheus::NFORMATS, reps, verbose);

    RunFirstFunctor<Space, Vector> f(A.nrows(), A.ncols());

    Morpheus::Oracle::tune(A, f, tuner);
    tuner.print();
  }
  Morpheus::finalize();
  return 0;
}