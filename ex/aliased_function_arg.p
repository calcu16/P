/*
 * Example P program showing
 * the operation of aliased arguments
 *
 * AC
 */

int func(int * const p, int * const q)
{
    *p = 6;
    *q = 7;
    return (*p) * (*q);
}

int main(void)
{
    int i, a[2];
    for(i = 0; i < 2; ++i)
        a[i] = 0;
    i = getNextInt();
    i = func(a, a + i);
    printf("%d %d %d\n", a[0], a[1], i);
    return 0;
}

/*
 * Input : 0
 * Output : Undefined Undefined 42
 *
 * Input : 1
 * Output : 6 7 42
 */
 