/*
 * File: garbage.c 
 * Description: Modification of functions for alocation and dealocation of memory 
 */

#include "garbage.h"

// table with data of alocations ad frees
gmemcount memtable;


void meminitialization() {
    memtable.mallocs = 0;
    memtable.reallocs = 0;
    memtable.callocs = 0;
    memtable.fopens = 0;
    memtable.frees = 0;
    memtable.fcloses = 0;
    memtable.allocated = 0;
    memtable.table = NULL;
	
    memtable.table = memorytablecreate();

    memtableinicialization(memtable.table, MMU_SIZE);
}

void* memmalloc(size_t size) {
	memtable.mallocs++;
	memtable.allocated  += size;

	void* tmpptr = malloc(size);

	if (tmpptr == NULL) {
	memallfree();
	exit(99);
	}

    gmemtableitem* item = memfindout(memtable.table, (intptr_t)tmpptr);


    item->ptr = tmpptr;
    item->allocated = size;
    item->type = mem_memory;

    return tmpptr;
}	

void* memrealloc(void* ptr, size_t size){
    memtable.reallocs++;
    memtable.allocated = memtable.allocated + size;

    void* tmpptr = realloc(ptr, size);

    if (tmpptr == NULL) {
        memallfree();
	exit(99);
    }

    gmemtableitem* item = NULL;
	
    if(tmpptr != ptr){
        item = memfindout(memtable.table, (intptr_t)ptr);
        item->allocated = 0;
        item->ptr = NULL;
    }

    item = memfindout(memtable.table, (intptr_t)tmpptr);


    item->ptr = tmpptr;
    item->allocated = size;
    item->type = mem_memory;

    return tmpptr;
}

void* memcalloc(size_t num, size_t size){
    memtable.callocs++;
    memtable.allocated = memtable.allocated + size;

    void* tmpptr = calloc(num, size);

    if (tmpptr == NULL) {
      memallfree();
      exit(99);
    }

    gmemtableitem* item = memfindout(memtable.table, (intptr_t)tmpptr);

    item->ptr = tmpptr;
    item->allocated = size;
    item->type = mem_memory;

    return tmpptr;
}

void* memfopen(const char* filename, const char* mode){
    memtable.fopens++;
    memtable.allocated = memtable.allocated + sizeof(FILE*);

    void* tmpptr = (FILE*)fopen(filename, mode);

    if(!tmpptr)
        return NULL;

    gmemtableitem* item = memfindout(memtable.table, (intptr_t)tmpptr);

    item->ptr = tmpptr;
    item->allocated = sizeof(FILE*);
    item->type = mem_file;

    return tmpptr;
}

void memfree(void* ptr){
    memtable.frees++;

    if(!ptr)
        return;

    gmemtableitem* item = memfindout(memtable.table, (intptr_t)ptr);

    if(!item)
        return;

    if(item->allocated != 0){
        free(ptr);
        item->allocated = 0;
        item->ptr = NULL;
    }
}

void memfclose(void* ptr){
    memtable.fcloses++;

    if(!ptr){
        return;
    }

    gmemtableitem* item = memfindout(memtable.table, (intptr_t)ptr);

    if(!item){
        return;
    }

    if(item->allocated != 0){
        fclose(ptr);
        item->allocated = 0;
        item->ptr = NULL;
    }

}

void memallfree(){
    if(!memtable.table && !memtable.table->data)
        return;

    gmemtableitemptr head = NULL;
    gmemtableitemptr item = NULL;


    for(unsigned int i = 0; i < memtable.table->size; i++){
        if(memtable.table->data[i] != 0){
            head = memtable.table->data[i];

            while((item = head)){
                head = head->next;

                if(item->allocated != 0){
                    if(item->type == mem_file)
                        memfclose(item->ptr);
                    else
                        memfree(item->ptr);
                }

                memtitemdestroy(item);

                memtable.table->data[i] = head;
            }

            memtable.table->data[i] = NULL;
        }
    }

    memtabledispose(memtable.table);

    memtdestroy(memtable.table);

    memtable.table = NULL;
}
    
gmemtable* memorytablecreate(){
    return (gmemtable*)malloc(sizeof(gmemtable));
}

gmemtableitem* memtitemcreate(intptr_t key){
    gmemtableitem* item = (gmemtableitem*)malloc(sizeof(gmemtableitem));

    if(!item)
        return NULL;

    item->ptr = NULL;
    item->next = NULL;
    item->key = key;
    item->type = mem_notdefined;

    return item;
}

void memtdestroy(gmemtable* T){
    if(!T)
        return;

    free(T);
}

void memtitemdestroy(gmemtableitem* item){
    if(!item)
        return;

    free(item);
}

void memtableinicialization(gmemtable* T, size_t size){
    if(!T)
        return;

    T->data = calloc(size, sizeof(gmemtableitemptr));

    if(!T->data)
        return;

    T->size = size;
}

void memtabledispose(gmemtable* T){
    if(!T)
        return;

    if(T->data)
        free(T->data);

    T->data = NULL;
}

gmemtableitem* memfindout(gmemtable* T, intptr_t key){
    if(!T)
        return NULL;

    size_t index = hash(key, T->size);

    gmemtableitem* item = T->data[index];

    if(!item){
        T->data[index] = memtitemcreate(key);

        return (T->data[index]);
    }

    while(item){
        if(item->key == key)
            return item;

        if(!item->next)
            break;

        item = item->next;
    };

    gmemtableitem* newItem = memtitemcreate(key);

    return (!newItem)? NULL : (item->next = newItem);
}

size_t hash(intptr_t key, size_t size){
    size_t sum = 0;

    while(key > 9){
        sum += key % 10;
        key /= 10;
    }

    return (97 * sum)%size;
}
