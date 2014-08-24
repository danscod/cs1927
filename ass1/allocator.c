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
vlink_t memoryDivide (vlink_t curr);
vlink_t enslaveRegion (vlink_t curr);

//Initialise the suballocator, and malloc memory for it
void sal_init(u_int32_t size) {

    //Check if already initialised, do nothing if so
    if (memory != NULL) {
        return;
    }

    //Round size to n
    u_int32_t n = sizeToN(size);

    //set global variables | initialise suballocator
    memory = malloc(n);         //May need (byte *) before malloc(), note the static type

    //check if malloc worked properly
    if (memory == NULL){
       fprintf(stderr, "sal_init: insufficient memory");
       abort();
    }

<<<<<<< HEAD
    //set global variables | size and initial location of free list
    free_list_ptr = memory[0]; //index of header of initial block
    memory_size = n;

    //set first free list pointer
    free_header_t *T = (free_header_t *)memory[0];                                  
=======
    //DANIEL NEED A COMMENT HERE TO EXPLAIN
    free_list_ptr = 0;
    memory_size = n;

    //set first free list pointer
    free_header_t *T = (free_header_t *)memory;                                  
>>>>>>> origin/master
    T->magic = MAGIC_FREE;
    T->size = n;
    T->next = T;                                                                
    T->prev = T;
}

//Malloc for the program above but using the suballocated region instead
<<<<<<< HEAD
//this looks pretty good, but go through the logic again and maybe change te order of some things (it could be fine though)
=======
>>>>>>> origin/master
void *sal_malloc(u_int32_t n) {

    //Use the idea of current node to make conceptualising/coding easier
    vlink_t curr = free_list_ptr;

    //Round n to nearest upper power of two, including the header
    u_int32_t n = sizeToN(n + HEADER_SIZE);

    //Scan through list looking for region of size n
    int passCount = 0;      //Makes the while loop work the first time free_list_ptr is passed (i.e. the beginning of search)
    int regionFound = 0;    //Boolean variable to identify if a suitable region has been found
    while (regionFound == 0) {
        //Ensure that loop will halt next time it reaches start
        if (curr == free_list_ptr && passCount != 0) {
<<<<<<< HEAD
            fprintf(stderr, "sal_malloc: insufficient memory");
=======
>>>>>>> origin/master
            return NULL;
        }     

        //Print error message if region accessed has already been allocated (and should therefore have been removed from free list);
        if (curr->magic == MAGIC_ALLOC) {
            fprintf(stderr, "Memory corruption");
            abort();
        }

        //Special case for undivided memory
        if (curr->next == curr) {
            //Check if suballocator is large enough
            if (curr->size >= n) {
<<<<<<< HEAD
                regionFound = 1; //if this is all youre doing here then you may as well just group it with below as its the same
            } else {
                //Meaning that suballocator isn't big enough
=======
                regionFound = 1;
            } else {
                //Meaning that suballocator isn't big enough?
>>>>>>> origin/master
                return NULL;
            }
        //Case if region is sufficiently large    
        } else if (curr->size >= n) {
            regionFound = 1;
        //Case if region is not large enough
        } else {
            curr = curr->next;
        }

        //Increment passCount
        passCount++;
    }

    //Divide segment of memory into smallest possible size
    while (curr->size >= n + HEADER_SIZE) {
        curr = memoryDivide(vlink_t curr);
    }

    //Remove region from the free list
    curr = vlink_t enslaveRegion (vlink_t curr);

    //Return pointer to the first byte AFTER the region header
    return ((void*)(curr + HEADER_SIZE));
}


<<<<<<< HEAD
//Free memory associated with specific block pointer
void sal_free(void *object) {
    // TODO
    merge();
=======
//Free all memory associated with suballocator
void sal_free(void *object) {
    // TODO
<<<<<<< HEAD

    //To clarify what object is...
    vlink_t 
=======
>>>>>>> origin/master
>>>>>>> origin/master
}

//Terminate the suballocator - must sal_init to use again
void sal_end(void) {

    //Free all global variables, which makes accessing the (now deleted) suballocator impossible
    free(memory);
<<<<<<< HEAD
    free(free_list_ptr); //dont think this and below need to be freed as they arent pointers, just variables(?)
    free(memory_size);

}

//Print all statistics regarding suballocator
void sal_stats(void) {
    printf("sal_stats\n");
    printf("Global Variable 'memory' is: %g", memory);
    printf("Global Variable 'free_list_ptr' is: %g", free_list_ptr);
    printf("Global Variable 'memory_size' is: %g", memory_size);
}

//New Functions -- will have to put prototypes in header file
=======
    free(free_list_ptr);
    free(memory_size);

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

//New Functions
>>>>>>> origin/master

//Functions for sal_init and sal_malloc
//Return usable size from given n value
u_int32_t sizeToN(u_int32_t size) {

    //round size to the nearest upper power of two, unless already power of two
    if ((size != 0) && (size & (size-1)) == 0) {
        n = size;
        break;
    } else {
        n = 1;
        while (n < size) {
            //This isn't very efficient, but works for time being
            n = 2 * n;
    }

    return n;
}

//Functions for sal_malloc
//Splits the region of memory passed in into two
vlink_t memoryDivide(vlink_t curr) {

    //Extract temporary void pointer from curr (for arithmetic) 
<<<<<<< HEAD
    (void*) temp = (void*)(curr); //i think youre meant to use a byte type but it probably doesnt matter
    //^ you dont need brackets here -- just void *temp but maybe ^
=======
    (void*) temp = (void*)(curr);

>>>>>>> origin/master
    //Progress temp to the new divided region
    temp = temp + (current->size)/2;

    //Setup the new region header
    vlink_t new = temp;
    new->size = curr->size/2;
    new->magic = MAGIC_FREE;

    //Shrink the old region
    curr->size = (curr->size)/2;

    //Link the new regions to the old ones (and vice versa)
    curr->next->prev = new;
    new->next = curr->next;     
    //Now new points to the old curr->next and vice versa
    curr->next = new;
    new->prev = curr;           
    //Now curr points to new and vice versa

    return curr;
<<<<<<< HEAD
}

//Converts a region from free to allocated, and removes it from the free list
vlink_t enslaveRegion(vlink_t curr) {

    //Mark header as allocated
    curr->magic = MAGIC_ALLOC;
    //Change all links to point to self
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    //also links that point to it need to point to either side of it (not sure if you did this, just by reading the comment)
    return curr;
}

void merge(void)
{
   free_header_t object = free_list_ptr->next;
   while (object->next->size != object->size){
      if (object == free_list_ptr){
         return;
      }
      object = object->next;
      if (object->magic != MAGIC_FREE){
         printf("Non-free region in list");
         exit(1);
      }
   }
   if (((unsigned char *)object - (unsigned char *)memory) % (object->size) * 2 == 0){
      object->size = object->size * 2;
      object->next->next->prev = object;
      object->next = object->next->next;
   } else {
      object = object->prev;
      object->size = object->size * 2;
      object->next->next->prev = object;
      object->next = object->next->next;
   }
   object = free_list_ptr;
   merge();
=======
>>>>>>> origin/master
}

//Converts a region from free to allocated, and removes it from the free list
vlink_t enslaveRegion(vlink_t curr) {

    //Mark header as allocated
    curr->magic = MAGIC_ALLOC;
    //Change all links to point to self
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;

    return curr;
} 
