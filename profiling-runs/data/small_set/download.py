"""
 download.py
 
 EPCC, The University of Edinburgh
 
 (c) 2023 The University of Edinburgh
 
 Contributing Authors:
 Christodoulos Stylianou (c.stylianou@ed.ac.uk)
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 	http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
"""

"""
The following script is used to download 100 real and square matrices 
with a maximum of 100k Non-zeros from SuiteSparse collection.
"""

import ssgetpy
import random
import os

OUTDIR = os.path.dirname(os.path.abspath(__file__)) + "/" + "matrices"
MAX_MATICES = 10000
SET_MATRICES = 100
MAX_NNZ = 100000

# Set seed to randomize list in the same way
random.seed(0)

matrices = ssgetpy.matrix.MatrixList()
dtypes = ["Real"]

for dtype in dtypes:
    result = ssgetpy.search(
        dtype=dtype.lower(), nzbounds=(0, MAX_NNZ), limit=MAX_MATICES
    )
    print(dtype + " matrices: " + str(len(result)))

    # Shuffle the list
    random.shuffle(result)

    ctr = 0
    for matrix in result:
        if len(matrices) == SET_MATRICES:
            break

        if (matrix.rows != matrix.cols) or (matrix.id in [230, 231]):
            # Matrices 230 & 231 are Skew-Symmetric
            result.remove(matrix)
            ctr += 1
        else:
            matrices.append(matrix)

    print("\tRemoved " + str(ctr) + " non-square " + dtype + " matrices")

print("Total Matrices in the set: " + str(len(matrices)))

matrices.download(destpath=OUTDIR, extract=True)
