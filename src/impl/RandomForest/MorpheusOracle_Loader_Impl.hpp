/**
 * MorpheusOracle_Loader_Impl.hpp
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

#ifndef MORPHEUSORACLE_IMPL_RANDOMFOREST_LOADER_IMPL_HPP
#define MORPHEUSORACLE_IMPL_RANDOMFOREST_LOADER_IMPL_HPP

#include <MorpheusOracle_TypeTraits.hpp>
#include <impl/DecisionTree/MorpheusOracle_Loader_Impl.hpp>
#include <impl/MorpheusOracle_Loader_Utils.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Morpheus {
namespace Oracle {
namespace Impl {

// template <typename Tree>
// void load_binary(
//     std::ifstream& sforest, Tree& forest, const bool feature_names = true,
//     typename std::enable_if_t<is_random_forest_v<Tree>>* = nullptr) {}
template <typename Tree>
void load_binary(
    std::ifstream&, Tree&, bool,
    typename std::enable_if_t<is_random_forest_v<Tree>>* = nullptr) {}

template <typename Tree>
void load_text(std::ifstream& sforest, Tree& forest,
               const bool feature_names                             = true,
               typename std::enable_if_t<is_random_forest_v<Tree>>* = nullptr) {
  // Load Metadata
  Impl::skip_comment(sforest);
  // Read four entries (NFeatures, Nclasses, NOutputs, NEstimators)
  std::vector<std::string> tokens;
  std::string line;
  std::getline(sforest, line);
  Morpheus::IO::Impl::tokenize(tokens, line);

  size_t nfeatures, nclasses, noutputs, nestimators;
  std::istringstream(tokens[0]) >> nfeatures;
  std::istringstream(tokens[1]) >> nclasses;
  std::istringstream(tokens[2]) >> noutputs;
  std::istringstream(tokens[3]) >> nestimators;

  forest.set_nfeatures(nfeatures);
  forest.set_nclasses(nclasses);
  forest.set_noutputs(noutputs);
  forest.set_nestimators(nestimators);

  forest.estimator_sizes().resize(nestimators, 0);  // TODO
  Impl::load_array(forest.estimator_sizes(), sforest, true);

  forest.classes().resize(nclasses, 0);
  Impl::load_array(forest.classes(), sforest, true);

  if (feature_names) {
    forest.feature_names_sizes().resize(nfeatures);
    Impl::load_array(forest.feature_names_sizes(), sforest, true);
    forest.feature_names().resize(nfeatures);
    Impl::load_array(forest.feature_names(), sforest, true);
  }

  // Load each tree
  forest.estimators().resize(nestimators);
  for (size_t i = 0; i < nestimators; i++) {
    Impl::load_text(sforest, forest.estimators(i), false);
  }
}

}  // namespace Impl
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_IMPL_RANDOMFOREST_LOADER_IMPL_HPP