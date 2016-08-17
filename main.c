#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "sqlite3.h"
#include "utarray.h"
#include "uthash.h"
#include "type.h"
#include "bfs_db.h"
#include "bfs_algorithm.h"
#include "generator.h"
//#include "queries.h"

/*
 -o  i import from file
 -o  r random
 -o  B deBruijn graph
*/

struct _globalArgs {
	char* vertex_Filename;				/* -o = i -v option */
	char *edge_Filename;				/* -o = i -e option */
	char *database_Filename;	/* -o = i -b option */
    float dens;   /* -o = r and -d */
    //const int nodes;   /* -o = r and -n */
    int m;  /* -o = B and -m */ //number of symbols
    int n; /* -o = B  and -n length of string  -o = r and -n number of nodes*/
    char *op; /* -o */

} globalArgs;

 //m - number of symbols 
    //n - length of strings

void convert_document( void )
{
	printf("Vertex %s\n",globalArgs.vertex_Filename);
	printf("Edge %s\n",globalArgs.edge_Filename);
	printf("Data %s\n",globalArgs.database_Filename);
    printf("Data %f\n",globalArgs.dens);
    printf("Data %d\n",globalArgs.m);
    printf("Data %d\n",globalArgs.n);
    printf("Data %s\n",globalArgs.op);
    
}

static const char *optString = "o:v:e:d:n:m:b:";


pair * create_intervals(int64_t num, int n_int){
		pair *temp;
		temp = malloc(n_int * sizeof(pair));
		uint64_t pass;
        pass = num/(int64_t)n_int;
		int i, begin = 1;
		
		for(i = 0; i < n_int; i++){
				
				pair t;
				if(i < n_int - 1){
						t.a = begin;
						t.b = t.a + pass - 1;
						begin += pass;
					}
				else{
					 
					t.a = begin;
					t.b = num;
					
					}	
				temp[i] = t;
			}
			
			return temp;
	}

void empty_hash(Vertex **vertices){

    Vertex *v, *tmp;
    HASH_ITER(hh, *vertices, v, tmp) {
        HASH_DEL(*vertices, v);
        free(v);
    }
}

void display_usage( void ){
	printf( "-v - vertex file name\n" );
    printf( "-e - edge file name\n" );
    printf( "-b - database file name\n" );
	/* ... */
	exit( EXIT_FAILURE );
}



