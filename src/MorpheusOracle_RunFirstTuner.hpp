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
#include <limits>

namespace Morpheus {
namespace Oracle {

class RunFirstTuner {
 public:
  RunFirstTuner(const size_t rep_limit = 10, const bool verbose = false)
      : timings_(Morpheus::NFORMATS, rep_limit, 0),
        max_timings_(Morpheus::NFORMATS, 0),
        avg_timings_(Morpheus::NFORMATS, 0),
        min_timings_(Morpheus::NFORMATS, 0),
        format_id_(0),
        format_count_(0),
        nformats_(Morpheus::NFORMATS),
        rep_limit_(rep_limit),
        rep_count_(0),
        verbose_(verbose) {
    if (verbose) {
      std::cout << "RunFirstTuner :: Tuner will run for " << rep_limit
                << " repetitions.\n";
      std::cout << "                 "
                << "Each repetition tunes for " << nformats_ << " formats.\n";
      std::cout << "  Repetition Number   |   Format ID" << std::endl;
    }
  }

  void operator++() {
    if (format_count_ < nformats_ - 1) {
      format_count_++;
    } else {
      format_count_ = 0;
      rep_count_++;
    }
  }

  int nformats() const { return nformats_; }

  int format_id() const { return format_id_; }

  size_t repetition_count() const { return rep_count_; }

  size_t repetition_limit() const { return rep_limit_; }

  bool finished() {
    if (verbose_) {
      std::cout << "       " << std::setw(10) << repetition_count();
      std::cout << "       " << std::setw(10) << format_id() << std::endl;
    }

    int completed = repetition_count() >= repetition_limit() ? true : false;

    if (completed) {
      compute_max_timings_();
      compute_avg_timings_();
      compute_min_timings_();
    }
    return completed;
  }

  void register_run(double runtime) {
    timings_(format_id_, rep_count_) = runtime;
  }

  void set_verbose(bool verbose = true) { verbose_ = verbose; }

  bool is_verbose() { return verbose_; }

  void reset() {
    format_id_ = 0;
    rep_count_ = 0;
    timings_.assign(nformats_, rep_limit_, 0);
  }

  void print() {
    if (repetition_count() == 0) {
      std::cout << "Run-first Tuner configured with repetition limit "
                << repetition_limit() << std::endl;
      return;
    }

    if (repetition_count() >= repetition_limit()) {
      std::cout << "Tuner reached repetition limit of " << repetition_limit()
                << "repetitions!" << std::endl;
    } else {
      throw std::runtime_error("Tuner is in inconsistent state.");
    }
  }

 private:
  void compute_max_timings_() {
    for (int i = 0; i < nformats_; i++) {
      double maxt = std::numeric_limits<double>::min();
      for (int j = 0; j < rep_limit_; j++) {
        if (timings_(i, j) > maxt) {
          max_timings_(i) = maxt;
        }
      }
    }
  }

  void compute_avg_timings_() {
    for (int i = 0; i < nformats_; i++) {
      double sumt = 0.0;
      for (int j = 0; j < rep_limit_; j++) {
        sumt = timings_(i, j);
      }
      avg_timings_(i) = sumt / rep_limit_;
    }
  }

  void compute_min_timings_() {
    for (int i = 0; i < nformats_; i++) {
      double mint = std::numeric_limits<double>::max();
      for (int j = 0; j < rep_limit_; j++) {
        if (timings_(i, j) < mint) {
          min_timings_(i) = mint;
        }
      }
    }
  }

  using vec2d = Morpheus::DenseMatrix<double, int, Kokkos::HostSpace>;
  using vec   = Morpheus::DenseVector<double, Kokkos::HostSpace>;
  vec2d timings_;
  vec max_timings_;
  vec avg_timings_;
  vec min_timings_;
  int format_id_;
  int format_count_;
  int nformats_;
  size_t rep_limit_;
  size_t rep_count_;
  bool verbose_;
};

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RUNFIRSTTUNER_HPP
