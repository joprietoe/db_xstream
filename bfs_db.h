#ifndef BFS_DB_H
#define BFS_DB_H

#include "db.h"
#include "sqlite3.h"
#include "uthash.h"
#include "type.h"
#include <stdint.h> 
#include <stdarg.h>

void create_database(database *db, char* filename);
void destroy_database_db(database db);
void sql_stmt(database db, const char* query, void * data, int (*callback)(void*,int,char**,char**));
bool sql_stmt_prepare(database db, const char *sql, uint8_t argc, ...);
bool sql_stmt_prepare_vertex(database db, Vertex **vertices, int64_t id_1, int64_t id_2);
bool sql_insert_updates(database db, UT_array *updates);
void gera_grafo(database db, int n, double dens);

#endif //BFS_DB_H