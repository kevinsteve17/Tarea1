/**
 * Tarea # 1
 * Sistemas Empotrados de Alto Desepeno
 * Integrantes: 
 *            - Fernando Paris
 *            - Esteban Rivera
 *            - Kevin Viquez
 * 
 * Malloc and Free hook for heap trace.
 * REFERENCE: https://github.com/tharina/heap-tracer
 */

#define _GNU_SOURCE

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <malloc.h>
#include <inttypes.h>

#define BEGIN_HOOK \
        reentrancy_guard++;

#define HOOK \
    if (reentrancy_guard == 1)

#define END_HOOK \
        reentrancy_guard--;


int reentrancy_guard;
int mallocCount = 0;
int freeCount = 0;



void* (*real_malloc)(size_t size); 
void* malloc(size_t size) {
    
    BEGIN_HOOK

    real_malloc = dlsym(RTLD_NEXT, "malloc");
    void* chunk = real_malloc(size);

    HOOK {
        size_t s = malloc_usable_size(chunk);
        mallocCount++;
        //fprintf(stderr, BLUE "MALLOC(%4ld):  %p-%p (%4ld Bytes)\n" RESET, size, chunk, chunk+s, s);
    }

    END_HOOK

    return chunk;
}

void (*real_free)(void* ptr);
void free(void* ptr) {

    BEGIN_HOOK

    real_free = dlsym(RTLD_NEXT, "free");

    HOOK {
        size_t s = malloc_usable_size(ptr);
        freeCount++;
        //fprintf(stderr, GREEN "FREE:          %p-%p (%4ld Bytes)\n" RESET, ptr, ptr+s, s);
    }

    END_HOOK

    return real_free(ptr);
}

void __attribute__((destructor))
destructorLib()
{
    printf( BLUE "\n ********** Analysis Finished! ********** \n");
    printf( GREEN "\nMemory allocations: %d\n", mallocCount);
    printf( RED "\nMemory Free: %d\n", freeCount);
    printf( YELLOW "\nTotal memory leaks found: %d\n", mallocCount - freeCount);

}