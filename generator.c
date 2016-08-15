#include "generator.h"
#include "type.h"
#include "sqlite3.h"
#include <inttypes.h> 
#include <math.h>
#include <limits.h>
#include <stdio.h>

void igraph_de_bruijn(database db, int m, int n) {

    //m - number of symbols 
    //n - length of strings 

    long int no_of_nodes, no_of_edges;
    long int i, j;
    long int mm = m;

    no_of_nodes = (long int) pow(m, n);
    no_of_edges = no_of_nodes*m;

    char* errorMessage;
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_stmt *stmt, *stmt_v;
    char buffer[] = "INSERT INTO edge (source, target, cost) VALUES (?1, ?2, 1)";
    char buffer2[] = "INSERT INTO vertex(id, parent, phase) VALUES (?1, -1 , ?2)";

    sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);
	sqlite3_prepare_v2(db, buffer2, strlen(buffer2), &stmt_v, NULL);

    for (i = 0; i < no_of_nodes; i++) {

         sqlite3_bind_int(stmt_v, 1, i);
        sqlite3_bind_int(stmt_v, 2, INT_MAX);
		 if (sqlite3_step(stmt_v) != SQLITE_DONE) {
               printf("Commit Failed inserting vertex [%d]!\n", i);
               //error_flag = false;
               break;
        }
        sqlite3_clear_bindings(stmt_v);
        sqlite3_reset(stmt_v);
        long int basis = (i * mm) % no_of_nodes;
        for (j = 0; j < m; j++) {
	
            sqlite3_bind_int(stmt, 1, i);
            sqlite3_bind_int(stmt, 2, basis + j);
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                printf("Commit Failed!\n");

            }

            sqlite3_clear_bindings(stmt);
            sqlite3_reset(stmt);
        }
        // sqlite3_reset(stmt);
    }

    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);


}


void gera_grafo(database db, int n, double dens) {

    int i, j;
    unsigned long edge;
    char* errorMessage;
    bool error_flag = false;

    //char *szSQL = "insert into edge (source, target, cost) values (?,?, 1)";
    sqlite3_stmt *stmt, *stmt_v;
    char buffer[] = "INSERT INTO edge (source, target, cost) VALUES (?1, ?2, 1)";
    char buffer2[] = "INSERT INTO vertex(id, parent, phase) VALUES (?1, -1 , ?2)";

    sqlite3_prepare_v2(db, buffer, strlen(buffer) + 1, &stmt, NULL);
    sqlite3_prepare_v2(db, buffer2, strlen(buffer2) + 1, &stmt_v, NULL);
    printf("\n\n Gerando Grafo Aleatorio de Densidade: %f #Vertices: %d\n\n", dens, n);
    edge = 0;
    srand((unsigned) time(NULL));
    

    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    for (i = 1; i <= n; i++) {
        
        sqlite3_bind_int(stmt_v, 1, i);
        sqlite3_bind_int(stmt_v, 2, INT_MAX);

        if (sqlite3_step(stmt_v) != SQLITE_DONE) {
               printf("Commit Failed inserting vertex [%d]!\n", i);
               error_flag = false;
               break;
        }
        sqlite3_clear_bindings(stmt_v);
        sqlite3_reset(stmt_v);
        for (j = 1; j <= n; j++) {

            double random = (double)rand()/(double)(RAND_MAX);
            if (i != j) {
                if (random < dens) {

                    sqlite3_bind_int(stmt, 1, i);
                    sqlite3_bind_int(stmt, 2, j);
                    edge++;
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        printf("Commit Failed!\n");
                        error_flag = false;
                        break;

                    }

                    //printf("Node i:%d,  --- Node j: %d, Edge: %lu \n", i, j, edge);
                    //sqlite3_clear_bindings(stmt);
                    sqlite3_reset(stmt);
                }

            }
            
            
        }

        //sqlite3_reset(stmt);
        //        sql_stmt("commit");



    }

    if(error_flag)
      sqlite3_exec(db, "ROLLBACK TRANSACTION", NULL, NULL, &errorMessage);
    else    
      sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    //sqlite3_reset(stmt_v);
    //sqlite3_reset(stmt);
    sqlite3_finalize(stmt_v);
    sqlite3_finalize(stmt);

}