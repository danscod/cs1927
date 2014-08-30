//
//  COMP1927 Assignment 1 - Memory Suballocator
//  allocator.c ... implementation
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in August 2014
//  Copyright (c) 2012-2014 UNSW. All rights reserved.
//

// OpenLearning Group: "Danapalooza"
// Wednesday 18 Drum
// Danilo Scodellaro & Daniel Lohrey
// z3414551 | z5015215

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
    free_list_ptr = 0; //index of header of initial block
    memory_size = n;

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
    vlink_t region;

    //Check if the allocator is large enough
    if (n > memory_size || n == 0) {
        fprintf(stderr, "sal_malloc: insufficient memory (n > memory_size)\n");
        return NULL;
    } else if (n == 0) {
        fprintf(stderr, "sal_malloc: invalid memory size (n != 0)\n");
        return NULL;
    }   

    //Round n to nearest upper power of two, including the header
    n = sizeToN(n + HEADER_SIZE);

    //Scan through list looking for region of size n
    //Makes the while loop work the first time free_list_ptr is passed
    int passCount = 0; 
    //Boolean variable to identify if a suitable region has been found     
    int regionFound = 0; 
    do { //go until you get back to the start and a region is found
       
        //Ensure that loop will halt next time it reaches start 
        if (curr == free_list_ptr && passCount != 0) {
            fprintf(stderr, "sal_malloc: insufficient memory\n");
            return NULL;
        }     

        //Print error message if region accessed has already been allocated 
        //(and should therefore have been removed from free list);
        if (toPointer(curr)->magic != MAGIC_FREE) {
            fprintf(stderr, "Non-free region in list or memory is corrupted\n");
            abort();
        }


        //Case if region is sufficiently large
        if (toPointer(curr)->size >= n) {    
            regionFound = 1;
            region = curr;
        //Case if region is not large enough
        } else {
            curr = toPointer(curr)->next;
        }

        //Increment passCount
        passCount++;
        
    } while (regionFound == 0 && curr != free_list_ptr);


    if (regionFound == 0) {
        fprintf(stderr, "sal_malloc: insufficient memory\n");
        return NULL;
    }

    //Divide segment of memory into smallest possible size
    while (toPointer(region)->size >= 2 * n) {
        region = memoryDivide(region);
    }

    //Ensure that after allocation, there will be at least one free memory chunk
    vlink_t temp = free_list_ptr;
    int length = 0;
    do {
        temp = toPointer(temp)->next;
        length++;
        sal_stats();
    } while (temp != free_list_ptr);
    //Return NULL if this is not the case
    if (length <= 1) {
        return NULL;
    }

    //Remove region from the free list
    region = enslaveRegion(region);
    
    //Return pointer to the first byte AFTER the region header
    return (void *)(toPointer(region) + 1);
}

//De-allocates a section of memory no longer needed by the user
void sal_free(void *object) {

    //As object points to memory AFTER the header, go back to start of header
    object = object - HEADER_SIZE;

    //Get the index for object
    vlink_t objectIndex = toIndex(object);

    //Ensure the region is not already free
    if (toPointer(objectIndex)->magic != MAGIC_ALLOC) {
        fprintf(stderr, "Attempt to free non-allocated memory\n");
        abort();
    }

    vlink_t temp = toPointer(free_list_ptr)->next;
    vlink_t curr = temp;
    while (temp != free_list_ptr){
        if (temp < curr){
            curr = temp;
        }
        temp = toPointer(temp)->next;
    }
    //curr is now the lowest position in free list
    
    //Find where in the list the object belongs
    while (curr < objectIndex) {
        curr = toPointer(curr)->next;
        if (curr == free_list_ptr){
           break;
        }
    }

    //make a new header
    free_header_t *T = (free_header_t *)object;                                  
    T->magic = MAGIC_FREE;
    T->size = toPointer(objectIndex)->size;
    T->next = curr;                                                                
    T->prev = toPointer(curr)->prev;

    //Insert object back into the list
    toPointer(toPointer(curr)->prev)->next = objectIndex;
    toPointer(curr)->prev = objectIndex;
    
    //check that list is in tact
    temp = toPointer(free_list_ptr)->next;
    while (temp != free_list_ptr){
        if (toPointer(temp)->magic != MAGIC_FREE) {
            fprintf(stderr, "Non-free region in list or memory is corrupted\n");
            abort();
        }
        temp = toPointer(temp)->next;
    }
    
    //Attempt to merge adjacent regions
    merge();
}