int main(int argc, char** argv) {

    clock_t begin, end;
    double time_spent = 0.0;
    //FILE* fd = NULL;
    FILE* fd = NULL;
    time_t start_t, end_t;
	double diff_t;
    database db = NULL;
    //globalArgs g_args;
    
    int opt = 0;
	
	/* Initialize globalArgs before we get to work. */
	 globalArgs.vertex_Filename = NULL;		/* false */
	 globalArgs.edge_Filename = NULL;
	 globalArgs.database_Filename = NULL;
     globalArgs.dens = 0.0;   /* -o = r and -d */
     globalArgs.m = 0;  /* -o = B and -m */ //number of symbols
     globalArgs.n = 0; /* -o = B  and -n length of string  -o = r and -n number of nodes*/
     globalArgs.op = NULL;
	
	
	/* Process the arguments with getopt(), then 
	 * populate globalArgs. 
	 */
	//opt = getopt( argc, argv, optString );
	while( (opt = getopt (argc, argv, optString)) != -1 ) {
		switch( opt ) {
			case 'o':
                 globalArgs.op = strdup(optarg);
                break;
            case 'd':
                 globalArgs.dens = atof(optarg);
                break;
            case 'm':
                 globalArgs.m = atoi(optarg);
                break;
            case 'n':
                 globalArgs.n = atoi(optarg);
                break;            
            case 'v':
				 globalArgs.vertex_Filename = strdup(optarg);	/* true */
				break;
			case 'e':
				 globalArgs.edge_Filename = strdup(optarg);
				break;
				
			case 'b':
				/* This generates an "assignment from
				 * incompatible pointer type" warning that
				 * you can safely ignore.
				 */
				 globalArgs.database_Filename = strdup(optarg);
				break;
			
				
			case 'h':	/* fall-through is intentional */
			case '?':
				display_usage();
				break;
				
			default:
				/* You won't actually get here. */
				break;
		}
		
		//opt = getopt( argc, argv, optString );
	}
   

    /********** INIT ****************/
   convert_document();
   create_database(&db,  "../grafo.db");
    if (db == NULL) {
        printf("Could not open database.");
        return 1;
    }

    sql_stmt(db, "PRAGMA synchronous=OFF", NULL, NULL);
    sql_stmt(db, "PRAGMA temp_store = MEMORY", NULL, NULL);
    sql_stmt(db, "PRAGMA JOURNAL_MODE=OFF", NULL, NULL);
   // sql_stmt(db, "PRAGMA LOCKING_MODE=EXCLUSIVE", NULL, NULL);
    sql_stmt(db, "PRAGMA cache_size=10000", NULL, NULL);
    sql_stmt(db, "PRAGMA mmap_size=268435456", NULL, NULL);


   // create table if not exists edge (source integer, target integer, cost integer,  primary key (source,target))
    /************ CREATE TABLES *********/
    char *sql_create_edge = "create table if not exists edge" 
                            "(source integer, target integer, cost integer)" ;

    char *sql_create_vertex = "create table if not exists vertex" 
                            "(id integer , parent integer, phase integer)";

   char *sql_create_update = "create table update_table" 
                            "(id integer, parent integer not null, account integer)";

    sql_stmt(db, sql_create_edge, NULL, NULL);
   
    sql_stmt(db, sql_create_update, NULL, NULL);
    sql_stmt(db, sql_create_vertex, NULL, NULL);
    
   

    /********** END INIT ****************/

    /********** LOAD DATA ***************/
   
    int m, n;
    int64_t nodes;
    nodes = m = n = 0;

     begin = clock();

     //if(!strcmp(globalArgs.op, "r"))
      //      gera_grafo(db,  globalArgs.n, globalArgs.m);
     //else if(!strcmp(globalArgs.op, "B"))
     //    igraph_de_bruijn(db, globalArgs.m,  globalArgs.n);
     //else 
       nodes = import_from_txt_file(db,  "/home/julio/vscode/db_xstream/out_v.txt",  "/home/julio/vscode/db_xstream/out.txt");
   
    // printf("Nodos y densidad: ");
     //scanf("%d,%f",&nodes,&dens);
     
     //gera_grafo(db, nodes,dens);
    
  /* printf("M,N: ");
   scanf("%d,%d", &m, &n);

   igraph_de_bruijn(db,m,n);*/

   
    
   //sql_stmt(db,"create unique index unique_edge_index on edge (source,target)", NULL,NULL);
   sql_stmt(db,"create unique index unique_vertex_index on vertex (id)", NULL,NULL);
   //sql_stmt(db,"vacuum", NULL,NULL);
  /* sql_stmt(db,"create view graph as select id_node, source, target, visit \ 
 				from edge_table join node_table on edge_table.target = node_table.id_node");*/

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
    //sql_stmt(db,"create unique index unique_edge_index on edge (source,target)", NULL, NULL);
   // sql_stmt(db,"create unique index unique_vertex_index on vertex (id)", NULL, NULL);
    
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
    bool global_execution = true;

    /******************** CREATE INTERVALS ******************/

     int number_of_intervals, i;  
     printf("Number of intervals: ");
     scanf("%d",&number_of_intervals);
    pair * intervals = create_intervals(nodes, number_of_intervals);
    
    
    /************ INIT SOME STRUCTS ***********************/
    int64_t phase = 0;
    UT_array *updates = NULL;
    UT_icd update_icd = {sizeof(Update), NULL, NULL, NULL};
    utarray_new(updates,&update_icd);
    
   // Vertex *vertices = NULL;


    /*************** REAL STUFF ****************************/
    init_alg(db,1);

    //bool gather_execution = true;

    while(global_execution){

        global_execution = false;
        phase += 1;
        //scatter

        sql_stmt(db,"DELETE FROM update_table", NULL, NULL);
        
        for(i = 0; i < number_of_intervals; i++){
             //load vertices

             //sql_stmt_prepare_vertex(db, &vertices, intervals[i].a, intervals[i].b);
             
             //unsigned int num_vertices;
             //num_vertices = HASH_COUNT(vertices);
            // printf("there are %u vertices interval \n", num_vertices);
             
              scatter_v2(db, intervals[i].a, intervals[i].b, phase, updates);

              if(utarray_len(updates) > 0)
                    global_execution = true;

             //empty_hash(&vertices);

            
        }

        if(global_execution){
            Update *p;
            for(p=(Update *)utarray_front(updates); p!=NULL; p=(Update *)utarray_next(updates,p)) {
               printf("id: [""%"PRId64"], parent: [""%"PRId64"], account [""%"PRId64"]\n",p->id, p->parent, p->account);
            }
            apply_update(db,updates);
            gather(db);
        }
            
           
        
    }
    
    end = clock();
    time(&end_t);
    
    diff_t = difftime(end_t, start_t);
    
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime spent in bfs %.3f \n",time_spent);
    printf("Time2 spent in bfs %.3lf\n", diff_t);
	fprintf(fd, "Time spent in bfs %.3f\n", time_spent);
	fprintf(fd, "Time2 spent in bfs %.3lf\n", diff_t);
	
    /*************************** END ALGORITHM *********************/
	fclose(fd);
    destroy_database(db);
    return (EXIT_SUCCESS);
}

