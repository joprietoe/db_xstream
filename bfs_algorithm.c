#include "algorithm.h"
#include "type.h"
#include "utarray.h"
#include "uthash.h"
#include "bfs_db.h"
#include <stdint.h>

bool init(database db, int64_t vertex) {

  char *query = "UPDATE vertex SET value = 0, parent = CASE WHEN id = ?1 THEN 0 ELSE -1 END";
  return !sql_stmt_prepare(db, query, 1, vertex);
}

bool scatter(database db, Vertex **vertices, Edge *e, int64_t phase, UT_array *updates) {
      Update *up = NULL;
      Vertex *v = NULL;
      HASH_FIND_INT(*vertices, &e->from, v);
      if(v->parent == (int64_t)-1 || (v->parent != -1 && v->value == phase)){
        
        do {
          up = generate_update(v,e);// save array<update> if memory is full?
          if(up){
            v->parent = up->node_id;
            v->value = up->value;
            up->value = phase;
            utarray_push_back(updates, up);
            return true;
          }
          else{
            apply_update(db,updates);
          }
        } while(!up);
      }
      
      return false;
}

void gather(database db) {
  
  char *query = "UPDATE vertex  SET "
            "parent = (SELECT update_table.source FROM update_table WHERE vertex.id = update_table.node and vertex.phase >= update_table.phase),"
             "phase = (SELECT update_table.phase FROM update_table WHERE vertex.id = update_table.node and vertex.phase >= update_table.phase)";

  sql_stmt(db,query, NULL,NULL);

}

Update *generate_update(Vertex *v, Edge *e) {

    Update *u = (Update *)malloc(sizeof(Update));
    
    if(u){
      u->source_id = v->id; 
      u->node_id = e->to;
    }
    
    return u;
}

bool apply_update(database db, UT_array *updates) {

  bool ret = sql_insert_updates(db, updates);
  utarray_clear(updates);
  return ret;
}