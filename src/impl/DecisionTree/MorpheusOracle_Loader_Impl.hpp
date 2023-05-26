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

#ifndef MORPHEUSORACLE_IMPL_DECISIONTREE_LOADER_IMPL_HPP
#define MORPHEUSORACLE_IMPL_DECISIONTREE_LOADER_IMPL_HPP

#include <MorpheusOracle_TypeTraits.hpp>
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
//     std::ifstream& stree, Tree& tree, const bool feature_names = true,
//     typename std::enable_if_t<is_decision_tree_v<Tree>>* = nullptr) {}

template <typename Tree>
void load_binary(
    std::ifstream&, Tree&, bool,
    typename std::enable_if_t<is_decision_tree_v<Tree>>* = nullptr) {}

template <typename Tree>
void load_text(std::ifstream& stree, Tree& tree,
               const bool feature_names                             = true,
               typename std::enable_if_t<is_decision_tree_v<Tree>>* = nullptr) {
  Impl::skip_comment(stree);
  // Read four entries (NFeatures, Nclasses, NodeCount, MaxDepth)
  std::vector<std::string> tokens;
  std::string line;
  std::getline(stree, line);
  Morpheus::IO::Impl::tokenize(tokens, line);

  size_t nfeatures, nclasses, nodecount, maxdepth;
  std::istringstream(tokens[0]) >> nfeatures;
  std::istringstream(tokens[1]) >> nclasses;
  std::istringstream(tokens[2]) >> nodecount;
  std::istringstream(tokens[3]) >> maxdepth;

  tree.set_nfeatures(nfeatures);
  tree.set_nclasses(nclasses);
  tree.set_nodecount(nodecount);
  tree.set_maxdepth(maxdepth);

  tree.classes().resize(nclasses, 0);
  Impl::load_array(tree.classes(), stree, true);

  if (feature_names) {
    tree.feature_names_sizes().resize(nfeatures);
    Impl::load_array(tree.feature_names_sizes(), stree, true);
    tree.feature_names().resize(nfeatures);
    Impl::load_array(tree.feature_names(), stree, true);
  }

  tree.left_child().resize(nodecount, 0);
  Impl::load_array(tree.left_child(), stree, true);

  tree.right_child().resize(nodecount, 0);
  Impl::load_array(tree.right_child(), stree, true);

  tree.threshold().resize(nodecount, 0);
  Impl::load_array(tree.threshold(), stree, true);

  tree.features().resize(nodecount, 0);
  Impl::load_array(tree.features(), stree, true);

  tree.values().resize(nodecount, nclasses);
  Impl::load_array(tree.values(), stree, true);
}

}  // namespace Impl
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_IMPL_DECISIONTREE_LOADER_IMPL_HPP