/* 
 * File:   main.c
 * Author: julio
 *
 * Created on October 12, 2015, 5:04 PM
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sqlite3.h"
#include "utarray.h"
//#include "sqlite3e.h"

/*i
 * 
 */

sqlite3* db;
int nodes_cant;

typedef struct edge {
    int node;
    struct edge *next;
} edge;



int first_row;

double randomn(void)
// random number between 0.0 and 1.0 (uncluded).
{
    double rrr;

    rrr = (double) rand() / (double) RAND_MAX;
    return rrr;
}

int select_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {

    int i;
    int *p_rn = (int*) p_data;

    if (first_row) {
        first_row = 0;

        for (i = 0; i < num_fields; i++) {
            printf("%20s", p_col_names[i]);
        }
        printf("\n");
        for (i = 0; i < num_fields * 20; i++) {
            printf("=");
        }
        printf("\n");
    }

    (*p_rn)++;

    for (i = 0; i < num_fields; i++) {
        printf("%20s", p_fields[i]);
    }

    printf("\n");
    return 0;
}

void select_stmt(const char* stmt) {
    char *errmsg;
    int ret;
    int nrecs = 0;

    first_row = 1;

    ret = sqlite3_exec(db, stmt, select_callback, &nrecs, &errmsg);

    if (ret != SQLITE_OK) {
        printf("Error in select statement %s [%s].\n", stmt, errmsg);
    } else {
        printf("\n   %d records returned.\n", nrecs);
    }
}

void sql_stmt(const char* stmt) {
    char *errmsg;
    int ret;

    ret = sqlite3_exec(db, stmt, 0, 0, &errmsg);

    if (ret != SQLITE_OK) {
        printf("Error in statement: %s [%s].\n", stmt, errmsg);
    }
    //sqlite3_finalize(stmt);
}

void insertParamSQL(int node) {
    if (!db)
        return;

    char *zErrMsg = 0;
    sqlite3_stmt *stmt;
    const char *pzTest;
    char *szSQL;

    // Insert data item into myTable
    szSQL = "insert into node_table (id_node,visit) values (?,0)";

    int rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);

    if (rc == SQLITE_OK) {
        // bind the value
        sqlite3_bind_int(stmt, 1, node);


        // commit
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    } else {
        printf("NOOOO");
    }
}

void gera_grafo(int n, double dens) {

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

    /* m - number of symbols */
    /* n - length of strings */

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


}

/*
 create table
 * update_table  UPDATE table_name SET column1 = value1, column2 = value2...., columnN = valueN
 * select table
 * 
 */



void visit(int *r) {
    char* errorMessage;
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_stmt *stmt;
    char buffer[] = "update  node_table set visit = 1 where id_node = ?";
    sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);
    sqlite3_bind_int(stmt, 1, *r);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Commit Failed!\n");
        sqlite3_free(errorMessage);

    }
    printf("Node visited %d  Cantidad: %d\n", *r, ++nodes_cant);
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);

}

void visit_sql_stmt(const char* stmt) {
    char *errmsg;
    int ret;
    int nrecs = 0;

    first_row = 1;

    ret = sqlite3_exec(db, stmt, -1, &nrecs, &errmsg);

    if (ret != SQLITE_OK) {
        printf("Error in select statement %s [%s].\n", stmt, errmsg);
    } else {
        printf("\n   %d records returned.\n", nrecs);
    }
}

int isVisit(int r) {

    char* errorMessage;
    int retval, value = -1;
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_stmt *stmt;
    char buffer[] = "select visit from node_table where id_node = ?";


    printf("Buffer : %s \n", buffer);

    // char str[15];
    // sprintf(str, "%d", r);
    // strcat(buffer, str);

    printf("Buffer : %s \n", buffer);

    retval = sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);
    sqlite3_bind_int(stmt, 1, r);

    while (1) {
        retval = sqlite3_step(stmt);
        if (retval == SQLITE_ROW) {
            value = sqlite3_column_int(stmt, 0);

        } else if (retval == SQLITE_DONE) {
            break;
        } else {
            sqlite3_finalize(stmt);
            printf("Some error encountered\n");
            break;
        }

    }
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);

    return value;

}

