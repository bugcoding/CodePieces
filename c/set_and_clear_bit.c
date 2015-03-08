#include <stdio.h>


unsigned int set_bits(unsigned int num, int n)
{
    num |= (1 << n);
    return num;
}

unsigned int clear_bits(unsigned int num, int n)
{
    num &= ~(1 << n);
    return num;
}



int main(int argc, char *argv[])
{
    printf("%d\n", set_bits(8, 1));
    printf("%d\n", clear_bits(8, 3));
    printf("%d\n", 10 << 1);
    return 0;
}
