#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sqlite3.h"
#include "utarray.h"
#include "uthash.h"
#include "type.h"
#include "bfs_db.h"
#include "bfs_algorithm.h"

database db;
/*void gera_grafo(int n, double dens) {

    int i, j;
    unsigned long edge;
    char* errorMessage;

    //char *szSQL = "insert into edge_table (source, target) values (?,?)";
    sqlite3_stmt *stmt;
    char buffer[] = "INSERT INTO edge_table VALUES (?1, ?2)";
    sqlite3_prepare_v2(db, buffer, strlen(buffer) + 1, &stmt, NULL);

    printf("\n\n Gerando Grafo Aleatorio de Densidade: %f #Vertices: %d\n\n", dens, n);
    edge = 0;
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    for (i = 1; i <= n; i++) {
        insertParamSQL(i);
        for (j = 1; j <= n; j++) {


            if (i != j) {
                if (randomn() < dens) {

                    sqlite3_bind_int(stmt, 1, i);
                    sqlite3_bind_int(stmt, 2, j);
                    edge++;
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        printf("Commit Failed!\n");

                    }

                    //printf("Node i:%d,  --- Node j: %d, Edge: %lu \n", i, j, edge);
                    sqlite3_clear_bindings(stmt);
                    sqlite3_reset(stmt);
                }


                //sql_stmt("commit");

            }
        }

        sqlite3_reset(stmt);
        //        sql_stmt("commit");



    }

    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);

}

void igraph_de_bruijn(int m, int n) {

    //m - number of symbols 
    //n - length of strings 

    long int no_of_nodes, no_of_edges;
    long int i, j;
    long int mm = m;

    no_of_nodes = (long int) pow(m, n);
    no_of_edges = no_of_nodes*m;

    char* errorMessage;
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_stmt *stmt;
    char buffer[] = "INSERT INTO edge_table VALUES (?1, ?2)";

    sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);

    for (i = 0; i < no_of_nodes; i++) {

        insertParamSQL(i);

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


}*/

int main(int argc, char** argv) {

    clock_t begin, end;
    double time_spent = 0.0;
    //FILE* fd = NULL;
    FILE* fd = NULL;
    time_t start_t, end_t;
	double diff_t;
    
/*HASH_ITER(hh, users, s, tmp) {
        HASH_DEL(users, s);
        free(s);
    }*/

    sqlite3_open("./grafo2.db", &db);

    if (db == 0) {
        printf("Could not open database.");
        return 1;
    }

    //sql_stmt("create table if not exists edge_table (source integer not null, target integer not null,  PRIMARY KEY (source, target))");
   
    
    //sql_stmt(db,"create table if not exists edge_table (source integer not null, target integer not null)");
    //sql_stmt(db,"create table if not exists node_table (id_node integer primary key, visit INTEGER DEFAULT (0))");
    //sql_stmt(db,"delete from edge_table");
    //sql_stmt(db,"delete from node_table");
    //sql_stmt(db,"vacuum");

   
    int nodes, m, n, init;
    float dens;

     begin = clock();
     //printf("Nodos y densidad: ");
    //scanf("%d,%f",&nodes,&dens);
    //gera_grafo(nodes,dens);
    
   printf("M,N: ");
   scanf("%d,%d", &m, &n);

    //igraph_de_bruijn(m,n);
    
  // sql_stmt(db,"create unique index unique_edge_index on edge_table (source,target)");
  // sql_stmt(db,"vacuum");
  // sql_stmt(db,"create view graph as select id_node, source, target, visit \ 
				from edge_table join node_table on edge_table.target = node_table.id_node");

    // insert_countries();

      end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime spent in graph creation %.3f",time_spent);
    
    fd = fopen("time.txt","w");
    if(NULL == fd)
    {
        printf("\n fopen() Error!!!\n");
        return 1;
    }
    
    fprintf(fd, "Time spent in graph creation: %.3f\n", time_spent);
    printf("\nNumber of edges\n\n");
    //select_stmt("select count(*) from edge_table");
    //select_stmt("select * from edge_table");

    printf("\nNumber of nodes\n\n");
   // select_stmt("select count(*) from node_table");
    //select_stmt("select * from node_table");

    //printf("\nVisit nodes\n\n");
    //int a = isVisit(4);
    //printf("Visit :%d \n",a);

    begin = clock();
    time(&start_t);
    
    //printf("\nBFS\n\n");
    //scanf("%d", &init);
    init = 1;
    //bfs(init);
    
    end = clock();
    time(&end_t);
    
    diff_t = difftime(end_t, start_t);
    
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime spent in bfs %.3f \n",time_spent);
    printf("Time2 spent in bfs %.3lf\n", diff_t);
	printf(fd, "Time spent in bfs %.3f\n", time_spent);
	printf(fd, "Time2 spent in bfs %.3lf\n", diff_t);
	
	fclose(fd);
    sqlite3_close(db);
    return (EXIT_SUCCESS);
}