#include <stdio.h>

int main (void)
{
    int MASK = 1 << 5;
    char kak = 0;
    kak |= MASK;
    printf ("kak = %d\n", kak);
    return 0;
}
