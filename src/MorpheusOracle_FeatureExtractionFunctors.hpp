/**
 * MorpheusOracle_FeatureExtractionFunctors.hpp
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

#ifndef MORPHEUSORACLE_FEATUREEXTRUCTIONFUNCTORS_HPP
#define MORPHEUSORACLE_FEATUREEXTRUCTIONFUNCTORS_HPP

#include <Morpheus_Core.hpp>

//! Generic Morpheus interfaces
namespace Morpheus {
//! Morpheus interfaces specific to Oracle package
namespace Oracle {

/**
 * \addtogroup functors Functors
 * \par Overview
 * TODO
 * \brief A set of functors to be used by various algorithms.
 * \{
 *
 */

/**
 * \addtogroup ml_functors ML Functors
 * \brief A set of functors to be used by ML autotuners such as
 * \p DecisionTreeTuner and \p RandomForestTuner. \ingroup functors
 * \{
 *
 */
template <typename ExecSpace>
struct ExtractFeaturesFunctor {
  template <typename Matrix, typename FeatureVector>
  void operator()(
      const Matrix& mat, FeatureVector& features,
      typename std::enable_if_t<
          Morpheus::is_dynamic_matrix_container_v<Matrix> &&
          Morpheus::is_dense_vector_format_container_v<FeatureVector> &&
          Morpheus::has_host_memory_space_v<FeatureVector> &&
          Morpheus::has_access_v<ExecSpace, Matrix>>* = nullptr) {
    static_assert(
        std::is_floating_point_v<typename FeatureVector::value_type>,
        "The value_type of the FeatureVector must be a floating-point "
        "type.");
    using size_type    = typename Matrix::size_type;
    using index_type   = typename Matrix::index_type;
    using memory_space = typename Matrix::memory_space;
    using IndexVector =
        Morpheus::DenseVector<index_type, size_type, memory_space>;

    const size_t features_size = 10;
    if (features.size() != features_size) {
      features.resize(features_size, 0);
    }

    IndexVector nnz_per_row(mat.nrows(), 0);
    IndexVector nnz_per_diag(mat.nrows() + mat.ncols() - 1, 0);
    Morpheus::count_nnz_per_row<ExecSpace>(mat, nnz_per_row, false);
    Morpheus::count_nnz_per_diagonal<ExecSpace>(mat, nnz_per_diag, false);

    features[0] = Morpheus::number_of_rows(mat);
    features[1] = Morpheus::number_of_columns(mat);
    features[2] = Morpheus::number_of_nnz(mat);
    features[3] = Morpheus::average_nnnz(mat);
    features[4] = Morpheus::density(mat);
    features[5] = Morpheus::max<ExecSpace>(nnz_per_row, nnz_per_row.size());
    features[6] = Morpheus::min<ExecSpace>(nnz_per_row, nnz_per_row.size());
    features[7] = Morpheus::std<ExecSpace>(nnz_per_row, nnz_per_row.size(),
                                           Morpheus::average_nnnz(mat));
    features[8] = Morpheus::count_nnz<ExecSpace>(nnz_per_diag);
    features[9] = Morpheus::count_nnz<ExecSpace>(nnz_per_diag, mat.nrows() / 5);
  }
};

/*! \}  // end of ml_functors group */
/*! \}  // end of functors group */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_FEATUREEXTRUCTIONFUNCTORS_HPP
