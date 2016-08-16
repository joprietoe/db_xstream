#pragma once

#ifndef TYPE_H
#define TYPE_H
#include <stdint.h> 
#include "sqlite3.h"
#include "uthash.h"
#include "utarray.h"

typedef sqlite3* database;
typedef enum { false = 0, true = !false } bool;

typedef struct _vertex {
	int64_t id;        /*key */
    int64_t parent;
    int64_t value;
    UT_hash_handle hh;  /* makes this structure hashable */
} Vertex;


typedef struct _edge {
	int64_t from;
    int64_t to;
    int64_t cost;
} Edge;

typedef struct _update{
	int64_t id;
    int64_t parent;
    int64_t account;

} Update;

//utarray_new(pairs,&intpair_icd);
//UT_icd update_icd = {sizeof(Update), NULL, NULL, NULL}; Declare in main maethod

typedef struct _pair {
    uint64_t a;
    uint64_t b;
    
}pair;

 


#endif