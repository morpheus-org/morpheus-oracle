/**
 * MorpheusOracle_RunFirstFunctor.hpp
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

#ifndef MORPHEUSORACLE_RUNFIRST_FUNCTOR_HPP
#define MORPHEUSORACLE_RUNFIRST_FUNCTOR_HPP

#include <type_traits>

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

template <typename T>
class RunFirstFunctorBase {
  T& actual() { return *static_cast<T*>(this); }
  T const& actual() const { return *static_cast<T const*>(this); }

 public:
  template <typename Data>
  auto clone(const Data& data) {
    return *actual().clone(data);
  }

  template <typename Dev>
  auto clone_host(Dev& dev) {
    return *actual().clone_host(dev);
  }

  template <typename Tuner, typename Dev, typename Host>
  bool state_transition(const Tuner& tuner, Dev& dev, Host& host,
                        int& current_state) {
    return *actual().state_transition(tuner, dev, host, current_state);
  }

  template <typename Dev>
  void run(const Dev& dev) {
    *actual().run(dev);
  }

  double postprocess_runtime(double runtime) {
    return *actual().postprocess_runtime(runtime);
  }
};

/*! \}  // end of functors group */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RUNFIRST_FUNCTOR_HPP
