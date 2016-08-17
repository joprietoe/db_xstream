#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "type.h"
#include "utarray.h"
#include "uthash.h"
#include "db.h"
#include <stdint.h>

extern bool init(database db, int64_t vertex);
extern bool scatter(database db, Vertex **vertices, int64_t min_id, int64_t max_id , int64_t phase, UT_array *updates);
extern void scatter_v2(database db, int64_t min_id, int64_t max_id , int64_t phase, UT_array *updates);
extern void gather(database db);
extern Update* generate_update(Vertex *v, Edge *e);
extern bool apply_update(database db, UT_array *update);

#endif