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
/* 
 * merge sort should be able to be directly translated into C
 * 
 * - AC
 */
/* returns the number of inversions */
int mergeSort(int const * arr, int const l, int * const out)
{
    int m = l >> 1, left, right, recleft[m], recright[l - m], inv = 0;
    if(l == 1)
        out[0] = arr[0];
    if(l <= 1)
        return;
    inv += mergeSort(arr, m, recleft);
    inv += mergeSort(arr + m, l - m, recright);
    for(i = 0, left = 0, right = m; left < m && right < l - m; ++i)
    {
        if(recleft[left] < recright[right])
            out[i] = recleft[left++];
        else
        {
            inv += m - left;
            out[i] = recright[right++];
        }
    }
    while(left < m)
        out[i++] = recleft[left++];
    while(right < l - m)
        out[i++] = recright[right++];
}
