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

#include <MorpheusOracle_RandomForest.hpp>

#include <Morpheus_Core.hpp>

namespace Morpheus {
namespace Oracle {

/**
 * \addtogroup tuners Tuners
 *
 */

class RandomForestTuner {
 public:
  using scalar_vector = Morpheus::DenseVector<double, Morpheus::HostSpace>;
  /*! Enum value specifying the state of the tuner when the optimum format
   * has not been yet selected*/
  enum format_state { INVALID_FORMAT_STATE = -1 };

  /**
   * @brief Construct a new RandomForestTuner object
   *
   * @param forest A RandomForest to by used by the tuner.
   */
  RandomForestTuner(const RandomForest& forest, bool verbose = false)
      : forest_(forest),
        timings_(2, 0),
        format_id_(INVALID_FORMAT_STATE),
        verbose_(verbose) {}

  RandomForestTuner(const std::string& fmetadata,
                    const std::vector<std::string>& ftrees,
                    bool verbose = false) {
    reload(fmetadata, ftrees);
    verbose_ = verbose;
  }

  void reload(const std::string& fmetadata,
              const std::vector<std::string>& ftrees) {
    forest_.load_forest(fmetadata, ftrees);
    reset();
  }

  void tune(const scalar_vector& sample) {
    format_id_ = forest_.evaluate(sample);
  }

  /**
   * @brief Resets the state of the tuner to the initial state.
   *
   */
  void reset() {
    format_id_ = INVALID_FORMAT_STATE;
    timings_.resize(2, 0);
  }

  /**
   * @brief Prints the state of the tuner.
   *
   */
  void print() {
    using namespace std;
    cout << "Tuner executed using a RandomForest loaded from: "
         << forest_.meatadata_filename() << endl;
    cout << endl;
    if (is_verbose()) {
      cout << "Trees were loaded from:" << endl;
      std::vector<std::string> tree_filenames = forest_.ctree_filenames();
      for (size_t i = 0; i < tree_filenames.size(); i++) {
        cout << "Tree [" << i << "]: " << tree_filenames[i] << endl;
      }
    }
    cout << endl;
    cout << "Tuner timing statistics:" << endl;
    cout << "------------------------" << endl;
    cout << setw(20) << "Feature Extraction: " << timings_[0] << " (s)" << endl;
    cout << setw(20) << "Inference: " << timings_[1] << " (s)" << endl;
    cout << endl;
    cout << "Optimum Format ID: " << format_id() << endl;
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
   * \p INVALID_FORMAT_STATE.
   *
   * @return size_t Optimum format index.
   */
  size_t format_id() const { return format_id_; }

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
  size_t format_id_;
  bool verbose_;
  /*! \endcond */
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RANDOMFORESTTUNER_HPP
