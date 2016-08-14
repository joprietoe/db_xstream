#include "bfs_db.h"
#include "sqlite3.h"
#include "type.h"
#include "uthash.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h> 
#include <stdlib.h>
#include <limits.h>


//database db;

void create_database(database *db, char* filename){
   
  int rc; 
  sqlite3 *temp;
  rc = sqlite3_open(filename, db); 
  if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(temp));
      exit(1);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

  // db = temp;
  // sqlite3_close(db);

}

void destroy_database(database db){
    
    int rc;
    rc = sqlite3_close(db);
    if( rc ){
      fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
      exit(1);
   }else{
      fprintf(stderr, "Closed database successfully\n");
   }
  

}

void sql_stmt(database db, const char* query, void * data, int (*callback)(void*,int,char**,char**)){
    char *errmsg;
    int ret;
    
    ret = sqlite3_exec(db, query, callback, data, &errmsg);

    if (ret != SQLITE_OK) {
        printf("Error [%s].\n", errmsg);
    }
    
}

bool sql_stmt_prepare(database db, const char *sql, uint8_t argc, ...){
    
    char* errorMessage;
    int retval;
    bool result;
    sql_stmt(db,"BEGIN TRANSACTION", NULL, NULL);
    sqlite3_stmt *stmt;

    retval = sqlite3_prepare_v2(db, sql, sizeof(sql) + argc * sizeof(int64_t), &stmt, NULL);

    va_list valist;
    va_start(valist, argc);
    int i;
    for(i = 1; i <= argc; i++)
      sqlite3_bind_int64(stmt, i, va_arg(valist, int64_t)); // or i+1 check this

    va_end(valist);

    retval = sqlite3_step(stmt);
    if (retval != SQLITE_DONE && retval != SQLITE_OK) {
            fprintf(stderr, "Error query: %s\n", sqlite3_errmsg(db));
            result = true;
    }
    
    if(result)
        sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, &errorMessage);
    else    
        sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    return result;
}

bool sql_stmt_prepare_vertex(database db, const char *sql, Vertex **vertices, int64_t id_1, int64_t id_2){
    
    char* errorMessage;
    int retval;
    bool result = false;
    Vertex *value = NULL;
    sql_stmt(db,"BEGIN TRANSACTION", NULL, NULL);
    sqlite3_stmt *stmt;

    char buffer[] = "select id, parent,phase from vertex where id >= ?1 and id <= ?2";

    retval = sqlite3_prepare_v2(db, sql, strlen(buffer), &stmt, NULL);
  
    sqlite3_bind_int64(stmt, 1, id_1); // or i+1 check this
    sqlite3_bind_int64(stmt, 2, id_2);

    while (1) { //Here search something about sqlite3_step vs sqlite3_exec
        retval = sqlite3_step(stmt);

       if (retval == SQLITE_ROW) {
            value =  malloc(sizeof(Vertex));
            value->id = (int64_t) sqlite3_column_int(stmt, 0);
            value->parent = (int64_t) sqlite3_column_int(stmt, 1);
            value->value = (int64_t) sqlite3_column_int(stmt, 2);
            HASH_ADD_INT(*vertices, id, value);
            //utarray_push_back(result, &value);
            // printf("Adya Node %d, Value : %d \n", r, value);

        } else if (retval == SQLITE_DONE) {
            break;
        } else {
            sqlite3_finalize(stmt);
            printf("Some error encountered\n");
            result = true;
            break;
        }

    }

    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    return result;
}

bool sql_insert_updates(database db, UT_array *updates){
    
    char* errorMessage;
    //int retval;
    bool result = false;
    
    sql_stmt(db,"BEGIN TRANSACTION", NULL, NULL);
    sqlite3_stmt *stmt;

    char buffer[] = "INSERT update_table (id,node_id, phase) values (?1,?2,?3)";

    //retval = sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);
    sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);

    Update *p = NULL;
    for(p=(Update *)utarray_front(updates); p!=NULL; p=(Update *)utarray_next(updates,p)) {
        sqlite3_bind_int(stmt, 1, p->source_id);
        sqlite3_bind_int(stmt, 2, p->node_id);
        sqlite3_bind_int(stmt, 3, p->value);

        int code = sqlite3_step(stmt); 
        if ( code != SQLITE_DONE){
            fprintf(stderr, "Commit Failed! because %d\n", code); 
            result = true;
            break;
        }
 
        sqlite3_reset(stmt);
    }

    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    return result;
}


void gera_grafo(database db, int n, double dens) {

    int i, j;
    unsigned long edge;
    char* errorMessage;
    bool error_flag = false;

    //char *szSQL = "insert into edge_table (source, target) values (?,?)";
    sqlite3_stmt *stmt, *stmt_v;
    char buffer[] = "INSERT INTO edge_table VALUES (?1, ?2)";
    char buffer2[] = "INSERT INTO vertex VALUES (?1, -1 , ?2)";

    sqlite3_prepare_v2(db, buffer, strlen(buffer) + 1, &stmt, NULL);
    sqlite3_prepare_v2(db, buffer, strlen(buffer2) + 1, &stmt_v, NULL);
    printf("\n\n Gerando Grafo Aleatorio de Densidade: %f #Vertices: %d\n\n", dens, n);
    edge = 0;
    srand((unsigned) time(NULL));
    

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    for (i = 1; i <= n; i++) {
        
        sqlite3_bind_int(stmt_v, 1, i);
        sqlite3_bind_int(stmt_v, 2, INT_MAX);

        if (sqlite3_step(stmt_v) != SQLITE_DONE) {
               printf("Commit Failed inserting vertex [%d]!\n", i);
               error_flag = false;
               break;
        }
        sqlite3_clear_bindings(stmt_v);
        sqlite3_reset(stmt_v);
        for (j = 1; j <= n; j++) {

            double random = (double)rand()/(double)(RAND_MAX);
            if (i != j) {
                if (random < dens) {

                    sqlite3_bind_int(stmt, 1, i);
                    sqlite3_bind_int(stmt, 2, j);
                    edge++;
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        printf("Commit Failed!\n");
                        error_flag = false;
                        break;

                    }

                    //printf("Node i:%d,  --- Node j: %d, Edge: %lu \n", i, j, edge);
                    
                }

            }
            sqlite3_clear_bindings(stmt);
            sqlite3_reset(stmt);
            
        }

        //sqlite3_reset(stmt);
        //        sql_stmt("commit");



    }

    if(error_flag)
      sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, &errorMessage);
    else    
      sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    //sqlite3_reset(stmt_v);
    //sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

}