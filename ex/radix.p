/*
Copyright (c) 2012, Andrew Carter, Dietrich Lagenbach, Xanda Schofield
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/
// C code taken from:
// http://sourcecode4u.com/sortings/
//   11-write-a-c-program-to-implement-radix-sort

void radixsort(int * const a, int *out, int const n)
{
  int i, *b, m = 0, exp = 1;

  // Find maximum value while creating the new table  
  out = a;
  b = a;

  for (i = 0; i < n; i++)
  {
    if (a[i] > m)
      m = a[i];
  }

  // For each digit
  while (m / exp > 0)
  {
    // Create array of bucket counts
    int bucket[10] = 
    {
      0
    };

    // Count items in each bucket
    for (i = 0; i < n; i++)
      bucket[out[i] / exp % 10]++;

    // Find cumulative index for each bucket
    for (i = 1; i < 10; i++)
      bucket[i] += bucket[i - 1];

    // Populate new array with next digit sorted
    for (i = n - 1; i >= 0; i--)
      b[--bucket[out[i] / exp % 10]] = out[i];

    // Write the partially sorted version out
    for (i = 0; i < n; i++)
      out[i] = b[i];
    
    // Move on to the next exponent
    exp *= 10;
  }
}
