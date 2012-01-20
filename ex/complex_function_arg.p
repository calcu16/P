/*
 * There is some question as to the
 * behavior of an assignment
 * when the right hand side is a
 * function argument.
 *
 * There are at least 4 choices,
 *  0) treat all arguments as references
 *    this means a raw int can be treated as an
 *    extra return value, and thus requires more
 *    const declarations.
 *    This would remove the need for references in P++.
 *    Also awkward when passing in a &variable.
 *  1) carry over return-references in assignments
 *    this weakens the no-aliasing requirement
 *    and can make things more difficult for the compiler
 *    to reason about
 *  2) don't carry over return-references
 *    this make a=a; not necessarily a noop, and makes things
 *    like ++a relinquish the return-reference
 *  3) only carry over return-references if the variable is the same
 *    this seems like the most confusing for a human, but maybe it
 *    makes sense.
 *    This would only allow reindexing of an variable, this
 *    would be more complicated in P++
 *
 * I'm leaning towards 0, with 3 being a close second
 * AC
 */
 
void func(int i, int* a, *p)
{
    int *b;
    b = a;
    ++a;
    b[0] = -1;
    a[0] = -2;
    a = a;
    a[1] = -3;
    i = 10;
    *p = -4;
    p = a;
    *p = -5;
}
 
 int main(void)
 {
    int i, j, a[5];
    for(i = 0; i < 5; i++)
        a[i] = i;
    j = 0;
    func(i, ++a, &j);
    printf("%d %d %d %d %d %d\n", i, a[-1], a[0], a[1], a[2], j);
 }
 
 /*
  * Input :
  * Output 0 : 10 1 -2 -5 4 -4/0/Undefined
  * Output 1 : 5 0 -1 -2 -3 -4
  * Output 2 : 5 0 1 2 3 -4
  * Output 3 : 5 0 1 -2 -3 -4
  */
 