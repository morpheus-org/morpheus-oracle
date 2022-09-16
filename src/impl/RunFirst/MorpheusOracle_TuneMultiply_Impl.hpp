/**
 * MorpheusOracle_TuneMultiply_Impl.hpp
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

#ifndef MORPHEUSORACLE_IMPL_RUNFIRST_TUNEMULTIPLY_IMPL_HPP
#define MORPHEUSORACLE_IMPL_RUNFIRST_TUNEMULTIPLY_IMPL_HPP

#include <MorpheusOracle_TypeTraits.hpp>

#include <chrono>

namespace Morpheus {
namespace Oracle {
namespace Impl {

template <typename ExecSpace, typename Matrix, typename Tuner>
void tune_multiply(
    Matrix& mat, Tuner& tuner,
    typename std::enable_if_t<Morpheus::Oracle::is_run_first_tuner_v<Tuner>>* =
        nullptr) {
  using value_type   = typename Matrix::value_type;
  using index_type   = typename Matrix::index_type;
  using array_layout = typename Matrix::array_layout;
  using memory_space = typename Matrix::memory_space;
  using vector =
      Morpheus::DenseVector<value_type, index_type, array_layout, memory_space>;
  using ns = std::chrono::nanoseconds;

  vector x(mat.ncols(), value_type(2)), y(mat.nrows(), value_type(0));

  auto mat_h = Morpheus::create_mirror_container(mat);
  Morpheus::copy(mat, mat_h);

  size_t current_format = -1;
  while (!tuner.finished()) {
    if (current_format != tuner.format_count()) {
      // Convert only when we start a new format_count
      Morpheus::convert<Kokkos::Serial>(mat_h, tuner.format_count());
      mat.resize(mat_h);
      Morpheus::copy(mat_h, mat);
      current_format = tuner.format_count();
    }

    auto start = std::chrono::steady_clock::now();
    Morpheus::multiply<ExecSpace>(mat, x, y, true);
    auto end = std::chrono::steady_clock::now();

    double runtime = std::chrono::duration_cast<ns>(end - start).count() * 1e-9;

    tuner.register_run(runtime);
    ++tuner;
  }
}

}  // namespace Impl
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_IMPL_RUNFIRST_TUNEMULTIPLY_IMPL_HPP