#include <immintrin.h>
#include <emmintrin.h>
#include <stdlib.h>
#include <stdio.h>
// Timeit
#include <time.h>
#include <asm/msr.h>

#define MAX 1024
#define MAX_ITER 10000000

/** #define USE_AVX2 */
/** #define USE_SSE2 */
#define USE_ALU
/** #define USE_ZEROUPPER */

void rdtscl(unsigned long long *ll)
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    *ll = ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}


int main() {
  char* a = aligned_alloc(32, sizeof(char)*MAX);
  char* b = aligned_alloc(32, sizeof(char)*MAX);

  __m256i out;

  unsigned long long start, stop;
  start = clock();
  /** rdtscl(&start); */

  for (int iter = 0; iter < MAX_ITER; iter++) {
    for (int i = 0; i < MAX / 32; i++) {
#ifdef USE_AVX2
      __asm__(
          "vmovdqa (%%rax), %%ymm0;"
          "vmovdqa (%%rbx), %%ymm1;"
          "vpcmpeqb %%ymm0, %%ymm1, %0;"
          : "=Yt"(out)
          : "a"(a+i*32), "b"(b+i*32)
          : "%ymm0", "%ymm1"
          );
#endif

#ifdef USE_ZEROUPPER
      __asm__ (
          "vzeroupper;"
        );
#endif

#ifdef USE_SSE2
      __m128i out1, out2;
      __asm__(
          "movdqu (%%rax), %%xmm0;"
          "movdqu (%%rbx), %%xmm1;"
          "pcmpeqb %%xmm0, %%xmm1;"
          "movdqu %%xmm0, %0;"
          "add $32, %%rax;"
          "add $32, %%rbx;"
          "movdqu (%%rax), %%xmm0;"
          "movdqu (%%rbx), %%xmm1;"
          "pcmpeqb %%xmm0, %%xmm1;"
          "movdqu %%xmm0, %1;"
          : "=x"(out1), "=x"(out2)
          : "a"(a+i*32), "b"(b+i*32)
          : "%xmm0", "%xmm1"
          );
#endif

#ifdef USE_ALU
      for (int j=i; j<i*32; j++) {
        int ret = a[j] == b[j];
      }
#endif

      __asm__ (
          "nop;"
      );
    }
  }

  stop = clock();
  /** rdtscl(&stop); */

  unsigned long long elaspe = stop - start;

  printf("Time Taken: %llu\n", elaspe);
  printf("Average Execution: %lf\n", elaspe * 1.0 / MAX_ITER);


  free(a);
  free(b);
  return 0;
}
