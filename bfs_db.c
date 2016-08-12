#include "bfs_db.h"
#include "sqlite3.h"
#include "type.h"
#include "uthash.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h> 
#include <stdlib.h>


//database db;

void create_database(char* filename){
   
  int rc; 
  
  rc = sqlite3_open(filename, &db); 
  if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   sqlite3_close(db);

}

void destroy_database(dabase db){
    
    int rc;
    rc = sqlite3_close(db);
    if( rc ){
      fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Closed database successfully\n");
   }
  

}

//
//int (*callback)(void*,int,char**,char**),  /* Callback function */
//  void *,                                    /* 1st argument to callback */
//
void sql_stmt(const char* query, void * data, int (*callback)(void*,int,char**,char**)){
    char *errmsg;
    int ret;
    
    ret = sqlite3_exec(db, query, callback, data, &errmsg);

    if (ret != SQLITE_OK) {
        printf("Error in statement: %s [%s].\n", stmt, errmsg);
    }
    sqlite3_finalize(stmt);
}

bool sql_stmt_prepare(const char *sql, uint8_t argc, ...){
    
    char* errorMessage;
    int retval;
    sql_stmt("BEGIN TRANSACTION", NULL, NULL);
    sqlite3_stmt *stmt;

    retval = sqlite3_prepare_v2(db, query, strlen(buffer), &stmt, NULL);

    va_list valist;
    va_start(valist, argc);
    int i;
    for(i = 1; i <= argc; i++)
      sqlite3_bind_int64(stmt, i, va_arg(valist, int64_t)); // or i+1 check this

    va_end(valist);

    while (1) { 
        retval = sqlite3_step(stmt);
       if (retval == SQLITE_DONE) {
            break;
        } else {
            sqlite3_finalize(stmt);
            printf("Some error encountered\n");
            return true;
            break;
        }
    }
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    return false;
}

bool sql_stmt_prepare_vertex(const char *sql, uthash **result, int64_t argc, int64_t *values){
    
    char* errorMessage;
    int retval;
    Vertex *value;
    sql_stmt("BEGIN TRANSACTION", NULL, NULL);
    sqlite3_stmt *stmt;

    //char buffer[] = "select id, parent,value from vertex where id >= ? and id <= ?";

    retval = sqlite3_prepare_v2(db, query, strlen(buffer), &stmt, NULL);

    int i;
    for(i = 0; i < argc; i++)
      sqlite3_bind_int64(stmt, 1, values[i]); // or i+1 check this

    while (1) { //Here search something about sqlite3_step vs sqlite3_exec
        retval = sqlite3_step(stmt);

       if (retval == SQLITE_ROW) {
            value = (Vertex *) malloc(sizeof(Vertex));
            value->id = (int64_t) sqlite3_column_int(stmt, 0);
            value->parent = (int64_t) sqlite3_column_int(stmt, 1);
            value->value = (int64_t) sqlite3_column_int(stmt, 2);
            HASH_ADD_INT( *result, id, value );
            //utarray_push_back(result, &value);
            // printf("Adya Node %d, Value : %d \n", r, value);

        } else if (retval == SQLITE_DONE) {
            break;
        } else {
            sqlite3_finalize(stmt);
            printf("Some error encountered\n");
            return true;
            break;
        }

    }

    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    return false;
}
	/*int64_t source_id;
    int64_t node_id;
    int64_t value;*/
//Check insert segun lo de mysqlite
bool sql_insert_updates(UT_array *updates){
    
    char* errorMessage;
    int retval;
    Vertex *value;
    sql_stmt("BEGIN TRANSACTION", NULL, NULL);
    sqlite3_stmt *stmt;

    char buffer[] = "UPDATE update_table SET id = ?1, node_id = ?2, value = ?3";

    retval = sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);

    Update *p;
    for(p=(Update *)utarray_front(updates); p!=NULL; p=(Update *)utarray_next(update,p)) {
        sqlite3_bind_int(stmt, 1, p->source_id);
        sqlite3_bind_int(stmt, 2, p->node_id);
        sqlite3_bind_int(stmt, 3, p->value);

        int code = sqlite3_step(stmt); 
        if ( code != SQLITE_DONE){
            fprintf(stderr, "Commit Failed! because %d\n", code); 
            return true; //or just break? Check this with my past project
        }
 
        sqlite3_reset(stmt);
    }

    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    return false;
}


