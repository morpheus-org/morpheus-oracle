/**
 * MorpheusOracle_RandomForestTuner.hpp
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

#ifndef MORPHEUSORACLE_RANDOMFORESTTUNER_HPP
#define MORPHEUSORACLE_RANDOMFORESTTUNER_HPP

#include <MorpheusOracle_MLTuner.hpp>
#include <MorpheusOracle_RandomForest.hpp>

#include <vector>

namespace Morpheus {
namespace Oracle {

/**
 * \addtogroup tuners Tuners
 *
 */

class RandomForestTuner : public MLTuner<RandomForestTuner> {
 public:
  using scalar_vector = std::vector<double>;

  /*! Value specifying the state of the tuner when the optimum state has
   * not been yet selected*/
  static const int INVALID_STATE = -1;

  /**
   * @brief Construct a new RandomForestTuner object
   *
   * @param forest A RandomForest to by used by the tuner.
   */
  RandomForestTuner(const RandomForest& forest, bool verbose = false)
      : forest_(forest),
        timings_(2, 0),
        state_id_(INVALID_STATE),
        verbose_(verbose) {}

  RandomForestTuner(const std::string& filename, const bool binary = true,
                    const bool feature_names = true, bool verbose = false) {
    reload(filename, binary, feature_names);
    verbose_ = verbose;
  }

  void reload(const std::string& filename, const bool binary = true,
              const bool feature_names = true) {
    forest_.load_forest(filename, binary, feature_names);
    reset();
    forest_.print();
  }

  template <typename SampleVector>
  void run(const SampleVector& sample) {
    state_id_ = forest_.evaluate(sample);
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
    cout << "Tuner executed using a RandomForest loaded from: "
         << forest_.filename() << endl;
    cout << endl;
    if (is_verbose()) {
      cout << endl;
      cout << "Metadata:" << endl;
      cout << "---------" << endl;
      cout << setw(25) << "Number of Features: " << forest_.nfeatures() << endl;
      cout << setw(25) << "Number of Classes: " << forest_.nclasses() << endl;
      cout << setw(25) << "Number of Outputs: " << forest_.noutputs() << endl;
      cout << setw(25) << "Number of Estimators: " << forest_.nestimators()
           << endl;

      cout << setw(25) << "Classes: ";
      for (size_t i = 0; i < forest_.cclasses().size(); i++) {
        cout << forest_.cclasses(i) << "\t";
      }
      cout << endl;

      cout << setw(25) << "Feature Names: ";
      for (size_t i = 0; i < forest_.cfeature_names().size(); i++) {
        cout << forest_.cfeature_names(i) << "\t";
      }
      cout << endl;
      cout << endl;
    }
    cout << endl;
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
  RandomForest forest_;
  scalar_vector timings_;
  int state_id_;
  bool verbose_;
  /*! \endcond */
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RANDOMFORESTTUNER_HPP
