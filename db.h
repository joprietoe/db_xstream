#ifndef DB_H
#define DH_H
#include "type.h"
#include <stdint.h> 
#include <stdarg.h>

extern void create_database(database db, char* filename);
extern void destroy_database(database db);
extern bool sql_stmt_prepare(database db, const char *sql, uint8_t argc, ...);
extern void sql_stmt(database db, const char* query, void * data, int (*callback)(void*,int,char**,char**));


#endif // DB_H