#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 11
 
bool is_sorted(int *a, int n)
{
  while ( --n >= 1 ) {
    if ( a[n] < a[n-1] ) return false;
  }
  return true;
}
 
void shuffle(int *a, int n)
{
  int i, t, r;
  for(i=0; i < n; i++) {
    t = a[i];
    r = rand() % n;
    a[i] = a[r];
    a[r] = t;
  }
}
 
void bogosort(int *a, int n)
{
  while ( !is_sorted(a, n) ) shuffle(a, n);
}
 
int main()
{
  int numbers[] = { 1, 10, 9,  7, 3, 0 , 5, 32, 45, 78, 51};
  int i;
 
  bogosort(numbers, SIZE);
  for (i=0; i < SIZE; i++) printf("%d ", numbers[i]);
  printf("\n");

  return 0;
}