/**
 * MorpheusOracle_RandomForest.hpp
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

#ifndef MORPHEUSORACLE_RANDOMFOREST_HPP
#define MORPHEUSORACLE_RANDOMFOREST_HPP

#include <MorpheusOracle_DecisionTree.hpp>
#include <MorpheusOracle_Loader.hpp>
#include <MorpheusOracle_Utils.hpp>
#include <Morpheus_Core.hpp>

namespace Morpheus {
namespace Oracle {

/**
 * \addtogroup containers Containers
 */

/**
 * @brief TODO
 *
 * \par Overview
 * TODO
 *
 * \par Example
 * \code
 * #include <Morpheus_Oracle.hpp>
 *
 * int main(){
 *  Morpheus::initialize();
 *  {
 *    TODO
 *  }
 *  Morpheus::finalize();
 *
 *  return 0;
 * }
 * \endcode
 */
class RandomForest {
 public:
  using size_type   = size_t;
  using index_type  = int;
  using value_type  = double;
  using string_type = std::string;
  using tree_type   = Morpheus::Oracle::DecisionTree;
  /*! A one-dimensional vector of DecisionTrees */
  using tree_vector = std::vector<tree_type>;
  /*! A one-dimensional vector of doubles */
  using scalar_vector = Morpheus::DenseVector<value_type, Kokkos::HostSpace>;
  /*! A one-dimensional vector of integers */
  using index_vector = Morpheus::DenseVector<index_type, Kokkos::HostSpace>;
  /*! A one-dimensional vector of strings */
  using string_vector = std::vector<string_type>;

  /**
   * @brief Construct a new DecisionTree object
   *
   * @param filename Absolute path to the filename to read the tree from.
   */
  RandomForest() = default;

  RandomForest(const std::string& fmetadata, const string_vector& ftrees)
      : nfeatures_(0),
        nclasses_(0),
        noutputs_(0),
        classes_(),
        feature_names_(),
        estimators_(),
        metadata_filename_(fmetadata),
        tree_filenames_(ftrees) {
    load_forest(fmetadata, ftrees);
  }

  RandomForest(const RandomForest& forest)
      : nfeatures_(forest.nfeatures()),
        nclasses_(forest.nclasses()),
        noutputs_(forest.noutputs()),
        classes_(forest.cclasses()),
        feature_names_(forest.cfeature_names()),
        estimators_(forest.cestimators()),
        metadata_filename_(forest.meatadata_filename()),
        tree_filenames_(forest.ctree_filenames()) {}

  RandomForest& operator=(const RandomForest& forest) {
    nfeatures_         = forest.nfeatures();
    nclasses_          = forest.nclasses();
    noutputs_          = forest.noutputs();
    classes_           = forest.cclasses();
    feature_names_     = forest.cfeature_names();
    estimators_        = forest.cestimators();
    metadata_filename_ = forest.meatadata_filename();
    tree_filenames_    = forest.ctree_filenames();
    return *this;
  }

  void load_forest(const std::string& fmetadata, const string_vector& ftrees) {
    metadata_filename_ = fmetadata;
    tree_filenames_    = ftrees;
    Morpheus::Oracle::load_forest(fmetadata, ftrees, *this);
  }

  index_type evaluate(const scalar_vector& sample) {
    using backend = typename index_vector::backend;
    index_vector voters(estimators().size(), 0);
    for (size_type i = 0; i < estimators().size(); i++) {
      voters[i] = estimators(i).recurse(sample);
    }

    // Majority voting to determine the selected class
    index_vector occurrences(nclasses(), 0);
    Morpheus::count_occurences<backend>(voters, occurrences);

    // find index with the largest value
    index_type idx = 0;
    index_type max = std::numeric_limits<index_type>::min();
    for (size_type i = 0; i < nclasses(); i++) {
      idx = max < occurrences(i) ? i : idx;
      max = max < occurrences(i) ? occurrences(i) : max;
    }

    return idx;
  }

  void print(size_t offset = 25) {
    std::cout << std::setw(offset) << "Number of Features: " << nfeatures()
              << std::endl;
    std::cout << std::setw(offset) << "Number of Classes: " << nclasses()
              << std::endl;
    std::cout << std::setw(offset) << "Number of Outputs: " << noutputs()
              << std::endl;

    for (size_type i = 0; i < estimators().size(); i++) {
      std::cout << std::setw(offset) << "Estimator " << i << ": " << std::endl;
      std::cout << std::setw(offset + 4) << "{" << std::endl;
      estimators(i).print(offset + 25);
      std::cout << std::setw(offset + 4) << "}" << std::endl;
    }
  }

  size_type nfeatures() const { return nfeatures_; }
  size_type nclasses() const { return nclasses_; }
  size_type noutputs() const { return noutputs_; }
  string_type meatadata_filename() const { return metadata_filename_; }
  const string_vector& ctree_filenames() const { return tree_filenames_; }

  void set_nfeatures(const size_t nfeatures) { nfeatures_ = nfeatures; }
  void set_nclasses(const size_t nclasses) { nclasses_ = nclasses; }
  void set_noutputs(const size_t noutputs) { noutputs_ = noutputs; }

  index_type& classes(size_t i) { return classes_[i]; }
  tree_type& estimators(size_t i) { return estimators_[i]; }
  string_type& feature_names(size_t i) { return feature_names_[i]; }

  const index_type& cclasses(size_t i) const { return classes_[i]; }
  const tree_type& cestimators(size_t i) const { return estimators_[i]; }
  const string_type& cfeature_names(size_t i) const {
    return feature_names_[i];
  }

  index_vector& classes() { return classes_; }
  tree_vector& estimators() { return estimators_; }
  string_vector& feature_names() { return feature_names_; }

  const index_vector& cclasses() const { return classes_; }
  const tree_vector& cestimators() const { return estimators_; }
  const string_vector& cfeature_names() const { return feature_names_; }

  /*! \cond */
 private:
  size_type nfeatures_, nclasses_, noutputs_;
  index_vector classes_;
  string_vector feature_names_;
  tree_vector estimators_;
  string_type metadata_filename_;
  string_vector tree_filenames_;
  /*! \endcond */
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RANDOMFOREST_HPP
