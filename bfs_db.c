#include "bfs_db.h"
#include "sqlite3.h"
#include <stdio.h>


database db;

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

void select_stmt(const char* stmt){
    char *errmsg;
    int ret;

    ret = sqlite3_exec(db, stmt, 0, 0, &errmsg);

    if (ret != SQLITE_OK) {
        printf("Error in statement: %s [%s].\n", stmt, errmsg);
    }
    sqlite3_finalize(stmt);
}
void sql_stmt(const char* stmt);
void insert_stmt(int64_t node);
