#include <stdio.h>
#include <stdint.h>
#include <limits.h>

/* figure out compilation own bits
#ifdef __WORDSIZE
#define PROGRAM_BITS __WORDSIZE
#endif
#ifndef PROGRAM_BITS
#define PROGRAM_BITS (sizeof(int)*8)
#endif
*/

#ifdef JULIA
#define PROGRAM_BITS JULIA
#else
#error
#endif

// print out bits and ask Q 
void my_dlopen_libs_info(void) {
    printf("my_dlopen_libs_info: Running binaries are %i bits\n", PROGRAM_BITS);
    printf("my_dlopen_libs_info: Shoud I keep/check libs version here?\n");
}
