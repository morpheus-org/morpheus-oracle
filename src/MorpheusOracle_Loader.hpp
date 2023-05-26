/**
 * MorpheusOracle_Loader.hpp
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

#ifndef MORPHEUSORACLE_LOADER_HPP
#define MORPHEUSORACLE_LOADER_HPP

#include <MorpheusOracle_TypeTraits.hpp>
#include <impl/MorpheusOracle_Loader_Impl.hpp>

namespace Morpheus {
namespace Oracle {

/**
 * \defgroup loader Loader
 * \brief Routines for loading DecisionTrees and RandomForest from file
 * \{
 *
 */

template <typename Tree>
void load(std::ifstream& stree, Tree& tree, const bool binary = true,
          const bool feature_names = true) {
  static_assert(
      is_decision_tree_v<Tree> || is_random_forest_v<Tree>,
      "Input Structure must be either a DecisionTree or RandomForest!");
  if (binary) {
    Impl::load_binary(stree, tree, feature_names);
  } else {
    Impl::load_text(stree, tree, feature_names);
  }
}

template <typename Tree>
void load(const std::string& ftree, Tree& tree, const bool binary = true,
          const bool feature_names = true) {
  auto mode = binary ? std::ios::in | std::ios::binary : std::ios::in;
  std::ifstream file(ftree.c_str(), mode);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file at: " + ftree);
  }
  file.seekg(0, std::ios::beg);

  load(file, tree, binary, feature_names);
}

/*! \} end of loader group
 */

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_LOADER_HPP