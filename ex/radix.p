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
