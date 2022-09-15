/**
 * MorpheusOracle_TypeTraits.hpp
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

#ifndef MORPHEUSORACLE_TYPETRAITS_HPP
#define MORPHEUSORACLE_TYPETRAITS_HPP

#include <MorpheusOracle_fwd.hpp>

#include <type_traits>

namespace Morpheus {
namespace Oracle {

/**
 * @brief A valid run first tuner is the one that is either of type
 * \p RunFirstTuner or a derived class of it.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_run_first_tuner {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*,
      typename std::enable_if<std::is_same<RunFirstTuner, U>::value ||
                              std::is_base_of<RunFirstTuner, U>::value>::type* =
          nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_run_first_tuner.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_run_first_tuner_v = is_run_first_tuner<T>::value;

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_TYPETRAITS_HPP