#include "algorithm.h"
#include "type.h"
#include "utarray.h"
#include "bfs_db.h"
#include <stdint.h>

#define MAXELEM 200

static bool init(database db, int64_t vertex) {

  char *query = "UPDATE vertex SET value = 0, parent = CASE WHEN id = ?1 THEN 0 ELSE -1 END";

  return !sql_stmt_prepare(query, 1, vertex);
}

void scatter(Vertex *v, Edge *e) {}
void gather(Vertex *v, Update *u) {}

Update *generate_update(Vertex *v, Edge *e) {}
bool apply_one_update(utarray *update) {}