#ifndef DB_H
#define DH_H
#include <stdint.h> 

typedef sqlite3* database;;

void create_database(char* filename);
void destroy_database(dabase db);
//void select_stmt(const char* stmt, void * data, int (*callback)(void*,int,char**,char**));
void sql_stmt(const char* query, void * data, int (*callback)(void*,int,char**,char**));
//void insert_stmt(int64_t node);

#endif // DB_H