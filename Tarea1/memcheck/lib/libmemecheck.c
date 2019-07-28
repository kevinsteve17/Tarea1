/**
 * Tarea # 1
 * Sistemas Empotrados de Alto Desepeno
 * Integrantes: 
 *            - Fernando Paris
 *            - Esteban Rivera
 *            - Kevin Viquez
 * 
 * Malloc and Free hook for heap trace.
 * REFERENCE (Based on): https://github.com/tharina/heap-tracer
 */

#define _GNU_SOURCE

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET "\x1b[0m"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <malloc.h>
#include <inttypes.h>

int reentrancy_guard;
int mallocCount = 0;
int freeCount = 0;

void* (*real_malloc)(size_t size); 
void* malloc(size_t size) 
{
    real_malloc = dlsym(RTLD_NEXT, "malloc");
    void* chunk = real_malloc(size);
    size_t s = malloc_usable_size(chunk);
    mallocCount++;

    return chunk;
}

void (*real_free)(void* ptr);
void free(void* ptr) 
{
    real_free = dlsym(RTLD_NEXT, "free");
    size_t s = malloc_usable_size(ptr);
    freeCount++;
    return real_free(ptr);
}

void __attribute__((destructor))
destructorLib()
{
    printf( BLUE    "\n ********** Analysis Finished! ********** \n");
    printf( GREEN   "\nMemory allocations: %d\n", mallocCount);
    printf( YELLOW  "\nMemory Free: %d\n", freeCount);
    printf( RED     "\nTotal memory leaks found: %d\n" RESET, mallocCount - freeCount);

}
