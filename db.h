#ifndef DB_H
#define DH_H
#include <stdint.h> 

//typedef struct database;
typedef sqlite3* database;;
typedef struct Vertex;
typedef struct Update;

void create_database(char* filename);
void destroy_database(dabase db);
void select_stmt(const char* stmt);
void sql_stmt(const char* stmt);
void insert_stmt(int64_t node);

#endif // DB_H