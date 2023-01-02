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
The following script is used to download all the real and binary square matrices 
from SuiteSparse collection. This results in approximately 2k matrices.
"""

import ssgetpy
import os

OUTDIR = os.path.dirname(os.path.abspath(__file__)) + "/" + "matrices"
MAX_MATICES = 10000

matrices = ssgetpy.matrix.MatrixList()
dtypes = ["Real", "Binary"]

for dtype in dtypes:
    result = ssgetpy.search(dtype=dtype.lower(), limit=MAX_MATICES)
    print(dtype + " matrices: " + str(len(result)))
    ctr = 0
    for matrix in result:
        if (matrix.rows != matrix.cols) or (matrix.id in [230, 231]):
            result.remove(matrix)
            ctr += 1
        else:
            matrices.append(matrix)

    print("\tRemoved " + str(ctr) + " non-square " + dtype + " matrices")

print("Total Matrices in the set: " + str(len(matrices)))

matrices.download(destpath=OUTDIR, extract=True)
