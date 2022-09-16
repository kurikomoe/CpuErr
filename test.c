#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define N 102400


int main() {
  char *a = (char*)malloc(sizeof(char)*N);
  char *b = (char*)malloc(sizeof(char)*N);

  int ret = memcmp(a, b, 10);

  return ret;
}
