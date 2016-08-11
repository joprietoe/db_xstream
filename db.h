#ifndef DB_H
#define DH_H
#include "types.h"
#include <stdint.h> 
#include <stdarg.h>

typedef sqlite3* database;;

extern void create_database(char* filename);
extern void destroy_database(dabase db);
extern bool sql_stmt_prepare(const char *sql, uint8_t argc, ...);
extern void sql_stmt(const char* query, void * data, int (*callback)(void*,int,char**,char**));
//void insert_stmt(int64_t node);

#endif // DB_H