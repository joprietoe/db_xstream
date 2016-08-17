#ifndef BFS_ALGORITHM_H
#define BFS_ALGORITHM_H

#include "algorithm.h"
#include "type.h"
#include "utarray.h"
#include "uthash.h"
#include "bfs_db.h"
#include <stdint.h>

bool init_alg(database db, int64_t vertex);


bool scatter_aux(database db, Vertex **vertices, Edge *e, int64_t phase, UT_array *updates);

bool scatter(database db, Vertex **vertices, int64_t min_id, int64_t max_id , int64_t phase, UT_array *updates);

void scatter_v2(database db, int64_t min_id, int64_t max_id , int64_t phase, UT_array *updates);

void gather(database db);

Update *generate_update(Vertex *v, Edge *e);

bool apply_update(database db, UT_array *updates);


#endif