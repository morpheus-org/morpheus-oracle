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
#include <numeric>

namespace Morpheus {
namespace Oracle {
namespace Impl {

template <typename Tree>
void load_binary(
    std::ifstream& stree, Tree& tree, const bool feature_names = true,
    typename std::enable_if_t<is_decision_tree_v<Tree>>* = nullptr) {
  const size_t integral_size = sizeof(int);
  const size_t scalar_size   = sizeof(double);
  const size_t char_size     = sizeof(char);

  std::vector<char> memblock(4 * integral_size);
  stree.read(memblock.data(), 4 * integral_size);

  char* ptr     = memblock.data();
  int* int_data = (int*)(memblock.data());
  tree.set_nfeatures(int_data[0]);
  tree.set_nclasses(int_data[1]);
  tree.set_nodecount(int_data[2]);
  tree.set_maxdepth(int_data[3]);

  /* Read Classes */
  tree.classes().resize(tree.nclasses(), 0);
  memblock.resize(tree.nclasses() * integral_size);
  ptr = memblock.data();
  stree.read(memblock.data(), tree.nclasses() * integral_size);
  for (size_t i = 0; i < tree.nclasses(); ++i) {
    tree.classes(i) = *((int*)ptr);
    ptr += integral_size;
  }

  if (feature_names) {
    /* Read Feature Names Sizes */
    tree.feature_names_sizes().resize(tree.nfeatures(), 0);
    memblock.resize(tree.nfeatures() * integral_size);
    ptr = memblock.data();
    stree.read(memblock.data(), tree.nfeatures() * integral_size);
    for (size_t i = 0; i < tree.nfeatures(); ++i) {
      tree.feature_names_sizes(i) = *((int*)ptr);
      ptr += integral_size;
    }

    /* Read Feature Name */
    auto total_chars = std::accumulate(
        tree.feature_names_sizes().data(),
        tree.feature_names_sizes().data() + tree.feature_names_sizes().size(),
        0);
    tree.feature_names().resize(tree.nfeatures());
    memblock.resize(total_chars * char_size);
    ptr = memblock.data();
    stree.read(memblock.data(), total_chars * char_size);
    std::string strblock = memblock.data();
    size_t pos           = 0;
    for (size_t i = 0; i < tree.nfeatures(); ++i) {
      tree.feature_names(i) = strblock.substr(pos, tree.feature_names_sizes(i));
      pos += tree.feature_names_sizes(i) * char_size;
    }
  }

  /* Read Left Children */
  tree.left_child().resize(tree.nodecount(), 0);
  memblock.resize(tree.nodecount() * integral_size);
  ptr = memblock.data();
  stree.read(memblock.data(), tree.nodecount() * integral_size);
  for (size_t i = 0; i < tree.nodecount(); ++i) {
    tree.left_child(i) = *((int*)ptr);
    ptr += integral_size;
  }

  /* Read Right Children */
  tree.right_child().resize(tree.nodecount(), 0);
  memblock.resize(tree.nodecount() * integral_size);
  ptr = memblock.data();
  stree.read(memblock.data(), tree.nodecount() * integral_size);
  for (size_t i = 0; i < tree.nodecount(); ++i) {
    tree.right_child(i) = *((int*)ptr);
    ptr += integral_size;
  }

  /* Read Threshold */
  tree.threshold().resize(tree.nodecount(), 0);
  memblock.resize(tree.nodecount() * scalar_size);
  ptr = memblock.data();
  stree.read(memblock.data(), tree.nodecount() * scalar_size);
  for (size_t i = 0; i < tree.nodecount(); ++i) {
    tree.threshold(i) = *((double*)ptr);
    ptr += scalar_size;
  }

  /* Read Features */
  tree.features().resize(tree.nodecount(), 0);
  memblock.resize(tree.nodecount() * integral_size);
  ptr = memblock.data();
  stree.read(memblock.data(), tree.nodecount() * integral_size);
  for (size_t i = 0; i < tree.nodecount(); ++i) {
    tree.features(i) = *((int*)ptr);
    ptr += integral_size;
  }

  /* Read Values */
  tree.values().resize(tree.nodecount(), tree.nclasses());
  memblock.resize(tree.nodecount() * tree.nclasses() * scalar_size);
  ptr = memblock.data();
  stree.read(memblock.data(), tree.nodecount() * tree.nclasses() * scalar_size);
  for (size_t i = 0; i < tree.nodecount(); ++i) {
    for (size_t j = 0; j < tree.nclasses(); ++j) {
      tree.values(i, j) = *((double*)ptr);
      ptr += scalar_size;
    }
  }
}

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