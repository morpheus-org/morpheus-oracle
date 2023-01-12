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
#include <fwd/MorpheusOracle_Fwd_DecisionTree.hpp>
#include <impl/MorpheusOracle_Loader_Impl.hpp>
#include <Morpheus_Core.hpp>

#include <vector>

namespace Morpheus {
namespace Oracle {

/**
 * \defgroup loader Loader
 * \brief Routines for loading DecisionTrees and RandomForest from file
 * \{
 *
 */

template <typename Tree>
void load_tree(const std::string& filename, Tree& tree,
               typename std::enable_if_t<is_decision_tree_v<Tree>>* = nullptr) {
  std::ifstream file(filename.c_str());

  Impl::skip_comment(file);
  // Read four entries (NFeatures, Nclasses, NodeCount, MaxDepth)
  std::vector<std::string> tokens;
  std::string line;
  std::getline(file, line);
  Impl::tokenize(tokens, line);

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
  Impl::load_array(tree.classes(), file, true);

  tree.feature_names().resize(nfeatures);
  Impl::load_array(tree.feature_names(), file, true);

  tree.left_child().resize(nodecount, 0);
  Impl::load_array(tree.left_child(), file, true);

  tree.right_child().resize(nodecount, 0);
  Impl::load_array(tree.right_child(), file, true);

  tree.threshold().resize(nodecount, 0);
  Impl::load_array(tree.threshold(), file, true);

  tree.features().resize(nodecount, 0);
  Impl::load_array(tree.features(), file, true);

  tree.values().resize(nodecount, nclasses);
  Impl::load_array(tree.values(), file, true);
}

template <typename Forest>
void load_forest(
    const std::string& fmetadata, const std::vector<std::string>& ftrees,
    Forest& forest,
    typename std::enable_if_t<is_random_forest_v<Forest>>* = nullptr) {
  // Load Metadata
  {
    std::ifstream file(fmetadata.c_str());

    Impl::skip_comment(file);
    // Read four entries (NFeatures, Nclasses, NOutputs)
    std::vector<std::string> tokens;
    std::string line;
    std::getline(file, line);
    Impl::tokenize(tokens, line);

    size_t nfeatures, nclasses, noutputs;
    std::istringstream(tokens[0]) >> nfeatures;
    std::istringstream(tokens[1]) >> nclasses;
    std::istringstream(tokens[2]) >> noutputs;

    forest.set_nfeatures(nfeatures);
    forest.set_nclasses(nclasses);
    forest.set_noutputs(noutputs);

    forest.classes().resize(nclasses, 0);
    Impl::load_array(forest.classes(), file, true);

    forest.feature_names().resize(nfeatures);
    Impl::load_array(forest.feature_names(), file, true);
  }
  // Load each tree
  {
    forest.estimators().resize(ftrees.size());
    for (size_t i = 0; i < ftrees.size(); i++) {
      load_tree(ftrees[i], forest.estimators(i));
    }
  }
}

/*! \} end of loader group
 */

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_LOADER_HPP