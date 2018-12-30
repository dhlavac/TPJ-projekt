/*
 * File: garbage.h 
 * @author: Dominik Hlavac Duran
 * Description: Header file of garbage collector
 */ 

#ifndef GARBAGE_INCLUDED
#define GARBAGE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "error.h"
 
#define MMU_SIZE 32

typedef enum typeofitem{
    mem_memory,
    mem_notdefined,
    mem_file,
} gmemtype;

typedef struct memtableitem{
    intptr_t key;
    gmemtype type;
    void* ptr;
    unsigned long allocated;

    struct memtableitem* next;
} *gmemtableitemptr, gmemtableitem;

typedef struct memtable{
    unsigned int size;
    gmemtableitem** data;
} gmemtable;

typedef struct memory_counters{
    gmemtable* table;
    unsigned long mallocs;
    unsigned long reallocs;
    unsigned long callocs;
    unsigned long fopens;
    unsigned long frees;
    unsigned long fcloses;
    unsigned long allocated;
} gmemcount;

extern gmemcount memtable;

//prototypes
void meminitialization(); /* initialization */
void* memmalloc(size_t size); /* malloc () */
void* memrealloc(void* ptr, size_t size); /* realloc() */
void* memcalloc(size_t num, size_t size); /* calloc() */
void* memfopen(const char* filename, const char* mode); /* open file */
void memfree(void* ptr); /* free specific memory */
void memfclose(void* ptr); /* close file */
void memallfree(); /* free all memory */

gmemtable* memorytablecreate();
gmemtableitem* memtitemcreate(intptr_t key);
void memtdestroy(gmemtable* T);
void memtitemdestroy(gmemtableitem* item);
void memtableinicialization(gmemtable* T, size_t size);
void memtabledispose(gmemtable* T);
gmemtableitem* memfindout(gmemtable* T, intptr_t key);
size_t hash(intptr_t key, size_t size);

#endif