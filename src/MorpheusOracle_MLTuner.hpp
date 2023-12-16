/**
 * MorpheusOracle_MLTuner.hpp
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

#ifndef MORPHEUSORACLE_ML_TUNER_HPP
#define MORPHEUSORACLE_ML_TUNER_HPP

namespace Morpheus {
namespace Oracle {

/**
 * \addtogroup tuners Tuners
 *
 */

template <typename T>
class MLTuner {
  T& actual() { return *static_cast<T*>(this); }
  T const& actual() const { return *static_cast<T const*>(this); }

 public:
  /*! Enum value specifying the state of the tuner when the optimum state
   * has not been yet selected*/
  const int INVALID_STATE = -1;

  void reload(const std::string& filename, const bool binary = true,
              const bool feature_names = true) {
    *actual().reload(filename, binary, feature_names);
  }

  template <typename ScalarVector>
  void run(const ScalarVector& sample) {
    *actual().run(sample);
  }

  void reset() { *actual().reset(); }

  void print() { *actual().print(); }

  auto& timings() { return *actual().timings(); }

  int state_id() const { return *actual().state_id(); }
  bool is_verbose() { return *actual().is_verbose(); }
  void set_verbose(bool verbose = true) { *actual().set_verbose(verbose); }
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_ML_TUNER_HPP
