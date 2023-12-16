/**
 * MorpheusOracle_Loader_Utils.hpp
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

#ifndef MORPHEUSORACLE_IMPL_LOADER_UTILS_HPP
#define MORPHEUSORACLE_IMPL_LOADER_UTILS_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

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
  Impl::tokenize(tokens, line);

  if (tokens.size() != vec.size()) {
    throw std::runtime_error("Entries (" + std::to_string(tokens.size()) +
                             ") exceed vector size (" +
                             std::to_string(vec.size()) + ").");
  }

  for (size_type i = 0; i < tokens.size(); i++) {
    std::istringstream(tokens[i]) >> vec[i];
  }
}

template <typename Vector, typename Stream>
void load_array(Vector& vec, Stream& input, bool skip_comments = true,
                const std::string delimiter = "#") {
  using size_type = typename Vector::size_type;

  if (skip_comments) {
    skip_comment(input, delimiter);
  }

  std::string line;
  std::getline(input, line);

  std::vector<std::string> tokens;
  Impl::tokenize(tokens, line);

  if (tokens.size() != vec.size()) {
    throw std::runtime_error("Entries (" + std::to_string(tokens.size()) +
                             ") exceed vector size (" +
                             std::to_string(vec.size()) + ").");
  }

  for (size_type i = 0; i < tokens.size(); i++) {
    std::istringstream(tokens[i]) >> vec[i];
  }
}

template <typename Matrix, typename Stream>
void load_2d_array(Matrix& mat, size_t dim1, size_t dim2, Stream& input,
                   bool skip_comments          = true,
                   const std::string delimiter = "#") {
  using size_type = typename Matrix::size_type;

  if (skip_comments) {
    skip_comment(input, delimiter);
  }

  size_type row = 0;
  while (row < dim1 && !input.eof()) {
    std::string line;
    std::getline(input, line);

    std::vector<std::string> tokens;
    Impl::tokenize(tokens, line);

    if (tokens.size() != dim2) {
      throw std::runtime_error("Entries (" + std::to_string(tokens.size()) +
                               ") exceed vector size (" + std::to_string(dim2) +
                               ").");
    }

    for (size_type i = 0; i < tokens.size(); i++) {
      std::istringstream(tokens[i]) >> mat[row * dim2 + i];
    }
    row++;
  }
}

}  // namespace Impl
}  // namespace Oracle
}  // namespace Morpheus

#endif  // MORPHEUSORACLE_IMPL_LOADER_UTILS_HPP