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

#include <Morpheus_Core.hpp>

#include <iomanip>
#include <iostream>
#include <limits>

//! Generic Morpheus interfaces
namespace Morpheus {
//! Morpheus interfaces specific to Oracle package
namespace Oracle {

/**
 * \addtogroup tuners Tuners
 * \par Overview
 * TODO
 * \brief A set of auto-tuners for automatic sparse matrix storage format
 * selection.
 * \{
 *
 */

/**
 * @brief A run-first tuner that selects the best storage format after running
 * the algorithm multiple times.
 *
 * \par Overview
 * The RunFirstTuner is responsible for tuning for the best sparse matrix
 * storage format by first running the operation multiple times.
 *
 * \par Example
 * \code
 * #include <Morpheus_Oracle.hpp>
 *
 * int main(){
 *  Morpheus::initialize();
 *  {
 *    Morpheus::DynamicMatrix<double, Kokkos::HostSpace> A;
 *    Morpheus::CsrMatrix<double, Kokkos::HostSpace> Acsr(4, 3, 6);
 *
 *    // initialize matrix entries
 *    Acsr.row_offsets(0) = 0; Acsr.row_offsets(1) = 2; Acsr.row_offsets(2) = 2;
 *    Acsr.row_offsets(3) = 3; Acsr.row_offsets(4) = 6;
 *
 *    Acsr.column_indices(0) = 0; Acsr.values(0) = 10;
 *    Acsr.column_indices(1) = 2; Acsr.values(1) = 20;
 *    Acsr.column_indices(2) = 2; Acsr.values(2) = 30;
 *    Acsr.column_indices(3) = 0; Acsr.values(3) = 40;
 *    Acsr.column_indices(4) = 1; Acsr.values(4) = 50;
 *    Acsr.column_indices(5) = 2; Acsr.values(5) = 60;
 *
 *    A = Acsr;
 *
 *    // Run the tuner for 10 repetitions and print verbose messages.
 *    Morpheus::Oracle::RunFirstTuner tuner(10, true);
 *
 *    // Tune A for the multiply operation
 *    Morpheus::Oracle::tune_multiply<Kokkos::Serial>(A, tuner);
 *
 *    // Print the tuning report
 *    tuner.print();
 *  }
 *  Morpheus::finalize();
 *
 *  return 0;
 * }
 * \endcode
 */
class RunFirstTuner {
 public:
  /*! A two-dimensional vector of doubles */
  using vec2d = Morpheus::DenseMatrix<double, size_t, Kokkos::HostSpace>;
  /*! A one-dimensional vector of doubles */
  using vec = Morpheus::DenseVector<double, Kokkos::HostSpace>;

  /*! Enum value specifying the state of the tuner when the optimum format has
   * not been yet selected*/
  enum format_state { INVALID_FORMAT_STATE = -1 };

  /**
   * @brief Construct a new RunFirstTuner object
   *
   * @param rep_limit Number of repetitions to run the tuner for.
   * @param verbose  Whether to print verbose messages.
   */
  RunFirstTuner(const size_t rep_limit = 10, const bool verbose = false)
      : timings_(Morpheus::NFORMATS, rep_limit, 0),
        max_timings_(Morpheus::NFORMATS, 0),
        avg_timings_(Morpheus::NFORMATS, 0),
        min_timings_(Morpheus::NFORMATS, 0),
        format_id_(INVALID_FORMAT_STATE),
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

  /**
   * @brief Advance the tuner by one step.
   *
   */
  void operator++() {
    if (repetition_count() < repetition_limit() - 1) {
      rep_count_++;
    } else {
      format_count_++;
      rep_count_ = 0;
    }
  }

  /**
   * @brief Registers the run-time of the last step the tuner completed.
   *
   * @param runtime Run-time of the last step.
   */
  void register_run(double runtime) {
    timings_(format_count(), repetition_count()) = runtime;
  }

  /**
   * @brief Checks if the tuner has completed the tuning process.
   *
   * @return true Tuning is completed.
   * @return false Tuning still not completed.
   */
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

  /**
   * @brief Resets the state of the tuner to the initial state.
   *
   */
  void reset() {
    format_id_    = INVALID_FORMAT_STATE;
    format_count_ = 0;
    rep_count_    = 0;
    verbose_      = false;
    timings_.assign(nformats_, rep_limit_, 0);
    max_timings_.assign(nformats_, 0);
    avg_timings_.assign(nformats_, 0);
    min_timings_.assign(nformats_, 0);
  }

  /**
   * @brief Prints the state of the tuner. Note that this is only possible
   * during the first step of the tuner or after the tuning process is completed
   * (i.e \p finished() returns true), otherwise a run-time exception is thrown.
   *
   */
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
      cout << endl;
      cout << "Tuner statistics:" << endl;
      cout << "-----------------" << endl;
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

  /**
   * @brief Provides a two-dimensional vector that contains the timings obtained
   * at each step of the tuner and for each format.
   *
   * @return vec2d& A two-dimensional vector containing timings of each format.
   */
  vec2d& timings() { return timings_; }

  /**
   * @brief Provides a one-dimensional vector that contains the maximum timings
   * for each format.
   *
   * @return vec& A one-dimensional vector containing max timings of each
   * format.
   */
  vec& max_timings() { return max_timings_; }

  /**
   * @brief Provides a one-dimensional vector that contains the minimum timings
   * for each format.
   *
   * @return vec& A one-dimensional vector containing minimum timings of each
   * format.
   */
  vec& min_timings() { return min_timings_; }

  /**
   * @brief Provides a one-dimensional vector that contains the average timings
   * for each format.
   *
   * @return vec& A one-dimensional vector containing average timings of each
   * format.
   */
  vec& avg_timings() { return avg_timings_; }

  /**
   * @brief Provides the current format index the tuner optimizes for.
   *
   * @return size_t Current format index.
   */
  size_t format_count() const { return format_count_; }

  /**
   * @brief Provides the total number of formats the tuner optimizes for.
   *
   * @return size_t Total number of formats.
   */
  size_t nformats() const { return nformats_; }

  /**
   * @brief Provides the index of the optimum format selected by the tuner. Note
   * that until the tuner finishes the tuning process, the format ID is set to
   * \p INVALID_FORMAT_STATE.
   *
   * @return size_t Optimum format index.
   */
  size_t format_id() const { return format_id_; }

  /**
   * @brief Provides the current repetition count the tuner runs for.
   *
   * @return size_t Current repetition count.
   */
  size_t repetition_count() const { return rep_count_; }

  /**
   * @brief Provides the total number of repetitions the tuner runs for.
   *
   * @return size_t Total number of repetitions.
   */
  size_t repetition_limit() const { return rep_limit_; }

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

  /**
   * @brief Set the limit of repetitions the tuner will carry out.
   *
   * @param rep_limit Number of repetitions.
   */
  void set_repetition_limit(int rep_limit = 10) { rep_limit_ = rep_limit; }

  /*! \cond */
 private:
  void compute_best_format_id_() {
    // best format the one with the minimum average
    double mint = avg_timings_(0);
    format_id_  = 0;
    for (size_t i = 0; i < nformats(); i++) {
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
  /*! \endcond */
};

/*! \}  // end of tuners group
 */
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_RUNFIRSTTUNER_HPP
