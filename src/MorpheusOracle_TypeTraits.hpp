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

#include <fwd/MorpheusOracle_Fwd_DecisionTree.hpp>
#include <fwd/MorpheusOracle_Fwd_RandomForest.hpp>
#include <fwd/MorpheusOracle_Fwd_RunFirstTuner.hpp>
#include <fwd/MorpheusOracle_Fwd_MLTuner.hpp>
#include <fwd/MorpheusOracle_Fwd_DecisionTreeTuner.hpp>
#include <fwd/MorpheusOracle_Fwd_RandomForestTuner.hpp>
#include <fwd/MorpheusOracle_Fwd_RunFirstFunctor.hpp>
#include <fwd/MorpheusOracle_Fwd_MLFunctor.hpp>

#include <type_traits>

namespace Morpheus {
namespace Oracle {

/**
 * \defgroup typetraits Type Traits
 * \brief Various tools for examining the different types available and
 * relationships between them during compile-time.
 * \{
 *
 */

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

/**
 * @brief A valid Decision Tree container is the one that is either of type
 * \p DecisionTree or a derived class of it.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_decision_tree {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*,
      typename std::enable_if<std::is_same<DecisionTree, U>::value ||
                              std::is_base_of<DecisionTree, U>::value>::type* =
          nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_decision_tree.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_decision_tree_v = is_decision_tree<T>::value;

/**
 * @brief A valid Random Forest container is the one that is either of type
 * \p RandomForest or a derived class of it.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_random_forest {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*,
      typename std::enable_if<std::is_same<RandomForest, U>::value ||
                              std::is_base_of<RandomForest, U>::value>::type* =
          nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_random_forest.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_random_forest_v = is_random_forest<T>::value;

/**
 * @brief A valid Decision Tree Tuner is the one that is either of type
 * \p DecisionTreeTuner or a derived class of it.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_decision_tree_tuner {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*, typename std::enable_if<
              std::is_same<DecisionTreeTuner, U>::value ||
              std::is_base_of<DecisionTreeTuner, U>::value>::type* = nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_decision_tree_tuner.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_decision_tree_tuner_v =
    is_decision_tree_tuner<T>::value;

/**
 * @brief A valid Random Forest Tuner is the one that is either of type
 * \p RandomForestTuner or a derived class of it.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_random_forest_tuner {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*, typename std::enable_if<
              std::is_same<RandomForestTuner, U>::value ||
              std::is_base_of<RandomForestTuner, U>::value>::type* = nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_random_forest_tuner.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_random_forest_tuner_v =
    is_random_forest_tuner<T>::value;

/**
 * @brief A valid Machine Learning Tuner is the one that is a derived class of
 * \p MLTuner<T>.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_ml_tuner {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*,
      typename std::enable_if<std::is_base_of<MLTuner<U>, U>::value>::type* =
          nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_ml_tuner.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_ml_tuner_v = is_ml_tuner<T>::value;

/**
 * @brief A valid Machine Learning (ML) Functor is the one that is derived from
 * the \p MLFunctorBase functor.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_ml_functor {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*, typename std::enable_if<
              std::is_base_of<MLFunctorBase<U>, U>::value>::type* = nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_ml_tuner.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_ml_functor_v = is_ml_functor<T>::value;

/**
 * @brief A valid RunFirst Functor is the one that is derived from
 * the \p RunFirstFunctorBase functor.
 *
 * @tparam T Type passed for check.
 */
template <class T>
class is_runfirst_functor {
  typedef char yes[1];
  typedef char no[2];

  template <class U>
  static yes& test(
      U*,
      typename std::enable_if<
          std::is_base_of<RunFirstFunctorBase<U>, U>::value>::type* = nullptr);

  template <class U>
  static yes& test(
      U*,
      typename std::enable_if<std::is_base_of<
          RunFirstFunctorBase<typename U::base>, U>::value>::type* = nullptr);

  template <class U>
  static no& test(...);

 public:
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

/**
 * @brief Short-hand to \p is_runfirst_functor.
 *
 * @tparam T Type passed for check.
 */
template <typename T>
inline constexpr bool is_runfirst_functor_v = is_runfirst_functor<T>::value;

/*! \} end of typetraits group
 */

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_TYPETRAITS_HPP