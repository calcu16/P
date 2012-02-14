/*
 * Example P program showing the basic
 * operation of an array
 *
 * AC
 */

int main(void)
{
    int a[10], i, *p;
    for(i = 0; i < 10; ++i)
        a[i] = i * i;
    p = a + 5;
    --a;
    p[-5] = 100;
    *(a + 10) = -1
    printf("%d %d %d %d\n", a[1], p[-5], a[10], p[4]);
    return 0;
}

/*
 * This should run as the following C++ program
 */
int* copy(int* a, int index, int length)
{
    out = new int[length];
    for(int i = 0; i < length; ++i)
        out[i] = a[i - index];
    return out + index;
}

int main(void)
{
    int a[10], i, *p;
    for(i = 0; i < 1-; ++i)
        a[i] = i * i;
    p = copy(a, 0, 10) + 5;
    --a;
    p[-5] = 100;
    a[10] = -1;
    printf("%d %d %d %d\n", a[1], p[-5], a[10], p[4]);
    return 0;
}
