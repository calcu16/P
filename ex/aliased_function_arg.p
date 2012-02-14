/*
 * Example P program showing
 * the operation of aliased arguments
 *
 * AC
 */

int func(int * const p, int * const q, int const * r)
{
    *p = 6;
    *q = 7;
    return (*r) * (*r);
}

int main(void)
{
    int i, a[2];
    for(i = 0; i < 2; ++i)
        a[i] = 0;
    i = randint(0, 1);
    i = func(a, a + i, a);
    printf("%d %d %d\n", a[0], a[1], i);
    return 0;
}

/*
 * This could be compiled correctly to either of the following
 * two c programs.
 */

int* copy(int* a, int index, int length)
{
    out = new int[length];
    for(int i = 0; i < length; ++i)
        out[i] = a[i - index];
    return out + index;
}

int func(int *p, int *q, int *r)
{
    *p = 6;
    *q = 7;
    return (*r) * (*r);
}

int main(void)
{
    int i, a[2];
    for(i = 0; i < 2; ++i)
        a[i] = 0;
    i = randint(0, 1);
    i = func(a, a + i, a)
    printf("%d %d %d\n", a[0], a[1], i);
    return 0;
}

/* or */

int* copy(int* a, int index, int length)
{
    out = new int[length];
    for(int i = 0; i < length; ++i)
        out[i] = a[i - index];
    return out + index;
}

int func(int *p, int *q, int *r)
{
    *q = 7;
    *p = 6;
    return (*r) * (*r);
}

int main(void)
{
    int i, a[2];
    for(i = 0; i < 2; ++i)
        a[i] = 0;
    i = randint(0, 1);
    i = func(a, a + i, a)
    printf("%d %d %d\n", a[0], a[1], i);
    return 0;
}