//Terminate the suballocator - must sal_init to use again
void sal_end(void) {

    //Free all global variables
    free(memory);
    memory = NULL;
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

//Additional Functions
//Print out all headers 
void printHeaders(void) {

    //Start from the beginning
    vlink_t curr = free_list_ptr;

    do {
        //Print this header
        printf("curr(index): %d\n", curr);
        printf("curr(pointer): %p\n", toPointer(curr));
        printf("curr->MAGIC: 0x%08x\n", toPointer(curr)->magic);
        printf("curr->size: %d\n", toPointer(curr)->size);    
        printf("curr->next: %d\n", toPointer(curr)->next);  
        printf("curr->prev: %d\n\n", toPointer(curr)->prev); 
        //Move along
        curr = toPointer(curr)->next;
    } while (curr != free_list_ptr);
 
    return;
}

//Helper Functions
//Converts an index to a pointer
free_header_t *toPointer(vlink_t index) {
    return ((free_header_t *)(memory + index));
}

//Converts a pointer to an index
vlink_t toIndex(free_header_t* pointer) {
    return ((byte *)pointer - memory);
}

//Return usable size from given n value
u_int32_t sizeToN(u_int32_t size) {

    int n = 1;
    //round size to the nearest upper power of two
    if ((size != 0) && (size & (size - 1)) == 0) { //check if power of two
        n = size;
    } else {
        //This is not the most efficient method, but fine for small numbers
        while (n < size) {
            n = 2 * n;
        }
    }
    return n;
}

//Splits the region of memory passed in into two
vlink_t memoryDivide(vlink_t curr) {

    //Create temporary vlink
    vlink_t temp = curr;              
    //Progress temp to the new divided region
    temp = temp + ((toPointer(curr)->size) / 2);

    //Setup the new region header
    free_header_t *new = toPointer(temp);
    new->size = toPointer(curr)->size / 2;
    new->magic = MAGIC_FREE;

    //Shrink the old region
    toPointer(curr)->size = (toPointer(curr)->size) / 2;
    
    //Link the new regions to the old ones (and vice versa)
    toPointer(toPointer(curr)->next)->prev = toIndex(new);
    new->next = toPointer(curr)->next;        

    //Now new points to the old curr->next and vice versa
    toPointer(curr)->next = toIndex(new);
    new->prev = curr; 

    return curr;
}

//Converts a region from free to allocated, and removes it from the free list
vlink_t enslaveRegion(vlink_t curr) {

    //If the enslaved region is the same as free_list_ptr, move it
    if (curr == free_list_ptr) {
        free_list_ptr = toPointer(curr)->next;
    }
    //Mark header as allocated
    toPointer(curr)->magic = MAGIC_ALLOC;
    //Change neighbour's links to skip the enslaved region
    toPointer(toPointer(curr)->prev)->next = toPointer(curr)->next;
    toPointer(toPointer(curr)->next)->prev = toPointer(curr)->prev;
    //Destroy links within the allocated header
    toPointer(curr)->next = curr;
    toPointer(curr)->prev = curr;

    return curr;
}

//Merges two adjacent free chunks of memory
void merge(void) {

    //set to next so you can loop until its found again
    int pass = 0;
    //loop until adjacent regions of equal size are found
    vlink_t curr = toPointer(free_list_ptr)->next;
    while (curr != free_list_ptr) {
        //check whether list contains any non-free regions 
        if (toPointer(curr)->magic != MAGIC_FREE) {
                fprintf(stderr, "Attempt to free non-allocated memory\n");
                abort();
        }
        curr = toPointer(curr)->next;
    }
    while (toPointer(toPointer(curr)->next)->size != toPointer(curr)->size) {
        //ends if it goes through whole list
        if (curr == free_list_ptr && pass == 1) {
            return;
        }
        curr = toPointer(curr)->next;
        pass = 1;
    }

    //check whether to merge
    if (curr % (toPointer(curr)->size * 2) == 0) {
      toPointer(curr)->size = toPointer(curr)->size * 2;
      toPointer(toPointer(toPointer(curr)->next)->next)->prev = curr;
      toPointer(curr)->next = toPointer(toPointer(curr)->next)->next;
    }
    
    //recurses to check if another set can be merged, starts at new position
    free_list_ptr = toPointer(curr)->next;
    pass = 0;
    while (curr != free_list_ptr && pass == 0) {
        //ends if it goes through whole list
        if (curr == free_list_ptr && pass == 1) {
            return;
        }
        //if another region is found; recurse
        if (toPointer(toPointer(curr)->next)->size 
            != toPointer(curr)->size) {
           merge();
        }
        curr = toPointer(curr)->next;
        pass = 1;
    }

    return;
}

//This merge function is not fully functional - while it does merge some 
//segments of memory, it doesn't merge all. Likely problem exists in how a loop 
//or recursion has been implemented, however we were unable to find the root 
//of the problem