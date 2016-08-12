#ifndef TYPE_H
#define TYPE_H
#include <stdint.h> 
#include "uthash.h"
#include "utarray.h"

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
	int64_t source_id;
    int64_t node_id;
    int64_t value;

} Update;

//utarray_new(pairs,&intpair_icd);
UT_icd update_icd = {sizeof(Update), NULL, NULL, NULL};

typedef enum TABLE{
	VERTEX, 
	EDGE
};
#endif