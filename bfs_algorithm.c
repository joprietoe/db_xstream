#include "algorithm.h"
#include "type.h"
#include "utarray.h"
#include "uthash.h"
#include "bfs_db.h"
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>

bool init_alg(database db, int64_t vertex) {

  char *query = "UPDATE vertex SET phase = 0, parent = 0 where id = ?1";
  return !sql_stmt_prepare(db, query, 1, vertex);
}

bool scatter_aux(database db, Vertex **vertices, Edge *e, int64_t phase, UT_array *updates) {
      Update *up = NULL;
      Vertex *v = NULL;
      HASH_FIND_INT(*vertices, &e->from, v);
      if(v->parent != -1 && v->value == phase - 1){  // IMPORTANT TO MAKE A RIGHT UPDATE (v->value == phase - 1)!
        
        do {
          up = generate_update(v,e);// save array<update> if memory is full?
          if(up){
            //v->parent = up->node_id;
            //v->value = up->value;
            up->account = phase;
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

bool scatter(database db, Vertex **vertices, int64_t min_id, int64_t max_id , int64_t phase, UT_array *updates){

    char* errorMessage;
    int retval;
    bool result = false;
    Edge *edge = NULL;
    sql_stmt(db,"BEGIN TRANSACTION", NULL, NULL);
    sqlite3_stmt *stmt;
    //source integer, target integer, cost integer
    char buffer[] = "select source, target, cost from edge where source >= ?1 and source <= ?2";

    retval = sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);
  
    sqlite3_bind_int64(stmt, 1, min_id); // or i+1 check this
    sqlite3_bind_int64(stmt, 2, max_id);

    while (1) { //Here search something about sqlite3_step vs sqlite3_exec
        retval = sqlite3_step(stmt);

       if (retval == SQLITE_ROW) {
            edge =  malloc(sizeof(Edge));
            edge->from = (int64_t) sqlite3_column_int(stmt, 0);
            edge->to = (int64_t) sqlite3_column_int(stmt, 1);
            edge->cost = (int64_t) sqlite3_column_int(stmt, 2);
            if(scatter_aux(db,vertices,edge,phase,updates))
                result = true;
            free(edge);
            
            //utarray_push_back(result, &value);
            // printf("Adya Node %d, Value : %d \n", r, value);

        } else if (retval == SQLITE_DONE) {
            break;
        } else {
            sqlite3_finalize(stmt);
            printf("Some error encountered\n");
            result = false;
            break;
        }

    }

    //sqlite3_clear_bindings(stmt);
    //sqlite3_reset(stmt);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    return result;
     
}



void gather(database db) {
  
 printf("gather phase\n"); 
  char *query = "UPDATE vertex  SET " 
            "parent = (SELECT update_table.parent FROM update_table WHERE vertex.id = update_table.id ),"
            "phase = (SELECT update_table.account FROM update_table WHERE vertex.id = update_table.id ) "
            "where id in (select update_table.id from update_table where update_table.id = id "
            "and vertex.phase >= update_table.account )";

  sql_stmt(db,query, NULL,NULL);

}

Update *generate_update(Vertex *v, Edge *e) {

    Update *u = (Update *)malloc(sizeof(Update));
    
    if(u){
      u->id = e->to; 
      u->parent = e->from;
    }
    
    return u;
}

bool apply_update(database db, UT_array *updates) {

 printf("apply_update\n");
  bool ret = sql_insert_updates(db, updates);
  utarray_clear(updates);
  return ret;
}

