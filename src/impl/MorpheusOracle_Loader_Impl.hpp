/**
 * MorpheusOracle_Loader_Impl.hpp
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

#ifndef MORPHEUSORACLE_LOADER_IMPL_HPP
#define MORPHEUSORACLE_LOADER_IMPL_HPP

#include <fwd/MorpheusOracle_Fwd_DecisionTree.hpp>
#include <Morpheus_Core.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Morpheus {
namespace Oracle {
namespace Impl {

void tokenize(std::vector<std::string>& tokens, const std::string& str,
              const std::string& delimiters = "\n\r\t ") {
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos) {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}

template <typename Stream>
void skip_comment(Stream& input, const std::string delimiter = "#") {
  std::string line;
  std::streampos oldpos;
  do {
    oldpos = input.tellg();
    std::getline(input, line);
  } while (line[0] == delimiter[0]);

  input.seekg(oldpos);
}

template <typename Stream>
void load_array(std::vector<std::string>& vec, Stream& input,
                bool skip_comments = true, const std::string delimiter = "#") {
  using size_type = typename std::vector<std::string>::size_type;

  if (skip_comments) {
    skip_comment(input, delimiter);
  }

  std::string line;
  std::getline(input, line);

  std::vector<std::string> tokens;
  tokenize(tokens, line);

  if (tokens.size() != vec.size()) {
    throw Morpheus::IOException("Entries (" + std::to_string(tokens.size()) +
                                ") exceed vector size (" +
                                std::to_string(vec.size()) + ").");
  }

  for (size_type i = 0; i < tokens.size(); i++) {
    std::istringstream(tokens[i]) >> vec[i];
  }
}

template <typename Vector, typename Stream>
void load_array(Vector& vec, Stream& input, bool skip_comments = true,
                const std::string delimiter = "#",
                typename std::enable_if_t<
                    Morpheus::is_dense_vector_format_container_v<Vector> &&
                    Morpheus::has_host_memory_space_v<Vector>>* = nullptr) {
  using size_type = typename Vector::size_type;

  if (skip_comments) {
    skip_comment(input, delimiter);
  }

  std::string line;
  std::getline(input, line);

  std::vector<std::string> tokens;
  tokenize(tokens, line);

  if (tokens.size() != vec.size()) {
    throw Morpheus::IOException("Entries (" + std::to_string(tokens.size()) +
                                ") exceed vector size (" +
                                std::to_string(vec.size()) + ").");
  }

  for (size_type i = 0; i < tokens.size(); i++) {
    std::istringstream(tokens[i]) >> vec[i];
  }
}

template <typename Matrix, typename Stream>
void load_array(Matrix& mat, Stream& input, bool skip_comments = true,
                const std::string delimiter = "#",
                typename std::enable_if_t<
                    Morpheus::is_dense_matrix_format_container_v<Matrix> &&
                    Morpheus::has_host_memory_space_v<Matrix>>* = nullptr) {
  using size_type = typename Matrix::size_type;

  if (skip_comments) {
    skip_comment(input, delimiter);
  }

  size_type row = 0;
  while (row < mat.nrows() && !input.eof()) {
    std::string line;
    std::getline(input, line);

    std::vector<std::string> tokens;
    tokenize(tokens, line);

    if (tokens.size() != mat.ncols()) {
      throw Morpheus::IOException("Entries (" + std::to_string(tokens.size()) +
                                  ") exceed vector size (" +
                                  std::to_string(mat.ncols()) + ").");
    }

    for (size_type i = 0; i < tokens.size(); i++) {
      std::istringstream(tokens[i]) >> mat(row, i);
    }
    row++;
  }
}

}  // namespace Impl
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_LOADER_IMPL_HPP