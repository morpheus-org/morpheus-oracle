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

template <typename Tree>
void load_binary(
    std::ifstream& sforest, Tree& forest, const bool feature_names = true,
    typename std::enable_if_t<is_random_forest_v<Tree>>* = nullptr) {
  const size_t integral_size = sizeof(int);
  const size_t char_size     = sizeof(char);

  std::vector<char> memblock(4 * integral_size);
  sforest.read(memblock.data(), 4 * integral_size);

  char* ptr     = memblock.data();
  int* int_data = (int*)(memblock.data());
  forest.set_nfeatures(int_data[0]);
  forest.set_nclasses(int_data[1]);
  forest.set_noutputs(int_data[2]);
  forest.set_nestimators(int_data[3]);

  /* Read Estimator Sizes */
  forest.estimator_sizes().resize(forest.nestimators(), 0);
  memblock.resize(forest.nestimators() * integral_size);
  ptr = memblock.data();
  sforest.read(memblock.data(), forest.nestimators() * integral_size);
  for (size_t i = 0; i < forest.nestimators(); ++i) {
    forest.estimator_sizes(i) = *((int*)ptr);
    ptr += integral_size;
  }

  /* Read Classes */
  forest.classes().resize(forest.nclasses(), 0);
  memblock.resize(forest.nclasses() * integral_size);
  ptr = memblock.data();
  sforest.read(memblock.data(), forest.nclasses() * integral_size);
  for (size_t i = 0; i < forest.nclasses(); ++i) {
    forest.classes(i) = *((int*)ptr);
    ptr += integral_size;
  }

  if (feature_names) {
    /* Read Feature Names Sizes */
    forest.feature_names_sizes().resize(forest.nfeatures(), 0);
    memblock.resize(forest.nfeatures() * integral_size);
    ptr = memblock.data();
    sforest.read(memblock.data(), forest.nfeatures() * integral_size);
    for (size_t i = 0; i < forest.nfeatures(); ++i) {
      forest.feature_names_sizes(i) = *((int*)ptr);
      ptr += integral_size;
    }

    /* Read Feature Name */
    auto total_chars = std::accumulate(forest.feature_names_sizes().data(),
                                       forest.feature_names_sizes().data() +
                                           forest.feature_names_sizes().size(),
                                       0);
    forest.feature_names().resize(forest.nfeatures());
    memblock.resize(total_chars * char_size);
    ptr = memblock.data();
    sforest.read(memblock.data(), total_chars * char_size);
    std::string strblock = memblock.data();
    size_t pos           = 0;
    for (size_t i = 0; i < forest.nfeatures(); ++i) {
      forest.feature_names(i) =
          strblock.substr(pos, forest.feature_names_sizes(i));
      pos += forest.feature_names_sizes(i) * char_size;
    }
  }

  /* Load each tree */
  forest.estimators().resize(forest.nestimators());
  for (size_t i = 0; i < forest.nestimators(); i++) {
    Impl::load_binary(sforest, forest.estimators(i), false);
  }
}

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