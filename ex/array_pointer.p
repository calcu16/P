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
 * Input : 
 * Output : 0, 100, -1, 81
 */
