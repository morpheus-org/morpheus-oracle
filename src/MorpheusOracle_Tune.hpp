/**
 * MorpheusOracle_Tune.hpp
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

#ifndef MORPHEUSORACLE_TUNE_HPP
#define MORPHEUSORACLE_TUNE_HPP

#include <impl/MorpheusOracle_Tune_Impl.hpp>

namespace Morpheus {
namespace Oracle {

/**
 * @brief Optimizes an operation by selecting the optimum format for the given
 * matrix. The tuning process depends on the tuner provided.
 *
 * @tparam Data The data type for which we are tuning for.
 * @tparam Functor The functor type of the operation to tune for.
 * @tparam Tuner The tuner type to be used for tuning.
 * @param data Data containing the input to be used the tuner
 * @param f The functor containing the operation to tune for.
 * @param tuner One of the supported tuners.
 */
template <typename Data, typename TuneFunctor, typename Tuner>
void tune(const Data& data, TuneFunctor& f, Tuner& tuner) {
  // TODO: Add static_assert to check if any of the available tuners
  // TODO: Add static_assert to check if any of the available functors base
  Impl::tune(data, f, tuner);
}

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_TUNE_HPP