/**
 * MorpheusOracle_Utils.hpp
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

#ifndef MORPHEUSORACLE_UTILS_HPP
#define MORPHEUSORACLE_UTILS_HPP

#include <iomanip>
#include <iostream>

namespace Morpheus {
namespace Oracle {

template <typename Container, typename Stream>
void print_array(const Container& c, size_t size, Stream& stream) {
  stream << "[";
  for (size_t i = 0; i < size - 1; i++) {
    stream << c[i] << ", ";
  }
  stream << c[size - 1] << "]" << std::endl;
}

template <typename Container, typename Stream>
void print_matrix(const Container& c, size_t size1, size_t size2,
                  Stream& stream, size_t width = 0) {
  stream << "[";
  for (size_t i = 0; i < size1 - 1; i++) {
    stream << std::setw(i == 0 ? 0 : width) << "[";
    for (size_t j = 0; j < size2 - 1; j++) {
      stream << c[i * size2 + j] << ", ";
    }
    stream << c[i * size2 + (size2 - 1)] << "]," << std::endl;
  }
  stream << std::setw(width) << "[";
  for (size_t j = 0; j < size2 - 1; j++) {
    stream << c[(size1 - 1) * size2 + j] << ", ";
  }
  stream << c[(size1 - 1) * size2 + (size2 - 1)] << "]]" << std::endl;
}

}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_UTILS_HPP