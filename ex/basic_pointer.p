/*
 * Example P program showing the basic operation
 * of a pointer
 *
 * AC
 */

int main(void)
{
    int i, *p;
    i = 5;
    p = &i;
    *p = 6;
    print("%d %d\n", i, *p);
    return 0;
}

/*
 * Input : 
 * Output : 5 6
 */
