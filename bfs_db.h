#ifndef BFS_DB_H
#define BFS_DB_H

#include "db.h"
#include "sqlite3.h"
#include <stdint.h> 

/*struct database { 
     sqlite3* db;
     char* filename;
};*/

typedef sqlite3* database;

struct Vertex {
	int64_t id;
    int64_t parent;
    int64_t value;
};

struct Update {
    int64_t node;
    int64_t id;
    int64_t value;
}

void create_database(char* filename);
void destroy_database_db(dabase db);
void select_stmt(const char* stmt);
void sql_stmt(const char* stmt);
void insert_stmt(int64_t node);
//void gera_grafo(int64_t n, double dens);
//void igraph_de_bruijn(int64_t m, int64_t n);

#endif //BFS_DB_H