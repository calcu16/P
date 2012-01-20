/*
 * Example P program showing the basic
 * operation of an array
 *
 * AC
 */
void func(int* p)
{
    *p = 0;
}
int main(void)
{
    int *p;
    *p = 1;
    func(p);
    printf("%d\n", *p);
    return 0;
}

/*
 * Input : 
 * Output : 0
 */
