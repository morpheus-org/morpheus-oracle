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

#include <vector>
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
  using vec2d = std::vector<double>;
  /*! A one-dimensional vector of doubles */
  using vec = std::vector<double>;

  /*! Value specifying the state of the tuner when the optimum state has
   * not been yet selected*/
  static const int INVALID_STATE = -1;

  /**
   * @brief Construct a new RunFirstTuner object
   *
   * @param rep_limit Number of repetitions to run the tuner for.
   * @param verbose  Whether to print verbose messages.
   */
  RunFirstTuner(const size_t nstates, const size_t rep_limit = 10,
                const bool verbose = false)
      : timings_(nstates * rep_limit, 0),
        max_timings_(nstates, 0),
        avg_timings_(nstates, 0),
        min_timings_(nstates, 0),
        state_id_(INVALID_STATE),
        state_count_(0),
        nstates_(nstates),
        rep_limit_(rep_limit),
        rep_count_(0),
        verbose_(verbose) {
    if (verbose) {
      std::cout << "RunFirstTuner :: Tuner will run for " << nstates_
                << " states.\n";
      std::cout << "                 Each state tunes for "
                << repetition_limit() << " repetitions.\n";

      std::cout << std::setw(10) << "State ID";
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
      state_count_++;
      rep_count_ = 0;
    }
  }

  /**
   * @brief Registers the run-time of the last step the tuner completed.
   *
   * @param runtime Run-time of the last step.
   */
  void register_run(double runtime) {
    timings_[state_count() * repetition_limit() + repetition_count()] = runtime;
  }

  /**
   * @brief Checks if the tuner has completed the tuning process.
   *
   * @return true Tuning is completed.
   * @return false Tuning still not completed.
   */
  bool finished() {
    if (verbose_) {
      std::cout << std::setw(10) << state_count();
      std::cout << std::setw(3) << "|";
      std::cout << std::setw(12) << repetition_count();
      std::cout << std::endl;
    }

    bool completed = state_count() >= nstates() ? true : false;

    if (completed) {
      compute_max_timings_();
      compute_avg_timings_();
      compute_min_timings_();
      compute_best_state_id_();
    }
    return completed;
  }

  /**
   * @brief Resets the state of the tuner to the initial state.
   *
   */
  void reset() {
    state_id_    = INVALID_STATE;
    state_count_ = 0;
    rep_count_   = 0;
    verbose_     = false;
    timings_.assign(timings_.size(), 0);
    max_timings_.assign(nstates_, -std::numeric_limits<double>::max());
    avg_timings_.assign(nstates_, -std::numeric_limits<double>::max());
    min_timings_.assign(nstates_, -std::numeric_limits<double>::max());
  }

  /**
   * @brief Prints the state of the tuner. Note that this is only possible
   * during the first step of the tuner or after the tuning process is completed
   * (i.e \p finished() returns true), otherwise a run-time exception is thrown.
   *
   */
  void print() {
    if ((repetition_count() == 0) && (state_count() == 0)) {
      std::cout << "Run-first Tuner configured with repetition limit "
                << repetition_limit() << std::endl;
      return;
    }

    if (state_count() >= nstates()) {
      using namespace std;
      cout << "Tuner executed " << repetition_limit() << " repetitions and "
           << "optimized for " << nstates() << " states!" << endl;
      cout << endl;
      cout << "Tuner statistics:" << endl;
      cout << "-----------------" << endl;
      cout << setw(10) << "State ID\t";
      cout << setw(10) << "tmin\t";
      cout << setw(10) << "tmax\t";
      cout << setw(10) << "tavg\t" << endl;

      for (int i = 0; i < nstates_; i++) {
        cout << setw(10) << i;
        cout << "\t" << setw(10) << setprecision(7) << min_timings_[i];
        cout << "\t" << setw(10) << setprecision(7) << max_timings_[i];
        cout << "\t" << setw(10) << setprecision(7) << avg_timings_[i];
        cout << endl;
      }

      cout << "Optimum State ID: " << state_id() << endl;
    } else {
      throw std::runtime_error(
          "Tuner is in inconsistent state. Requesting to print the summary can "
          "be done once tuning finishes.");
    }
  }

  /**
   * @brief Provides a two-dimensional vector that contains the timings obtained
   * at each step of the tuner and for each state.
   *
   * @return vec2d& A two-dimensional vector containing timings of each state.
   */
  vec2d& timings() { return timings_; }

  /**
   * @brief Provides a one-dimensional vector that contains the maximum timings
   * for each state.
   *
   * @return vec& A one-dimensional vector containing max timings of each
   * state.
   */
  vec& max_timings() { return max_timings_; }

  /**
   * @brief Provides a one-dimensional vector that contains the minimum timings
   * for each state.
   *
   * @return vec& A one-dimensional vector containing minimum timings of each
   * state.
   */
  vec& min_timings() { return min_timings_; }

  /**
   * @brief Provides a one-dimensional vector that contains the average timings
   * for each state.
   *
   * @return vec& A one-dimensional vector containing average timings of each
   * state.
   */
  vec& avg_timings() { return avg_timings_; }

  /**
   * @brief Provides the current state index the tuner optimizes for.
   *
   * @return int Current state index.
   */
  int state_count() const { return state_count_; }

  /**
   * @brief Provides the total number of states the tuner optimizes for.
   *
   * @return int Total number of states.
   */
  int nstates() const { return nstates_; }

  /**
   * @brief Provides the index of the optimum state selected by the tuner. Note
   * that until the tuner finishes the tuning process, the state ID is set to
   * \p INVALID_STATE.
   *
   * @return int Optimum state index.
   */
  int state_id() const { return state_id_; }

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
  void compute_best_state_id_() {
    // best state the one with the minimum average
    double mint = avg_timings_[0];
    state_id_   = 0;
    for (int i = 0; i < nstates(); i++) {
      if (avg_timings_[i] < mint) {
        mint      = avg_timings_[i];
        state_id_ = i;
      }
    }
  }

  void compute_max_timings_() {
    for (int i = 0; i < nstates_; i++) {
      double maxt = std::numeric_limits<double>::min();
      for (size_t j = 0; j < rep_limit_; j++) {
        if (timings_[i * rep_limit_ + j] > maxt) {
          max_timings_[i] = timings_[i * rep_limit_ + j];
          maxt            = timings_[i * rep_limit_ + j];
        }
      }
    }
  }

  void compute_avg_timings_() {
    for (int i = 0; i < nstates_; i++) {
      double sumt = 0.0;
      for (size_t j = 0; j < rep_limit_; j++) {
        sumt += timings_[i * rep_limit_ + j];
      }
      avg_timings_[i] = sumt / (double)rep_limit_;
    }
  }

  void compute_min_timings_() {
    for (int i = 0; i < nstates_; i++) {
      double mint = std::numeric_limits<double>::max();
      for (size_t j = 0; j < rep_limit_; j++) {
        if (timings_[i * rep_limit_ + j] < mint) {
          min_timings_[i] = timings_[i * rep_limit_ + j];
          mint            = timings_[i * rep_limit_ + j];
        }
      }
    }
  }

  vec2d timings_;
  vec max_timings_;
  vec avg_timings_;
  vec min_timings_;
  int state_id_;
  int state_count_;
  int nstates_;
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
