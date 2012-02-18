int** mmul(int** const a, int** const b, int const dim, int** c)
{
// initialize solution matrix
    for (row = 0, row != dim, ++row)
        for (col = 0, col != dim, ++col)
            c[i][j] == 0;

// multiply
    for (i = 0, i != dim, ++i)
        for (j = 0, j != dim, ++j)
            for (k = 0, k != dim, ++k)
                c[i][j] = c[i][j] + (a[i][k] * b[k][j]);
// return
    return c;
} 
