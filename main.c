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



//database db;


/*void igraph_de_bruijn(int m, int n) {

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
    database db = NULL;
    
    
   
/*HASH_ITER(hh, users, s, tmp) {
        HASH_DEL(users, s);
        free(s);
    }*/

    /********** INIT ****************/
    
    create_database(&db, "./grafo2.db");
    if (db == NULL) {
        printf("Could not open database.");
        return 1;
    }

    /*sql_stmt(db, "PRAGMA synchronous=OFF", NULL, NULL);
    sql_stmt(db, "PRAGMA temp_store = MEMORY", NULL, NULL);
    sql_stmt(db, "PRAGMA JOURNAL_MODE=OFF", NULL, NULL);
    sql_stmt(db, "PRAGMA LOCKING_MODE=EXCLUSIVE", NULL, NULL);
    sql_stmt(db, "PRAGMA cache_size=10000", NULL, NULL);
    sql_stmt(db, "PRAGMA mmap_size=268435456", NULL, NULL);*/


    
    /************ CREATE TABLES *********/
    char *sql_create_edge = "create table if not exists edge" 
                            "(source integer not null, target integer not null, cost integer not null)"
                            "WITHOUT ROWID";

    char *sql_create_vertex = "create table if not exists vertex" 
                            "(id integer not null, parent integer not null, phase integer not null)"
                            "WITHOUT ROWID";

   char *sql_create_update = "create temporary table update_table" 
                            "(target integer not null, parent integer not null, phase integer not null)"
                            "WITHOUT ROWID";

    sql_stmt(db, sql_create_edge, NULL, NULL);
   
    sql_stmt(db, sql_create_update, NULL, NULL);
    sql_stmt(db, sql_create_vertex, NULL, NULL);
    
    //sql_stmt(db,"truncate from edge_table", NULL, NULL);
    //sql_stmt(db,"truncate from node_table", NULL, NULL);
    //sql_stmt(db,"vacuum");

    /********** END INIT ****************/

    /********** LOAD DATA ***************/
   
    int nodes, m, n, init;
    float dens;

     begin = clock();
     printf("Nodos y densidad: ");
     scanf("%d,%f",&nodes,&dens);
     
     gera_grafo(db, nodes,dens);
    
   //printf("M,N: ");
   //scanf("%d,%d", &m, &n);

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
    
    /********************** CREATE INDEX ***************************/
    sql_stmt(db,"create unique index unique_edge_index on edge (source,target)", NULL, NULL);
    sql_stmt(db,"create unique index unique_vertex_index on vertex (id)", NULL, NULL);
    
    //printf("\nNumber of edges [%U]\n\n");
    
    //sql_stmt("select count(*) from edge", NULL, NULL);
    

    //printf("\nNumber of nodes\n\n");
   // select_stmt("select count(*) from node_table");
    //select_stmt("select * from node_table");

    //printf("\nVisit nodes\n\n");
    //int a = isVisit(4);
    //printf("Visit :%d \n",a);
    /********************* END LOAD DATA ************************/

    /********************* ALGORITHM  **************************/
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
	
    /*************************** END ALGORITHM *********************/
	fclose(fd);
    destroy_database(db);
    return (EXIT_SUCCESS);
}