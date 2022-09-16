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
      std::cout << "RunFirstTuner :: Tuner will run for " << nformats()
                << " formats.\n";
      std::cout << "                 "
                << "Each format tunes for " << repetition_limit()
                << " repetitions.\n";

      std::cout << std::setw(10) << "Format ID";
      std::cout << std::setw(3) << "|";
      std::cout << std::setw(12) << "Repetition" << std::endl;

      std::cout << std::setw(13) << "|";
      std::cout << std::setw(12) << "Number" << std::endl;
      std::cout << "-------------------------" << std::endl;
    }
  }

  void operator++() {
    if (repetition_count() < repetition_limit() - 1) {
      rep_count_++;
    } else {
      format_count_++;
      rep_count_ = 0;
    }
  }

  size_t format_count() const { return format_count_; }

  size_t nformats() const { return nformats_; }

  size_t format_id() const {
    if (format_count() >= nformats()) {
      return format_id_;
    } else {
      throw std::runtime_error(
          "Tuner is in inconsistent state. Requesting the optimal format_id "
          "can be done once tuning finishes.");
    }
  }

  size_t repetition_count() const { return rep_count_; }

  size_t repetition_limit() const { return rep_limit_; }

  bool finished() {
    if (verbose_) {
      std::cout << std::setw(10) << format_count();
      std::cout << std::setw(3) << "|";
      std::cout << std::setw(12) << repetition_count();
      std::cout << std::endl;
    }

    bool completed = format_count() >= nformats() ? true : false;

    if (completed) {
      compute_max_timings_();
      compute_avg_timings_();
      compute_min_timings_();
      compute_best_format_id_();
    }
    return completed;
  }

  void register_run(double runtime) {
    timings_(format_count(), repetition_count()) = runtime;
  }

  void set_verbose(bool verbose = true) { verbose_ = verbose; }

  bool is_verbose() { return verbose_; }

  void reset() {
    format_id_ = 0;
    rep_count_ = 0;
    timings_.assign(nformats_, rep_limit_, 0);
    max_timings_.assign(nformats_, 0);
    avg_timings_.assign(nformats_, 0);
    min_timings_.assign(nformats_, 0);
  }

  void print() {
    if ((repetition_count() == 0) && (format_count() == 0)) {
      std::cout << "Run-first Tuner configured with repetition limit "
                << repetition_limit() << std::endl;
      return;
    }

    if (format_count() >= nformats()) {
      using namespace std;
      cout << "Tuner executed " << repetition_limit() << " repetitions and "
           << "optimized for " << nformats() << " formats!" << endl;

      cout << "\nTuner statistics:" << endl;
      cout << setw(10) << "Format ID\t";
      cout << setw(10) << "tmin\t";
      cout << setw(10) << "tmax\t";
      cout << setw(10) << "tavg\t" << endl;

      for (size_t i = 0; i < nformats_; i++) {
        cout << setw(10) << i;
        cout << "\t" << setw(10) << setprecision(7) << min_timings_(i);
        cout << "\t" << setw(10) << setprecision(7) << max_timings_(i);
        cout << "\t" << setw(10) << setprecision(7) << avg_timings_(i);
        cout << endl;
      }

      cout << "Optimum Format ID: " << format_id() << endl;
    } else {
      throw std::runtime_error(
          "Tuner is in inconsistent state. Requesting to print the summary can "
          "be done once tuning finishes.");
    }
  }

 private:
  void compute_best_format_id_() {
    // best format the one with the minimum average
    double mint = avg_timings_(0);
    for (size_t i = 0; i < nformats_; i++) {
      if (avg_timings_(i) < mint) {
        mint       = avg_timings_(i);
        format_id_ = i;
      }
    }
  }

  void compute_max_timings_() {
    for (size_t i = 0; i < nformats_; i++) {
      double maxt = std::numeric_limits<double>::min();
      for (size_t j = 0; j < rep_limit_; j++) {
        if (timings_(i, j) > maxt) {
          max_timings_(i) = timings_(i, j);
          maxt            = timings_(i, j);
        }
      }
    }
  }

  void compute_avg_timings_() {
    for (size_t i = 0; i < nformats_; i++) {
      double sumt = 0.0;
      for (size_t j = 0; j < rep_limit_; j++) {
        sumt += timings_(i, j);
      }
      avg_timings_(i) = sumt / rep_limit_;
    }
  }

  void compute_min_timings_() {
    for (size_t i = 0; i < nformats_; i++) {
      double mint = std::numeric_limits<double>::max();
      for (size_t j = 0; j < rep_limit_; j++) {
        if (timings_(i, j) < mint) {
          min_timings_(i) = timings_(i, j);
          mint            = timings_(i, j);
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
  size_t format_id_;
  size_t format_count_;
  size_t nformats_;
  size_t rep_limit_;
  size_t rep_count_;
  bool verbose_;
};

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RUNFIRSTTUNER_HPP
