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

//Function Prototypes
u_int32_t sizeToN(u_int32_t n);
free_header_t* toPointer(vlink_t index);
vlink_t toIndex(free_header_t* pointer);
vlink_t memoryDivide (vlink_t curr);
vlink_t enslaveRegion (vlink_t curr);
void merge(void);
void printHeaders(void);

//Essential Functions
//Initialise the suballocator, and malloc memory for it
void sal_init(u_int32_t size) {

    //Check if already initialised, do nothing if so
    if (memory != NULL) {
        return;
    }

    //Round size to n
    u_int32_t n = sizeToN(size);

    //set global variables | initialise suballocator
    memory = (byte *)malloc(n);         

    //check if malloc worked properly
    if (memory == NULL){
       fprintf(stderr, "sal_init: insufficient memory");
       abort();
    }

    //set global variables | size and initial location of free list
    free_list_ptr = memory[0]; //index of header of initial block
    memory_size = n;

    //this is dodge???
    //set first free list pointer
    free_header_t *T = (free_header_t *)memory;                                  
    T->magic = MAGIC_FREE;
    T->size = n;
    T->next = 0;                                                                
    T->prev = 0;
}

//Malloc for the program above but using the suballocated region instead
void *sal_malloc(u_int32_t n) {

    //Use the idea of current node to make conceptualising/coding easier
    vlink_t curr = free_list_ptr;

    //Round n to nearest upper power of two, including the header
    n = sizeToN(n + HEADER_SIZE);

/*
    //Check if the allocator is large enough
    if (n > memory_size) {
        return NULL;
    }
*/
    //Scan through list looking for region of size n
    //Makes the while loop work the first time free_list_ptr is passed
    int passCount = 0; 
    //Boolean variable to identify if a suitable region has been found     
    int regionFound = 0;    
    while (regionFound == 0) {
        //Ensure that loop will halt next time it reaches start
        if (curr == free_list_ptr && passCount != 0) {
            fprintf(stderr, "sal_malloc: insufficient memory\n");
            return NULL;
        }     

        //Print error message if region accessed has already been allocated 
        //(and should therefore have been removed from free list);
        if (toPointer(curr)->magic != MAGIC_FREE) {
            fprintf(stderr, "Memory corruption\n");
            sal_stats();
            abort();
        }


        //Case if region is sufficiently large    
        if ((toPointer(curr)->size) >= n) { //this is just going to pick the first region large enough and split it
            regionFound = 1;          //try to go through the whole list and find the smallest one that is large enough
        //Case if region is not large enough
        } else {
            curr = toPointer(curr)->next;
        }

        //Increment passCount
        passCount++;
        printf("passCount = %d, regionFound = %d, curr = %p, curr->next = %p\n", passCount, regionFound, toPointer(curr), toPointer(curr));
    } 
    printf("loop escaped\n");

    //Divide segment of memory into smallest possible size
    while (toPointer(curr)->size >= 2 * n) { //so it only splits if its more than twice the size, otherwise it will be too small
        curr = memoryDivide(curr);
    }
    printf("loop escaped\n");

    //Remove region from the free list
    curr = enslaveRegion(curr);

    //Return pointer to the first byte AFTER the region header
    return (void*)(toPointer(curr) + HEADER_SIZE);
}

void sal_free(void *object)
{
   // TODO
}

//Terminate the suballocator - must sal_init to use again
void sal_end(void) {

    //Free all global variables, which makes accessing the (now deleted) suballocator impossible
    free(memory);
    //memory = NULL; //should check if free does this
    free_list_ptr = 0;
    memory_size = 0;

}

//Print all statistics regarding suballocator
void sal_stats(void) {
    //Print the global variables
    printf("sal_stats\n\n");
    printf("Global Variable 'memory' is: %p\n", memory);
    printf("Global Variable 'free_list_ptr' is: %d\n", free_list_ptr);
    printf("Global Variable 'memory_size' is: %d\n\n", memory_size);
    
    printHeaders();
}


//Complementary Functions

//Return usable size from given n value
u_int32_t sizeToN(u_int32_t size) {

    int n = 1;
    //round size to the nearest upper power of two, unless already power of two
    if ((size != 0) && (size & (size - 1)) == 0) {
        n = size;
    } else {
        while (n < size) {
            //This isn't very efficient, but works for time being
            n = 2 * n;
        }
    }

    return n;
}

//Helper Functions
//Converts an index to a pointer
free_header_t* toPointer(vlink_t index) {
    return ((free_header_t*)(memory + index));
}

//Converts a pointer to an index
vlink_t toIndex(free_header_t* pointer) {
    return (pointer - (free_header_t *)memory);
}

//INCOMPLETE
//Splits the region of memory passed in into two
vlink_t memoryDivide(vlink_t curr) {

    //Create temporary vlink
    vlink_t temp = curr;              

    //Progress temp to the new divided region
    temp = temp + (toPointer(curr)->size) / 2;

    //Setup the new region header
    free_header_t *new = toPointer(temp);
    new->size = toPointer(curr)->size / 2;
    new->magic = MAGIC_FREE;

    //Shrink the old region
    toPointer(curr)->size = (toPointer(curr)->size) / 2;

    //Link the new regions to the old ones (and vice versa)
    toPointer(toPointer(curr)->next)->prev = toIndex(new); //ill trust you, but if you havnt then double check it, itll be a pain to find if it needs to be fixed
    new->next = toPointer(curr)->next;        
    //Now new points to the old curr->next and vice versa
    toPointer(curr)->next = toIndex(new);
    new->prev = curr;                                                                                                               ///this looks suss
    //Now curr points to new and vice versa

    return curr;
}

//Converts a region from free to allocated, and removes it from the free list
vlink_t enslaveRegion(vlink_t curr) {

    //Mark header as allocated
    toPointer(curr)->magic = MAGIC_ALLOC;
    //Change neighbour's links to skip the enslaved region
    toPointer(toPointer(curr)->prev)->next = toPointer(curr)->next; //i feel like this might skip one in one direction as well (im sure this is annoying (sorry) but its hard to fix and easy to map)
    toPointer(toPointer(curr)->next)->prev = toPointer(curr)->prev;
    //Destroy links within the allocated header
    toPointer(curr)->next = curr;
    toPointer(curr)->prev = curr;

    return curr;
}

void merge(void) {
    return;
}

//print out all headers 
void printHeaders(void) {

    //Start from the beginning
    vlink_t curr = free_list_ptr;

    do {
        //Print this header
        printf("curr: %d\n", curr);
        printf("curr->MAGIC: 0x%08x\n", toPointer(curr)->magic);
        printf("curr->size: %d\n", toPointer(curr)->size);    
        printf("curr->next: %d\n", toPointer(curr)->next);  
        printf("curr->prev: %d\n\n", toPointer(curr)->prev); 

        //Move along
        curr = toPointer(curr)->next;
    } while (curr != free_list_ptr);
 
    return;
}
