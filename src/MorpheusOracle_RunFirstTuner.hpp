/**
 * MorpheusOracle_RunFirstTuner.hpp
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

#ifndef MORPHEUSORACLE_RUNFIRSTTUNER_HPP
#define MORPHEUSORACLE_RUNFIRSTTUNER_HPP

#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

namespace MorpheusOracle {

class RunFirstTuner {
 public:
  RunFirstTuner(const size_t rep_limit = 10, const bool verbose = false)
      : format_id_(0),
        format_ids(rep_limit, 0),
        rep_limit_(rep_limit),
        rep_count_(0),
        verbose_(verbose) {
    if (verbose) {
      std::cout << "Tuner will run for " << rep_limit << " repetitions"
                << std::endl;
      std::cout << "  Repetition Number  | Format ID" << std::endl;
    }
  }

  void operator++() { rep_count_++; }

  int format_id() const { return format_id_; }

  size_t repetition_count() const { return rep_count_; }

  size_t repetition_limit() const { return rep_limit_; }

  bool finished(int fmt_id) {
    format_id_ = fmt_id;
    format_ids.push_back(format_id_);

    if (verbose_) {
      std::cout << "       " << std::setw(10) << repetition_count();
      std::cout << "       " << std::setw(10) << format_id() << std::endl;
    }

    return repetition_count() >= repetition_limit() ? true : false;
  }

  void set_verbose(bool verbose_ = true) { verbose_ = verbose_; }

  bool is_verbose() { return verbose_; }

  void reset() {
    format_id_ = 0;
    rep_count_ = 0;
    format_ids.resize(0);
  }

  int most_common() {
    int max = 0;
    int most_common_ = -1;
    std::map<int, int> map_;
    for (auto vi = format_ids.begin(); vi != format_ids.end(); vi++) {
      map_[*vi]++;
      if (map_[*vi] > max) {
        max = map_[*vi];
        most_common_ = *vi;
      }
    }

    return most_common_;
  }

  void print() {
    if (repetition_count() == 0) {
      std::cout << "Run-first Tuner configured with repetition limit "
                << repetition_limit() << std::endl;
      return;
    }

    // report solver results
    if (repetition_count() >= repetition_limit()) {
      std::cout << "Tuner reached repetition limit of " << repetition_limit()
                << "repetitions!" << std::endl;
    } else {
      throw std::runtime_error("Tuner is in inconsistent state.");
    }

    std::cout << "Most common format : " << most_common() << std::endl;
  }

 private:
  int format_id_;
  std::vector<int> format_ids;
  size_t rep_limit_;
  size_t rep_count_;
  bool verbose_;
};

}  // namespace MorpheusOracle

#endif  // MORPHEUSORACLE_RUNFIRSTTUNER_HPP
