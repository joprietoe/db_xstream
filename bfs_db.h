#ifndef BFS_DB_H
#define BFS_DB_H

#include "db.h"
#include "type.h"
#include "sqlite3.h"
#include "utarray.h"
#include <stdint.h> 

typedef sqlite3* database;


void create_database(char* filename);
void destroy_database_db(dabase db);
//void select_stmt(const char* stmt, void * data, int (*callback)(void*,int,char**,char**));
void sql_stmt(const char* query, void * data, int (*callback)(void*,int,char**,char**));
void sql_stmt_prepare_vertex(const char *sql, UT_array * data, int64_t argc, int64_t *values);

//void insert_stmt(int64_t node);
//void gera_grafo(int64_t n, double dens);
//void igraph_de_bruijn(int64_t m, int64_t n);

#endif //BFS_DB_H