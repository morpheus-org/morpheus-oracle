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
  RandomForestTuner(const RandomForest& forest)
      : forest_(forest), format_id_(INVALID_FORMAT_STATE) {}

  void reload(const std::string& fmetadata,
              const std::vector<std::string>& ftrees) {
    forest_.load_forest(fmetadata, ftrees);
  }

  void tune(const scalar_vector& sample) {
    format_id_ = forest_.evaluate(sample);
  }

  /**
   * @brief Resets the state of the tuner to the initial state.
   *
   */
  void reset() { format_id_ = INVALID_FORMAT_STATE; }

  /**
   * @brief Provides the index of the optimum format selected by the tuner. Note
   * that until the tuner finishes the tuning process, the format ID is set to
   * \p INVALID_FORMAT_STATE.
   *
   * @return size_t Optimum format index.
   */
  size_t format_id() const { return format_id_; }

  /*! \cond */
 private:
  RandomForest forest_;
  size_t format_id_;
  /*! \endcond */
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RANDOMFORESTTUNER_HPP
