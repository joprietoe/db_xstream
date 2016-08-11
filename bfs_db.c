#include "bfs_db.h"
#include "sqlite3.h"
#include "type.h"
#include "utarray.h"
#include <stdio.h>
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

void sql_stmt_prepare_vertex(const char *sql, UT_array * result, int64_t argc, int64_t *values){
    
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
            utarray_push_back(result, &value);
           // printf("Adya Node %d, Value : %d \n", r, value);

        } else if (retval == SQLITE_DONE) {
            break;
        } else {
            sqlite3_finalize(stmt);
            printf("Some error encountered\n");
            break;
        }

    }

    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
}


