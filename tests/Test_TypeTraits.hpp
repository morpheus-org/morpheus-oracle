/**
 * Test_TypeTraits.hpp
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

#ifndef TEST_ORACLE_TEST_TYPETRAITS_HPP
#define TEST_ORACLE_TEST_TYPETRAITS_HPP

#include <Morpheus_Oracle.hpp>
#include <gtest/gtest.h>

TEST(TypeTraits, IsRunFirstTuner) {
  int res = Morpheus::Oracle::is_run_first_tuner<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_run_first_tuner<A>::value;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_run_first_tuner<
      Morpheus::Oracle::RunFirstTuner>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_run_first_tuner_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_run_first_tuner_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_run_first_tuner_v<Morpheus::Oracle::RunFirstTuner>;
  EXPECT_EQ(res, 1);
}

TEST(TypeTraits, IsDecisionTree) {
  int res = Morpheus::Oracle::is_decision_tree<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_decision_tree<A>::value;
  EXPECT_EQ(res, 0);

  res =
      Morpheus::Oracle::is_decision_tree<Morpheus::Oracle::DecisionTree>::value;
  EXPECT_EQ(res, 1);

  class NewTree : public Morpheus::Oracle::DecisionTree {};
  res = Morpheus::Oracle::is_decision_tree<NewTree>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_decision_tree_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_decision_tree_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_decision_tree_v<Morpheus::Oracle::DecisionTree>;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_decision_tree_v<NewTree>;
  EXPECT_EQ(res, 1);
}

TEST(TypeTraits, IsRandomForest) {
  int res = Morpheus::Oracle::is_random_forest<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_random_forest<A>::value;
  EXPECT_EQ(res, 0);

  res =
      Morpheus::Oracle::is_random_forest<Morpheus::Oracle::RandomForest>::value;
  EXPECT_EQ(res, 1);

  class NewForest : public Morpheus::Oracle::RandomForest {};
  res = Morpheus::Oracle::is_random_forest<NewForest>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_random_forest_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_random_forest_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_random_forest_v<Morpheus::Oracle::RandomForest>;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_random_forest_v<NewForest>;
  EXPECT_EQ(res, 1);
}

TEST(TypeTraits, IsDecisionTreeTuner) {
  int res = Morpheus::Oracle::is_decision_tree_tuner<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_decision_tree_tuner<A>::value;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_decision_tree_tuner<
      Morpheus::Oracle::DecisionTreeTuner>::value;
  EXPECT_EQ(res, 1);

  class NewTreeTuner : public Morpheus::Oracle::DecisionTreeTuner {};
  res = Morpheus::Oracle::is_decision_tree_tuner<NewTreeTuner>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_decision_tree_tuner_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_decision_tree_tuner_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_decision_tree_tuner_v<
      Morpheus::Oracle::DecisionTreeTuner>;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_decision_tree_tuner_v<NewTreeTuner>;
  EXPECT_EQ(res, 1);
}

TEST(TypeTraits, IsRandomForestTuner) {
  int res = Morpheus::Oracle::is_random_forest_tuner<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_random_forest_tuner<A>::value;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_random_forest_tuner<
      Morpheus::Oracle::RandomForestTuner>::value;
  EXPECT_EQ(res, 1);

  class NewForestTuner : public Morpheus::Oracle::RandomForestTuner {};
  res = Morpheus::Oracle::is_random_forest_tuner<NewForestTuner>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_random_forest_tuner_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_random_forest_tuner_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_random_forest_tuner_v<
      Morpheus::Oracle::RandomForestTuner>;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_random_forest_tuner_v<NewForestTuner>;
  EXPECT_EQ(res, 1);
}

TEST(TypeTraits, IsMLTuner) {
  int res = Morpheus::Oracle::is_ml_tuner<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_ml_tuner<A>::value;
  EXPECT_EQ(res, 0);

  res =
      Morpheus::Oracle::is_ml_tuner<Morpheus::Oracle::DecisionTreeTuner>::value;
  EXPECT_EQ(res, 1);

  res =
      Morpheus::Oracle::is_ml_tuner<Morpheus::Oracle::RandomForestTuner>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_ml_tuner_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_ml_tuner_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_ml_tuner_v<Morpheus::Oracle::DecisionTreeTuner>;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_ml_tuner_v<Morpheus::Oracle::RandomForestTuner>;
  EXPECT_EQ(res, 1);
}

TEST(TypeTraits, IsMLFunctor) {
  int res = Morpheus::Oracle::is_ml_functor<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_ml_functor<A>::value;
  EXPECT_EQ(res, 0);

  struct NewMLFunctor : public Morpheus::Oracle::MLFunctorBase<NewMLFunctor> {
    NewMLFunctor() {}

    void extract_features(const std::vector<double>&) { return; }

    std::vector<double>& features() { return _features; }

    void inference(Morpheus::Oracle::RandomForestTuner&) { return; }

   private:
    std::vector<double> _features;
  };

  res = Morpheus::Oracle::is_ml_functor<NewMLFunctor>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_ml_functor_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_ml_functor_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_ml_functor_v<NewMLFunctor>;
  EXPECT_EQ(res, 1);
}

TEST(TypeTraits, IsRunFirstFunctor) {
  int res = Morpheus::Oracle::is_runfirst_functor<int>::value;
  EXPECT_EQ(res, 0);

  struct A {};
  res = Morpheus::Oracle::is_runfirst_functor<A>::value;
  EXPECT_EQ(res, 0);

  struct NewRunFirstFunctor
      : public Morpheus::Oracle::RunFirstFunctorBase<NewRunFirstFunctor> {
    auto clone(const std::vector<double>& data) { return data; }

    auto clone_host(const std::vector<double>& data) { return data; }

    bool state_transition(const Morpheus::Oracle::RunFirstTuner&,
                          std::vector<double>& dev, std::vector<double>&,
                          int&) {
      return true;
    }

    void run(const std::vector<double>&) { return; }

    double postprocess_runtime(double runtime) { return runtime; }
  };

  res = Morpheus::Oracle::is_runfirst_functor<NewRunFirstFunctor>::value;
  EXPECT_EQ(res, 1);

  res = Morpheus::Oracle::is_runfirst_functor_v<int>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_runfirst_functor_v<A>;
  EXPECT_EQ(res, 0);

  res = Morpheus::Oracle::is_runfirst_functor_v<NewRunFirstFunctor>;
  EXPECT_EQ(res, 1);
}

#endif  // TEST_ORACLE_TEST_TYPETRAITS_HPP