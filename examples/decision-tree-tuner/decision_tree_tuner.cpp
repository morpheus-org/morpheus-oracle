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

template <typename ExecSpace>
struct ExtractFeaturesFunctor : public Morpheus::Oracle::MLFunctorBase<
                                    ExtractFeaturesFunctor<ExecSpace>> {
  using features_vector_t = Morpheus::DenseVector<double, Morpheus::HostSpace>;

  ExtractFeaturesFunctor(size_t nfeatures)
      : _nfeatures(nfeatures), _features(nfeatures, 0) {}

  template <typename Data>
  void extract_features(
      const Data& data,
      typename std::enable_if_t<Morpheus::is_dynamic_matrix_container_v<Data> &&
                                Morpheus::has_access_v<ExecSpace, Data>>* =
          nullptr) {
    using size_type    = typename Data::size_type;
    using index_type   = typename Data::index_type;
    using memory_space = typename Data::memory_space;
    using IndexVector =
        Morpheus::DenseVector<index_type, size_type, memory_space>;

    if (_features.size() != _nfeatures) {
      _features.resize(_nfeatures, 0);
    }

    IndexVector nnz_per_row(data.nrows(), 0);
    IndexVector nnz_per_diag(data.nrows() + data.ncols() - 1, 0);
    Morpheus::count_nnz_per_row<ExecSpace>(data, nnz_per_row, false);
    Morpheus::count_nnz_per_diagonal<ExecSpace>(data, nnz_per_diag, false);

    _features[0] = Morpheus::number_of_rows(data);
    _features[1] = Morpheus::number_of_columns(data);
    _features[2] = Morpheus::number_of_nnz(data);
    _features[3] = Morpheus::average_nnnz(data);
    _features[4] = Morpheus::density(data);
    _features[5] = Morpheus::max<ExecSpace>(nnz_per_row, nnz_per_row.size());
    _features[6] = Morpheus::min<ExecSpace>(nnz_per_row, nnz_per_row.size());
    _features[7] = Morpheus::std<ExecSpace>(nnz_per_row, nnz_per_row.size(),
                                            Morpheus::average_nnnz(data));
    _features[8] = Morpheus::count_nnz<ExecSpace>(nnz_per_diag);
    _features[9] =
        Morpheus::count_nnz<ExecSpace>(nnz_per_diag, data.nrows() / 5);
  }

  features_vector_t& features() { return _features; }

  template <typename Tuner>
  void inference(Tuner& tuner) {
    tuner.run(features());
  }

 private:
  size_t _nfeatures;
  Morpheus::DenseVector<double, Morpheus::HostSpace> _features;
};

int main(int argc, char* argv[]) {
  Morpheus::initialize(argc, argv);
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

    Morpheus::Oracle::DecisionTreeTuner tuner(ftree, true, true, true);
    ExtractFeaturesFunctor<Space> f(10);

    Morpheus::Oracle::tune(A, f, tuner);
    tuner.print();
  }
  Morpheus::finalize();
  return 0;
}