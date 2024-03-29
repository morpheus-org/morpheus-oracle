/**
 * Morpheus_Oracle.hpp
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

#ifndef MORPHEUS_ORACLE_HPP
#define MORPHEUS_ORACLE_HPP

// Tuners
#include <MorpheusOracle_RunFirstTuner.hpp>
#include <MorpheusOracle_MLTuner.hpp>
#include <MorpheusOracle_DecisionTreeTuner.hpp>
#include <MorpheusOracle_RandomForestTuner.hpp>

// Operations
#include <MorpheusOracle_Tune.hpp>

// Functors
#include <MorpheusOracle_RunFirstFunctor.hpp>
#include <MorpheusOracle_MLFunctor.hpp>

// Utils
#include <MorpheusOracle_Macros.hpp>
#include <MorpheusOracle_TypeTraits.hpp>

// IO
#include <MorpheusOracle_Loader.hpp>

#endif  // MORPHEUS_ORACLE_HPP