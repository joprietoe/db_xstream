#include "algorithm.h"
#include "type.h"
#include "utarray.h"
#include "uthash.h"
#include "bfs_db.h"
#include <stdint.h>

static bool init(database db, int64_t vertex) {

  char *query = "UPDATE vertex SET value = 0, parent = CASE WHEN id = ?1 THEN 0 ELSE -1 END";

  return !sql_stmt_prepare(query, 1, vertex);
}

static bool scatter(uthash **vertices, Edge *e, int64_t phase, UT_array *updates) {
      Update up;
      Vertex *v = HASH_FIND_INT(*vertices, &e->from, v);
      if(v->parent == (int64_t)-1 || (v->parent != -1 && v->value == phase)){
        up = generate_update(v,e);// save array<update> if memory is full?
        utarray_push_back(updates, &up);
        return true;
      }
      
      return false;
}
static void gather(UT_array *updates) {
      
}

Update *generate_update(Vertex *v, Edge *e) {


}

static bool apply_one_update(UT_array *updates) {

  bool ret = sql_insert_updates(updates);
  utarray_free(updates);
  return ret;
}