UT_array * adyacent(int r) {
    UT_array * result = NULL;
    utarray_new(result, &ut_int_icd);

    char* errorMessage;
    int retval, value;
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_stmt *stmt;
    //char buffer[] = "select target from edge_table join node_table on edge_table.source = node_table.id_node where node_table.id_node = ? and node_table.visit = 0";
    //char buffer[] = "select id_node from edge_table join node_table on edge_table.target = node_table.id_node where source = ? and visit = 0";
    char buffer[] = "select id_node from graph where source = ? and visit = 0";

    retval = sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);
    sqlite3_bind_int(stmt, 1, r);

    while (1) {
        retval = sqlite3_step(stmt);

        if (retval == SQLITE_ROW) {
            value = (int) sqlite3_column_int(stmt, 0);
            utarray_push_back(result, &value);
           // printf("Adya Node %d, Value : %d \n", r, value);


        } else if (retval == SQLITE_DONE) {
            break;
        } else {
            sqlite3_finalize(stmt);
            printf("Some error encountered\n");
            break;
        }

    }

    /*int *p;
    printf("\n UT_ARRAY \n");
    for(p=(int*)utarray_front(result);
      p!=NULL;
      p=(int*)utarray_next(result,p)) {
    printf("%d\n",*p);
  }*/
    sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
    sqlite3_finalize(stmt);
    if (utarray_len(result) == 1) utarray_clear(result);
    return result;
}

/*visit(start node) 
queue <- start node 
WHILE queue is nor empty DO 
  x <- queue 
  FOR each y such that (x,y) is an edge 
                  and y has not been visited yet DO 
    visit(y) 
    queue <- y 
  END 
END */

void bfs(int r) {

    int *p;
    UT_array *queue;
    utarray_new(queue, &ut_int_icd);

    utarray_push_back(queue, &r);
    visit(&r);

    while (utarray_len(queue) > 0) {

        int *i = (int*) utarray_front(queue);
        //printf("To visit %d\n", *i);
       // utarray_erase(queue, 0, 1);
        //visit(i);

        UT_array *temp = adyacent(*i);

        for (p = (int*) utarray_front(temp);
                p != NULL;
                p = (int*) utarray_next(temp, p)) {
            //printf("%d\n", *p);
            visit(p);
            utarray_push_back(queue,p);
        }
        utarray_erase(queue, 0, 1);
        /*if (temp) {
            utarray_concat(queue, temp);
        }*/

        //

    }
}

int main(int argc, char** argv) {

    clock_t begin, end;
    double time_spent = 0.0;
    FILE* fd = NULL;
    time_t start_t, end_t;
	double diff_t;
    


    sqlite3_open("./grafo2.db", &db);

    if (db == 0) {
        printf("Could not open database.");
        return 1;
    }

    //sql_stmt("create table if not exists edge_table (source integer not null, target integer not null,  PRIMARY KEY (source, target))");
   
    
    sql_stmt("create table if not exists edge_table (source integer not null, target integer not null)");
    sql_stmt("create table if not exists node_table (id_node integer primary key, visit INTEGER DEFAULT (0))");
    sql_stmt("delete from edge_table");
    sql_stmt("delete from node_table");
    sql_stmt("vacuum");

   
    int nodes, m, n, init;
    float dens;

     begin = clock();
     //printf("Nodos y densidad: ");
    //scanf("%d,%f",&nodes,&dens);
    //gera_grafo(nodes,dens);
    
   printf("M,N: ");
   scanf("%d,%d", &m, &n);

    igraph_de_bruijn(m,n);
    
   sql_stmt("create unique index unique_edge_index on edge_table (source,target)");
   sql_stmt("vacuum");
   sql_stmt("create view graph as select id_node, source, target, visit \ 
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
    select_stmt("select count(*) from edge_table");
    //select_stmt("select * from edge_table");

    printf("\nNumber of nodes\n\n");
    select_stmt("select count(*) from node_table");
    //select_stmt("select * from node_table");

    //printf("\nVisit nodes\n\n");
    //int a = isVisit(4);
    //printf("Visit :%d \n",a);

    begin = clock();
    time(&start_t);
    
    //printf("\nBFS\n\n");
    //scanf("%d", &init);
    init = 1;
    bfs(init);
    
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

