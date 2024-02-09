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

#ifndef MORPHEUSORACLE_IMPL_RUNFIRST_TUNE_IMPL_HPP
#define MORPHEUSORACLE_IMPL_RUNFIRST_TUNE_IMPL_HPP

#include <MorpheusOracle_TypeTraits.hpp>

#include <chrono>
#include <limits>

namespace Morpheus {
namespace Oracle {
namespace Impl {

// template <typename Matrix, typename TuneFunctor, typename Tuner>
// void tune(
//     const Matrix& mat, TuneFunctor& f, Tuner& tuner,
//     typename std::enable_if_t<Morpheus::Oracle::is_run_first_tuner_v<Tuner>>*
//     =
//         nullptr) {
//   using ns       = std::chrono::nanoseconds;
//   using MemSpace = typename Matrix::memory_space;

//   auto mat_mirror = Morpheus::create_mirror<MemSpace>(mat);
//   Morpheus::copy(mat, mat_mirror);

//   auto mat_mirror_h = Morpheus::create_mirror_container(mat_mirror);
//   Morpheus::copy(mat_mirror, mat_mirror_h);

//   int current_format = Morpheus::Oracle::RunFirstTuner::INVALID_FORMAT_STATE;
//   Morpheus::conversion_error_e status = Morpheus::CONV_SUCCESS;
//   while (!tuner.finished()) {
//     if (((current_format == static_cast<int>(DIA_FORMAT)) ||
//          (current_format == static_cast<int>(HDC_FORMAT))) &&
//         (mat.nrows() != mat.ncols())) {
//       status = Morpheus::DYNAMIC_TO_PROXY;
//     } else if (current_format != tuner.format_count()) {
//       // Convert only when we start a new format_count
//       status = Morpheus::convert<Morpheus::Serial>(mat_mirror_h,
//                                                    tuner.format_count());
//       if (status == Morpheus::CONV_SUCCESS) {
//         mat_mirror.activate(mat_mirror_h.active_index());
//         mat_mirror.resize(mat_mirror_h);
//         Morpheus::copy(mat_mirror_h, mat_mirror);
//         current_format = tuner.format_count();
//       }
//     }
//     double runtime;
//     if (status == Morpheus::CONV_SUCCESS) {
//       auto start = std::chrono::steady_clock::now();
//       f(mat_mirror, true);
//       auto end = std::chrono::steady_clock::now();

//       runtime = std::chrono::duration_cast<ns>(end - start).count() * 1e-9;
//     } else {
//       runtime = std::numeric_limits<double>::max();
//     }
//     tuner.register_run(runtime);
//     ++tuner;
//   }
// }

template <typename Data, typename TuneFunctor, typename Tuner>
void tune(
    const Data& data, TuneFunctor& f, Tuner& tuner,
    typename std::enable_if_t<
        Morpheus::Oracle::is_run_first_tuner_v<Tuner> &&
        Morpheus::Oracle::is_runfirst_functor_v<TuneFunctor>>* = nullptr) {
  using ns = std::chrono::nanoseconds;

  auto mirror      = f.clone(data);
  auto host_mirror = f.clone_host(mirror);

  while (!tuner.finished()) {
    double runtime;
    if (f.state_transition(tuner, mirror, host_mirror)) {
      auto start = std::chrono::steady_clock::now();
      for (int i = 0; i < tuner.repetitions(); i++) {
        f.run(mirror);
      }
      auto end = std::chrono::steady_clock::now();

      runtime = std::chrono::duration_cast<ns>(end - start).count() * 1e-9;
    } else {
      runtime = std::numeric_limits<double>::max();
    }

    double final_runtime = f.postprocess_runtime(runtime);

    tuner.register_run(final_runtime);
    ++tuner;
  }
}

// template <typename Matrix, typename TuneFunctor, typename Tuner>
// void tune(
//     const Matrix& mat, TuneFunctor& f, Tuner& tuner,
//     typename std::enable_if_t<Morpheus::Oracle::is_run_first_tuner_v<Tuner>>*
//     =
//         nullptr) {
//   using ns       = std::chrono::nanoseconds;
//   using MemSpace = typename Matrix::memory_space;

//   Matrix mat_mirror;
//   auto mat_coo = Morpheus::create_mirror(mat);
//   Morpheus::copy(mat, mat_coo);

//   Morpheus::conversion_error_e status = Morpheus::CONV_SUCCESS;
//   status = Morpheus::convert<Morpheus::Serial>(mat_coo,
//   Morpheus::COO_FORMAT); if (status != Morpheus::CONV_SUCCESS) {
//     return;
//   }

//   int current_format = Morpheus::Oracle::RunFirstTuner::INVALID_FORMAT_STATE;
//   while (!tuner.finished()) {
//     if (current_format != tuner.format_count()) {
//       auto mat_h = Morpheus::create_mirror(mat_coo);
//       Morpheus::copy(mat_coo, mat_h);
//       // Convert only when we start a new format_count
//       status = Morpheus::convert<Morpheus::Serial>(mat_h,
//       tuner.format_count()); if (status == Morpheus::CONV_SUCCESS) {
//         mat_mirror.activate(mat_h.active_index());
//         mat_mirror.resize(mat_h);
//         Morpheus::copy(mat_h, mat_mirror);
//         current_format = tuner.format_count();
//       }
//     }
//     double runtime;
//     if (status == Morpheus::CONV_SUCCESS) {
//       auto start = std::chrono::steady_clock::now();
//       f(mat_mirror, true);
//       auto end = std::chrono::steady_clock::now();

//       runtime = std::chrono::duration_cast<ns>(end - start).count() * 1e-9;
//     } else {
//       runtime = std::numeric_limits<double>::max();
//     }
//     tuner.register_run(runtime);
//     ++tuner;
//   }
// }

}  // namespace Impl
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_IMPL_RUNFIRST_TUNE_IMPL_HPP