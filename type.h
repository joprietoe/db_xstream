#ifndef TYPE_H
#define TYPE_H
#include <stdint.h> 

typedef enum { false = 0, true = !false } bool;

typedef struct _vertex {
	int64_t id;
    int64_t parent;
    int64_t value;
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

typedef enum TABLE{
	VERTEX, 
	EDGE
};
#endif