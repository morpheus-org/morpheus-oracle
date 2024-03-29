/**
 * MorpheusOracle_DecisionTreeTuner.hpp
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

#ifndef MORPHEUSORACLE_DECISIONTREETUNER_HPP
#define MORPHEUSORACLE_DECISIONTREETUNER_HPP

#include <MorpheusOracle_MLTuner.hpp>
#include <MorpheusOracle_DecisionTree.hpp>

#include <vector>

namespace Morpheus {
namespace Oracle {

/**
 * \addtogroup tuners Tuners
 *
 */

class DecisionTreeTuner : public MLTuner<DecisionTreeTuner> {
 public:
  using scalar_vector = std::vector<double>;

  /*! Value specifying the state of the tuner when the optimum state has
   * not been yet selected*/
  static const int INVALID_STATE = -1;

  /**
   * @brief Construct a new DecisionTreeTuner object
   *
   * @param tree A DecisionTree to be used by the tuner.
   */
  DecisionTreeTuner(const DecisionTree& tree, bool verbose = false)
      : tree_(tree),
        timings_(2, 0),
        state_id_(INVALID_STATE),
        verbose_(verbose) {}

  DecisionTreeTuner(const std::string& filename, const bool binary = true,
                    const bool feature_names = true, bool verbose = false) {
    reload(filename, binary, feature_names);
    verbose_ = verbose;
  }

  void reload(const std::string& filename, const bool binary = true,
              const bool feature_names = true) {
    tree_.load_tree(filename, binary, feature_names);
    reset();
  }

  template <typename SampleVector>
  void run(const SampleVector& sample) {
    state_id_ = tree_.evaluate(sample);
  }
  /**
   * @brief Resets the state of the tuner to the initial state.
   *
   */
  void reset() {
    state_id_ = INVALID_STATE;
    timings_.resize(2, 0);
  }

  /**
   * @brief Prints the state of the tuner.
   *
   */
  void print() {
    using namespace std;
    cout << "Tuner executed using a DecisionTree loaded from: "
         << tree_.filename() << endl;
    cout << endl;

    if (is_verbose()) {
      cout << "Metadata:" << endl;
      cout << "---------" << endl;
      cout << setw(25) << "Number of Features: " << tree_.nfeatures() << endl;
      cout << setw(25) << "Number of Classes: " << tree_.nclasses() << endl;
      cout << setw(25) << "Number of Nodes: " << tree_.nodecount() << endl;
      cout << setw(25) << "Max Depth: " << tree_.maxdepth() << endl;

      cout << setw(25) << "Classes: ";
      for (size_t i = 0; i < tree_.cclasses().size(); i++) {
        cout << tree_.cclasses(i) << "\t";
      }
      cout << endl;

      cout << setw(25) << "Feature Names: ";
      for (size_t i = 0; i < tree_.cfeature_names().size(); i++) {
        cout << tree_.cfeature_names(i) << "\t";
      }
      cout << endl;
      cout << endl;
    }

    cout << "Tuner timing statistics:" << endl;
    cout << "------------------------" << endl;
    cout << setw(20) << "Feature Extraction: " << timings_[0] << " (s)" << endl;
    cout << setw(20) << "Inference: " << timings_[1] << " (s)" << endl;
    cout << endl;
    cout << "Optimum Format ID: " << state_id() << endl;
  }

  /**
   * @brief Provides a one-dimensional vector that contains the timings obtained
   * for feature extraction and inference.
   *
   * @return scalar_vector& A one-dimensional vector containing timings.
   */
  scalar_vector& timings() { return timings_; }

  /**
   * @brief Provides the index of the optimum format selected by the tuner. Note
   * that until the tuner finishes the tuning process, the format ID is set to
   * \p INVALID_STATE.
   *
   * @return int Optimum format index.
   */
  int state_id() const { return state_id_; }

  /**
   * @brief Checks if the tuner prints verbose messages.
   *
   * @return true Tuner prints verbose messages.
   * @return false Tuner does not print verbose messages.
   */
  bool is_verbose() { return verbose_; }

  /**
   * @brief Whether to enable verbose messages by the tuner.
   *
   * @param verbose Boolean option for setting the verboseness of the tuner.
   */
  void set_verbose(bool verbose = true) { verbose_ = verbose; }

  /*! \cond */
 private:
  DecisionTree tree_;
  scalar_vector timings_;
  int state_id_;
  bool verbose_;
  /*! \endcond */
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_DECISIONTREETUNER_HPP
