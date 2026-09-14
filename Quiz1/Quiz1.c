#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int main()
{
  int a, b, i;
  int *c;
  int *array = (int *)malloc(SIZE * sizeof(int));

  a = 7;
  b = 3;

  array[0] = b;
  c = (array + b);
  *c = b;

  for (i = 0; i < SIZE / 2; i++)
  {
    array[i] = c[0] + a;
    c[i + 2] = i;
    printf("array[%d] = %d\n", i, array[i]);
  }
  printf("\n\n\n");

  for (i = SIZE / 2; i < SIZE; i++)
  {
    array[i] = *(c + i - b);
  }

  for (i = 0; i < SIZE; i++)
  {
    printf("array[%d] = %d\n", i, array[i]);
  }
  c = NULL;
  free(array);
  return 0;
}