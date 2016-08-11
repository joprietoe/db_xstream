#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "type.h"
#include "utarray.h"
#include "db.h"
#include <stdint.h>

extern static bool init(database db);
extern void scatter(Vertex *v, Edge *e);
extern void gather(Vertex *v, Update *u);
extern Update* generate_update(Vertex *v, Edge *e);
static bool apply_one_update(utarray *update);

#endif