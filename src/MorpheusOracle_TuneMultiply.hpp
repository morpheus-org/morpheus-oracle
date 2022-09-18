/**
 * MorpheusOracle_TuneMultiply.hpp
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

#ifndef MORPHEUSORACLE_TUNEMULTIPLY_HPP
#define MORPHEUSORACLE_TUNEMULTIPLY_HPP

#include <Morpheus_Core.hpp>
#include <impl/MorpheusOracle_TuneMultiply_Impl.hpp>

namespace Morpheus {
namespace Oracle {

template <typename ExecSpace, typename DynamicMatrix, typename Tuner>
void tune_multiply(DynamicMatrix& mat, Tuner& tuner) {
  static_assert(Morpheus::is_dynamic_matrix_container<DynamicMatrix>::value,
                "Input Matrix for the tuner must be a valid DynamicMatrix.");
  static_assert(Morpheus::is_execution_space<ExecSpace>::value,
                "Input Execution Space must be a valid Execution Space.");

  Impl::tune_multiply<ExecSpace>(mat, tuner);
}

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_TUNEMULTIPLY_HPP