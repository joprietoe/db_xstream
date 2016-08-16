#ifndef GENERATOR_H
#define GENERATOR_H

#include "type.h"

void import_from_txt_file(database db, char *vertex_filename, char *edge_filename);
void gera_grafo(database db, int n, double dens);
void igraph_de_bruijn(database db, int m, int n);
#endif
