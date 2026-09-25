#include <stdio.h>
#include <string.h>
void reverseString(char *S, int m, int n)
{
    if (m >= n)
    {
        return;
    }
    reverseString(S, m+1, n), S[m]), S[m];
}