/**
 * MorpheusOracle_Tune_Impl.hpp
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

#ifndef MORPHEUSORACLE_IMPL_RANDOMFOREST_TUNE_IMPL_HPP
#define MORPHEUSORACLE_IMPL_RANDOMFOREST_TUNE_IMPL_HPP

#include <MorpheusOracle_TypeTraits.hpp>

#include <chrono>

namespace Morpheus {
namespace Oracle {
namespace Impl {

template <typename Matrix, typename FeaturesFunctor, typename Tuner>
void tune(const Matrix& mat, FeaturesFunctor& f, Tuner& tuner,
          typename std::enable_if_t<
              Morpheus::Oracle::is_random_forest_tuner_v<Tuner>>* = nullptr) {
  using vector = typename Tuner::scalar_vector;
  using ns     = std::chrono::nanoseconds;

  auto start = std::chrono::steady_clock::now();
  vector features;
  f(mat, features);
  auto end = std::chrono::steady_clock::now();
  tuner.timings()[0] =
      std::chrono::duration_cast<ns>(end - start).count() * 1e-9;

  start = std::chrono::steady_clock::now();
  tuner.tune(features);
  end = std::chrono::steady_clock::now();
  tuner.timings()[1] =
      std::chrono::duration_cast<ns>(end - start).count() * 1e-9;
}

}  // namespace Impl
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_IMPL_RANDOMFOREST_TUNE_IMPL_HPP