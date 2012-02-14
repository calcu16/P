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
 * This should run as the following C++ program
 */
template<typename T>
T* copy(T* in)
{
    return new T(copy(*in));
}
template<typename T>
T copy(T in)
{
    return in;
}

int main(void)
{
    int i, *p;
    i = 5;
    p = copy(&i);
    *p = 6;
    printf("%d %d\n", i, *p);
    return 0;
}
