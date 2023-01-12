/**
 * MorpheusOracle_DecisionTree.hpp
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

#ifndef MORPHEUSORACLE_DECISIONTREE_HPP
#define MORPHEUSORACLE_DECISIONTREE_HPP

#include <MorpheusOracle_Loader.hpp>
#include <MorpheusOracle_Utils.hpp>
#include <Morpheus_Core.hpp>

namespace Morpheus {
namespace Oracle {

/**
 * \addtogroup containers Containers
 * \par Overview
 * TODO
 * \brief TODO
 * \{
 *
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
class DecisionTree {
 public:
  using size_type   = size_t;
  using index_type  = int;
  using value_type  = double;
  using string_type = std::string;
  /*! A two-dimensional vector of doubles */
  using scalar_vector2d =
      Morpheus::DenseMatrix<value_type, size_type, Kokkos::HostSpace>;
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
  DecisionTree() = default;

  DecisionTree(const std::string& filename)
      : nfeatures_(0),
        nclasses_(0),
        nodecount_(0),
        maxdepth_(0),
        classes_(),
        feature_names_(),
        left_child_(),
        right_child_(),
        threshold_(),
        features_(),
        values_() {
    load_tree(filename);
  }

  DecisionTree(const DecisionTree& tree)
      : nfeatures_(tree.nfeatures()),
        nclasses_(tree.nclasses()),
        nodecount_(tree.nodecount()),
        maxdepth_(tree.maxdepth()),
        classes_(tree.cclasses()),
        feature_names_(tree.cfeature_names()),
        left_child_(tree.cleft_child()),
        right_child_(tree.cright_child()),
        threshold_(tree.cthreshold()),
        features_(tree.cfeatures()),
        values_(tree.cvalues()) {}

  DecisionTree& operator=(const DecisionTree& tree) {
    nfeatures_     = tree.nfeatures();
    nclasses_      = tree.nclasses();
    nodecount_     = tree.nodecount();
    maxdepth_      = tree.maxdepth();
    classes_       = tree.cclasses();
    feature_names_ = tree.cfeature_names();
    left_child_    = tree.cleft_child();
    right_child_   = tree.cright_child();
    threshold_     = tree.cthreshold();
    features_      = tree.cfeatures();
    values_        = tree.cvalues();
    return *this;
  }

  void load_tree(const std::string& filename) {
    Morpheus::Oracle::load_tree(filename, *this);
  }

  index_type evaluate(const scalar_vector& sample) { return recurse(sample); }

  index_type recurse(const scalar_vector& sample, int node = 0) {
    if (threshold(node) != LEAF) {
      if (sample[features(node)] <= threshold(node)) {
        return recurse(sample, left_child(node));
      } else {
        return recurse(sample, right_child(node));
      }
    } else {
      // find index with the largest value
      index_type idx = 0;
      value_type max = std::numeric_limits<value_type>::min();
      for (size_type i = 0; i < values().ncols(); i++) {
        idx = max < values(node, i) ? i : idx;
        max = max < values(node, i) ? values(node, i) : max;
      }
      return classes(idx);
    }
  }

  void print(size_t offset = 25) {
    std::cout << std::setw(offset) << "Number of Features: " << nfeatures()
              << std::endl;
    std::cout << std::setw(offset) << "Number of Classes: " << nclasses()
              << std::endl;
    std::cout << std::setw(offset) << "Number of Nodes: " << nodecount()
              << std::endl;
    std::cout << std::setw(offset) << "Max Depth: " << maxdepth() << std::endl;

    std::cout << std::setw(offset) << "Classes : ";
    print_array(classes(), nclasses(), std::cout);
    std::cout << std::setw(offset) << "Feature Names : ";
    print_array(feature_names(), feature_names().size(), std::cout);
    std::cout << std::setw(offset) << "Left Children : ";
    print_array(left_child(), nodecount(), std::cout);
    std::cout << std::setw(offset) << "Right Children : ";
    print_array(right_child(), nodecount(), std::cout);
    std::cout << std::setw(offset) << "Thresholds : ";
    print_array(threshold(), nodecount(), std::cout);
    std::cout << std::setw(offset) << "Features : ";
    print_array(features(), nodecount(), std::cout);

    std::cout << std::setw(offset) << "Values : ";
    print_matrix(values(), nodecount(), nclasses(), std::cout, offset + 2);
  }

  size_type nfeatures() const { return nfeatures_; }
  size_type nclasses() const { return nclasses_; }
  size_type nodecount() const { return nodecount_; }
  size_type maxdepth() const { return maxdepth_; }

  void set_nfeatures(const size_t nfeatures) { nfeatures_ = nfeatures; }
  void set_nclasses(const size_t nclasses) { nclasses_ = nclasses; }
  void set_nodecount(const size_t nodecount) { nodecount_ = nodecount; }
  void set_maxdepth(const size_t maxdepth) { maxdepth_ = maxdepth; }

  index_type& classes(size_t i) { return classes_(i); }
  index_type& left_child(size_t i) { return left_child_(i); }
  index_type& right_child(size_t i) { return right_child_(i); }
  value_type& threshold(size_t i) { return threshold_(i); }
  value_type& features(size_t i) { return features_(i); }
  value_type& values(size_t i, size_t j) { return values_(i, j); }
  string_type& feature_names(size_t i) { return feature_names_[i]; }

  const index_type& cclasses(size_t i) const { return classes_(i); }
  const index_type& cleft_child(size_t i) const { return left_child_(i); }
  const index_type& cright_child(size_t i) const { return right_child_(i); }
  const value_type& cthreshold(size_t i) const { return threshold_(i); }
  const value_type& cfeatures(size_t i) const { return features_(i); }
  const value_type& cvalues(size_t i, size_t j) const { return values_(i, j); }
  const string_type& cfeature_names(size_t i) const {
    return feature_names_[i];
  }

  index_vector& classes() { return classes_; }
  index_vector& left_child() { return left_child_; }
  index_vector& right_child() { return right_child_; }
  scalar_vector& threshold() { return threshold_; }
  scalar_vector& features() { return features_; }
  scalar_vector2d& values() { return values_; }
  string_vector& feature_names() { return feature_names_; }

  const index_vector& cclasses() const { return classes_; }
  const index_vector& cleft_child() const { return left_child_; }
  const index_vector& cright_child() const { return right_child_; }
  const scalar_vector& cthreshold() const { return threshold_; }
  const scalar_vector& cfeatures() const { return features_; }
  const scalar_vector2d& cvalues() const { return values_; }
  const string_vector& cfeature_names() const { return feature_names_; }

  /*! \cond */
 private:
  size_type nfeatures_, nclasses_, nodecount_, maxdepth_;
  index_vector classes_;
  string_vector feature_names_;
  index_vector left_child_;
  index_vector right_child_;
  scalar_vector threshold_;
  scalar_vector features_;
  scalar_vector2d values_;
  const int LEAF = -2;
  /*! \endcond */
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_DECISIONTREE_HPP
