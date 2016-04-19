#include <stdio.h>
#include <assert.h>

int is_prime(int x)
{
    int i;
    assert(x >= 2);
    for(i=2;i<x;i++) {
        if (x % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main()
{
    int i;
    for(i=2;i<1000000;i++) {
        if (is_prime(i)) {
            printf("[%d]\n", i);
        }
    }
    return 0;
}
