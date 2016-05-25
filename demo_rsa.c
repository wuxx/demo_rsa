#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define error() do {printf("error: [%s][%d]\n", __func__, __LINE__); exit(-1);} while(0)

typedef unsigned int u32;
typedef signed   int s32;

u32 rand_fd;

u32 get_random(u32 min, u32 max)
{
    u32 rand_num;

    read(rand_fd, &rand_num, 4);
    rand_num = rand_num % (max - min);

    rand_num += min;
    return rand_num;
}

u32 is_prime(u32 x)
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

u32 gcd(u32 x, u32 y)
{
    if (x > y) {
        if (x % y != 0) {
            return gcd(y, x%y);
        } else {
            return y;
        }
    } else  {
        return gcd(y, x);
    }
}

#if 0
int gcd_ext(int a, int b, int *x, int *y) 
{
    if(b==0)
    {
        *x = 1,*y = 0;
        return a;
    }
    else
    {
        int r = gcd_ext(b, a%b, x, y);
        int t = *x;
        *x = *y;
        *y = t - a/b * *y;
        return r;
    }
}
#endif


/* modular multiplicative inverse
   (d * e) mod phin = 1
*/
int mod_mi(int phin, int d)
{
    int e = 1;
    while (1) {
        if ((d * e) % phin == 1) {
            return e;
        }
        e++;
    }
}

/* a^b % n */
int mod_exp(int a,int b,int n)     
{     
    int ret=1;     
    int tmp=a;     
    while(b)     
    {     
        if(b & 0x1) {
            ret = ret * tmp % n;     
        }

        tmp = tmp * tmp % n;     
        b >>= 1;     
    }     
    return ret;     
}

u32 phi(u32 p, u32 q)
{
    return (p - 1)*(q - 1);
}

int prime_map[100];

int main()
{
    u32 i, j = 0;
    u32 p_index = 0, q_index = 0;
    u32 plaintext1, plaintext2, ciphertext;
    u32 phin;
    u32 p, q;
    u32 n;
    u32 e;
    u32 x, y;
    u32 d;
    
    if ((rand_fd = open("/dev/urandom", O_RDONLY)) == -1) {
        perror("open");
        exit(-1);
    }


    for(i=2;i<1000000;i++) {
        if (is_prime(i)) {
            printf("[%d]\n", i);
            prime_map[j++] = i;
            if (j == 100) {
                break;
            }
        }
    }

    /* 1. choose 2 diffent prime num p & q */
    do {
        p_index = get_random(0, 20);
        q_index = get_random(0, 20);
    } while(p_index == q_index);

    p = prime_map[p_index];
    q = prime_map[q_index];

    printf("1. p: %d; q: %d\n", p, q);

    /* 2. caculate p * q */
    n = p * q;  /* bit length of key */
    printf("2. n: %d\n", n);

    /* 3. caculate phin */
    phin = phi(p, q);
    printf("3. phin: %d \n", phin);

    /* 4. choose e */
    for(e=2;e<phin;e++) {
        if (gcd(phin, e) == 1) {    /* FIXME: random e */
            break;  
        }
    }

    if (e == phin) {
        error();
    }

    printf("4. e: %d \n", e);

    /* 5. caculate the d */
    /* (d * e) mod phin = 1 */
    d = mod_mi(phin, e);
    printf("5. d: %d \n", d);

    /* 6. public key (n, e) & private key (n, d) */
    printf("public  key: (n, e) (%d, %d)\n", n, e);
    printf("private key: (n, d) (%d, %d)\n", n, d);

    /* 7. encrypt */
    /* ( plaintext ^ e ) mod n = ciphertext */
    plaintext1 = get_random(1, 100);
    ciphertext = mod_exp(plaintext1, e, n);
    printf("7. encrpyt (%d) -> (%d) \n", plaintext1, ciphertext);
    
    /* 8. decrypt */
    /* ( ciphertext ^ d ) mod n = plaintext */
    plaintext2 = mod_exp(ciphertext, d, n);
    printf("8. decrypt (%d) -> (%d) \n", ciphertext, plaintext2);

    close(rand_fd);
    return 0;
}
