#ifndef BFS_DB_H
#define BFS_DB_H

#include "db.h"
#include "sqlite3.h"
#include "uthash.h"
#include "type.h"
#include <stdint.h> 
#include <stdarg.h>

typedef sqlite3* database;

void create_database(char* filename);
void destroy_database_db(dabase db);
//void select_stmt(const char* stmt, void * data, int (*callback)(void*,int,char**,char**));
void sql_stmt(const char* query, void * data, int (*callback)(void*,int,char**,char**));
bool sql_stmt_prepare(const char *sql, uint8_t argc, ...);
void sql_stmt_prepare_vertex(const char *sql, uthash **data, int64_t argc, int64_t *values);
bool sql_insert_updates(UT_array *updates);

//void insert_stmt(int64_t node);
//void gera_grafo(int64_t n, double dens);
//void igraph_de_bruijn(int64_t m, int64_t n);

#endif //BFS_DB_H