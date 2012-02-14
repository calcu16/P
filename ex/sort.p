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
