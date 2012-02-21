void fw(int** const in, int length, int** out) {

    out = in;

    for(int i = 0; i < length; i++)
        for(int j = 0; j < length; j++)
            for(int k = 0; k < length; k++)
                if (out[i][k]+out[k][j] < out[i][j])
                    out[i][j] = out[i][k]+out[k][j];
}
