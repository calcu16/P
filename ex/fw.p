void fw(const int** in, const int length, int** out)
{
    int i,j,k;
    out = in;
    for(j = 0; j < length; j = j + 1)
        for(i = 0; i < length; i = i + 1)
            for(k = 0; k < length; k = k + 1)
                if(out[i][j] + out[j][k] < out[i][k])
                    out[i][k] = out[i][j]+out[j][k];
}

