/**
 * MorpheusOracle_RunFirstFunctors.hpp
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

#ifndef MORPHEUSORACLE_RUNFIRSTFUNCTORS_HPP
#define MORPHEUSORACLE_RUNFIRSTFUNCTORS_HPP

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
 * \addtogroup runfirst_functors RunFirst Functors
 * \brief A set of functors to be used by \p RunFirst autotuner.
 * \ingroup functors
 * \{
 *
 */

template <typename ExecSpace, typename Vector>
struct RunFirstMultiplyFunctor {
  RunFirstMultiplyFunctor(Vector& x, Vector& y) : _x(x), _y(y) {}

  RunFirstMultiplyFunctor(size_t nrows, size_t ncols)
      : _x(ncols, 2.0), _y(nrows, 0) {}

  template <typename Matrix>
  void operator()(
      const Matrix& A, bool init,
      typename std::enable_if_t<
          Morpheus::is_dynamic_matrix_container_v<Matrix> &&
          Morpheus::has_access_v<ExecSpace, Matrix, Vector>>* = nullptr) {
    Morpheus::multiply<ExecSpace>(A, _x, _y, init);
  }

 private:
  Vector _x, _y;
};

/*! \}  // end of runfirst_functors group */
/*! \}  // end of functors group */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RUNFIRSTFUNCTORS_HPP
