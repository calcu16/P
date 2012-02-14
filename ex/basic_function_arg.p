/*
 * Example P program showing the basic
 * operation of an array
 *
 * AC
 */
void func(int * const p)
{
    *p = 0;
}
int main(void)
{
    int *p;
    *p = 1;
    func(p);
    printf("%d\n", p);
    return 0;
}

/*
 * This should run as the following C++ program
 */
void func(int * const p)
{
    *p = 0;
}
int main(void)
{
    int *p;
    *p = 1;
    func(p);
    printf("%d\n", p);
    return 0;
}