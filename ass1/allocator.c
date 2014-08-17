//
//  COMP1927 Assignment 1 - Memory Suballocator
//  allocator.c ... implementation
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in August 2014
//  Copyright (c) 2012-2014 UNSW. All rights reserved.
//

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define HEADER_SIZE    sizeof(struct free_list_header)  
#define MAGIC_FREE     0xDEADBEEF
#define MAGIC_ALLOC    0xBEEFDEAD

typedef unsigned char byte;
typedef u_int32_t vlink_t;
typedef u_int32_t vsize_t;
typedef u_int32_t vaddr_t;

typedef struct free_list_header {
    u_int32_t magic;           // ought to contain MAGIC_FREE
    vsize_t size;              // # bytes in this block (including header)
    vlink_t next;              // memory[] index of next free block
    vlink_t prev;              // memory[] index of previous free block
} free_header_t;

// Global data

static byte *memory = NULL;   // pointer to start of suballocator memory
static vaddr_t free_list_ptr; // index in memory[] of first block in free list
static vsize_t memory_size;   // number of bytes malloc'd in memory[]

//Initialise the suballocator, and malloc memory for it
void sal_init(u_int32_t size) {

    //Check if already initialised
    if (memory != NULL) {
        return;
    }

                                                                                    /*      This will overshoot wildy for any moderately large value of size
                                                                                    //find size which is a power of two
                                                                                    u_int32_t n = 8;
                                                                                    while (n < size) {
                                                                                        n = n * 2;
                                                                                    }
                                                                                    */


    //round size to the nearest upper power of two, unless already power of two
    u_int32_t n = 1;
    if ((size != 0) && (size & (size-1)) == 0) {
        n = size;
        break;
    } else {
        while (n < size) {
        n = 2 * n;
    }

    //set global variables | initialise suballocator
    memory = malloc(n); 

    //check if malloc worked properly
    if (memory == NULL){
       fprintf(stderr, "sal_init: insufficient memory");
       abort();
    }
    free_list_ptr = 0;
    memory_size = n;

    //set first free list pointer
    free_header_t *T = (free_header_t *)memory;                                     //What's this line doing?
    T->magic = MAGIC_FREE;
    T->size = n;
    T->next = T;                                                                    //I've changed it to T to match the convention in lab03's list.c also, more condensed.
    T->prev = T;
}

//Malloc for the program above but using the suballocated region instead
void *sal_malloc(u_int32_t n) {
    // TODO
    return NULL; // temporarily
}

//Free all memory associated with suballocator
void sal_free(void *object) {
    // TODO
}

//Terminate the suballocator - must sal_init to use again
void sal_end(void) {
    // TODO
}

//Print all statistics regarding suballocator
void sal_stats(void) {

    // Optional, but useful
    printf("sal_stats\n");
    printf("Global Variable 'memory' is: %g", memory);
    printf("Global Variable 'free_list_ptr' is: %g", free_list_ptr);
    printf("Global Variable 'memory_size' is: %g", memory_size);

    // we "use" the global variables here
    // just to keep the compiler quiet
    memory = memory;
    free_list_ptr = free_list_ptr;
    memory_size = memory_size;
}
