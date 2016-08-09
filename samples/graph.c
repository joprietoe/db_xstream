
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include <conio.h>
#include <sys/types.h>

// #include <iostream.h>
// #include <iomanip.h>
#include <time.h>     // for seconds()

#define MaxDim  1000
#define Rand_Max  100000

// Prototypes

int find( int );
void ffunion( int , int );
void seedRandom(unsigned int );
double randomn(void );
double seconds(void );

void gera_grafo(int , double);
void gera_grafo_sim(int , double);
void print_grafo(int );
void print_listas(int );
void constroi_lista(int );
void alcance(int, int);
void print_alcance(void);
void init_buscaP(void);
void buscaP(int );
void init_buscaP_Color(int );
void buscaP_Color(int );
void print_alc_buscaP(int );
void FordBellman(int );
void Prim(int );
void Dijkstra(int );
void Kruskal(void  );
void Boruvka(int );

void AGM_buscaP( int , int );
void print_listas_AGM_B(int , int , int* );
void print_grafo_arquivo(int );
void eClique(void );


// Variaveis Globais

int MAdj[MaxDim][MaxDim];
int Dist[MaxDim][MaxDim];

int LisAdjP[MaxDim][MaxDim];
int LisAdjN[MaxDim][MaxDim];
int CardP[MaxDim];
int CardN[MaxDim];

int M_arcos; // Numero de Arcos no Grafo
int I_arco[MaxDim*MaxDim]; //  I_arco[k] Inicio(Origem) do Arco k
int J_arco[MaxDim*MaxDim]; //  J_arco[k] Fim(Destino) do Arco k

int Hq, Tq;
int Dim;

int PosPilha[MaxDim];
int PosTopol[MaxDim];
int IndTopol;

int Q[MaxDim];
int mark[MaxDim];

int DistInfinita;

int AGM_LisAdjP[MaxDim][MaxDim];
int AGM_CardP[MaxDim];

int _MaxColor;
int _CorUsada[MaxDim];

// Programa Principal

int main()
{
	int v,a,i,min;
	float  faux;
	double dens;

	printf(" Digite o numero de vertices no grafo: ");
	scanf("%d", &Dim);

	printf(" Digite a densidade o grafo: ");
	scanf("%f", &faux);

	dens = faux;


	// Geracao do Grafo

	gera_grafo(Dim, dens);

	constroi_lista (Dim);
    
///    print_grafo_arquivo(Dim);

    print_listas(Dim);
   
#if 0
    min=Dim;
    for(v=1; v<=Dim; v++)
    {
        for(i=1; i<=3; i++)
        {
        init_buscaP_Color(1);
        buscaP_Color(1);
        if(_MaxColor<min)min = _MaxColor;
        printf(" \n MaxColor: %d  min: %d\n", _MaxColor, min );
        }
    }
#endif
    
	// Imprime Grafo

	print_grafo(Dim);


	printf("Tecle <enter>");
	getchar();

	print_listas(Dim);


	printf("Digite o numero do vertice para saber seu alcance: ");
	scanf("%d",&v);


	printf(" Alcance (Busca em Largura) \n");
    alcance(v, Dim);

	printf("Tecle <enter>");
	getchar();


    init_buscaP ();
    for (v=1; v<=Dim; v++)
    {
        if ( mark[v] == 0 )
        {
            mark[v] = 1;
            buscaP ( v );
        }
    }

	printf("Tecle <enter>");
	getchar();


    exit(123);
/*    
    for (v=1; v<=Dim; v++)
    {
     	printf("PosTopol[%d]=%d\n",v, PosTopol[v]);
    }
*/
    
	printf("Tecle <enter>");
	getchar();


//	printf(" Boruvka \n");
//    Boruvka( Dim );

//	printf("Tecle <enter>");
//	getch();


//	printf(" CMC de %d aos demais vertice via ALg. Ford & Belman  \n", v);
     v=a;
//    FordBellman(v);
//	printf("Tecle <enter>");
//	getch();  

//    Kruskal();
  //  Dijkstra(v);
//	printf("Tecle <enter>");
//	getch();  
//    Prim(v); 

	printf("Tecle <enter>");
	getchar();



/*
    v = 1;
	printf(" Kruskal   \n");

	Kruskal();
	printf(" Prim   \n");

	Prim(v);
	getch();

    v = 1;
    init_buscaP();
	buscaP(v);
	print_alc_buscaP(v);

	alcance(v,Dim);
	print_alcance();
*/
	return(0);
}

//
// Algoritmo de Enumeracao - BackTrack
//

void enum_a (int level, int max_level)
{
     if(level == max_level)
     {
     }
     else
     {
         
     }
}

//
// Algoritmo de Boruvka para Arvore Geradora Minima
//


void Boruvka(int n)
{
     int i, j, p, ii, jj, iter, n_grupos;
     int grupo[MaxDim], minDist[MaxDim], minArco[MaxDim];
     int arco_usado[MaxDim*MaxDim];
     
     
     printf("\n Boruvka \n n=%d M_arcos=%d \n", n, M_arcos);
     for(i=1; i<=n; i++)
     {
       grupo[i] = i;
       AGM_CardP[i] = 0;
     }
     for(i=1; i<=M_arcos; i++)
     {
       arco_usado[i] = 0;
     }
     n_grupos = n;

     iter = 0;
     print_listas_AGM_B( n, iter, grupo);
     printf("Tecle <enter>");
	 getchar();

     
     do
     {
       iter++;
                            
       // Inicializa Distancias Minimas para cada grupo
       for(i=1; i<=n_grupos; i++)
       {
         minDist[i]=DistInfinita;
       }
       
       // Encontrar o grupo mais próximo de cada grupo
       // a aresta correspondente e a distancia associada 
       for(j=1; j<=M_arcos; j++)
       {
          ii = I_arco[j]; jj = J_arco[j];
          if (grupo[ii] != grupo[jj])
          {
             if ( Dist[ii][jj] < minDist[grupo[ii]])
             {
                  minDist[grupo[ii]] = Dist[ii][jj];
                  minArco[grupo[ii]] = j;
             }
             if ( Dist[ii][jj] < minDist[grupo[jj]])
             {
                  minDist[grupo[jj]] = Dist[ii][jj];
                  minArco[grupo[jj]] = j;
             }
          }
       }
       // Insere as aresta nas arvore geradora minima AGM
       for (ii=1; ii<=n_grupos; ii++)
       {
           jj = minArco[ii];
           if (arco_usado[jj] == 0) // Se o arco nao foi usado
           {
            arco_usado[jj] = 1;
            
            i = I_arco[jj]; j = J_arco[jj];
			p = AGM_CardP[i] + 1;
			AGM_LisAdjP[i][p] = j;
			AGM_CardP[i] = p;
			p = AGM_CardP[j] + 1;
            AGM_LisAdjP[j][p] = i;
			AGM_CardP[j] = p;
           }
       }
       
       
       // Determina os grupos (componentes conexos) da AGM 
       // (parcial), Se for um soh, a AGM foi obtida.
              
       for (i=1; i<=n; i++)
       {
           mark[i] = 0;
       }
       
       n_grupos = 0;
       for (i=1; i<=n; i++)
       {
           if (mark[i] == 0)
           {
               n_grupos++;
               AGM_buscaP( i, n_grupos );
           }
       }       
       
       for (i=1; i<=n; i++)
       {
           grupo[i] = mark[i];
       }

       print_listas_AGM_B( n, iter, grupo);
	   printf("Tecle <enter>");
	   getchar();
       
     }
     while (n_grupos > 1);
}


//
//   Busca em Profundidade na Floresta Construida
//   ao longo do Algoritmo de Boruvka
//

void AGM_buscaP( int v, int rotulo)
{
     int card, w, i;
     
     card = AGM_CardP[v];
     for (i=1; i<=card; i++)
     {
		 w = AGM_LisAdjP[v][i];
         if (mark[w] == 0)
         {
             mark[w] = rotulo;
             AGM_buscaP(w, rotulo);
         }
     }
}

// Print Listas AGM Boruvka

void print_listas_AGM_B(int n, int iter, int grupo[])
{
	int i,j, p;
	printf("\n Boruvka - Floresta Iter %d - Listas de Adjacencia\n", iter);
	for(i=1; i<=n; i++)
	{
		printf("%d | Grupo %d | %d Arcos Saindo : ", i, grupo[i], AGM_CardP[i]);
		p = AGM_CardP[i];
		for(j=1; j<=p; j++)
		{
			printf(" %d ",AGM_LisAdjP[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}




// 
//  Caminho-mais-curto com custos nao negativos
//      Algoritmo de Dijkstra
//

void Dijkstra(int s)
{
	int v,w,k,l, vmin, dmin;
	int d_temp[MaxDim];
	int pred[MaxDim];
	int pos_prox[MaxDim];


    // Inicializacao: d_temp contem um estimativa pessimista
	// da distancia de s a v

	for (v=1; v<=Dim; v++)
	{
		d_temp[v] = DistInfinita;
		pos_prox[v] = 0;
		pred[v] = -1;
	}

    // Caso Base - conhece-se o vértice mais próximo de s
	// (ele mesmo).

	d_temp[s] = 0;
	pred[s] = 0;

	k = 1;
	pos_prox[s] = k;


	// Agora pode-se atualizar as estimativas pessimistas
	// das distancias.

	for(l=1; l<=CardP[s]; l++)
	{
		v = LisAdjP[s][l];
		if (d_temp[v] > d_temp[s]+Dist[s][v])
		{
           d_temp[v] = d_temp[s]+Dist[s][v];
		   pred[v] = s;
		}
	}


    // Passo Indutivo
	// Conhece-se os k vértices mais próximos de s
	// Então conhece-se os k+1 vertices mais próximos de s

    //  O k+1-ésimo mais próximo é o que possui o menor d_temp

    for (k=2; k<=Dim; k++)
	{
       // Encontra o k-ésimo vértice mais próximo de s

	   vmin = 0;
	   dmin = DistInfinita;
       for (v=1; v<=Dim; v++)
	   {
		   if ((d_temp[v] < dmin) && (pos_prox[v] == 0))
		   {
               vmin = v;
               dmin = d_temp[v];
		   }
	   }


       printf(" Vmin: %d  DistMin: %d\n", vmin, dmin);

	   pos_prox[vmin] = k;


	  // Atualiza-se as estimativas pessimistas
	  // das distancias.

   	  for(l=1; l<=CardP[vmin]; l++)
	  {
	 	 v = LisAdjP[vmin][l];
		 if ( (pos_prox[v] == 0) && (d_temp[v] > d_temp[vmin] + Dist[vmin][v])) //d_temp[vmin]+Dist[s][v])
		 {
            d_temp[v] = d_temp[vmin] + Dist[vmin][v]; //d_temp[vmin]+Dist[s][v];
		    pred[v] = vmin;
		 }
	  }

	}

	// Distancias e caminhos mais curtos

	printf(" DIJKSTRA -- CMC a partir de %d \n",s);
	for (v=1; v<=Dim; v++)
	{
		printf(" %d D=%d %d-esimo mais proximo Caminho: ",
			v, d_temp[v], pos_prox[v]);
		w = v;
		while (pred[w] > 0)
		{
			printf(" %d ",pred[w]);
			w = pred[w];
		}
		printf("\n");
	}

	for (v=1; v<=Dim; v++)
	{
			printf("Aresta: %d %d %d\n",v ,pred[v], Dist[v][pred[v]]);
    }

}

//====================================================





void Prim(int s)
{
	int v,w,k,l, vmin, dmin;
	int d_temp[MaxDim];
	int pred[MaxDim];
	int pos_prox[MaxDim];


    // Inicializacao: d_temp contem um estimativa pessimista
	// da distancia de s a v

	for (v=1; v<=Dim; v++)
	{
		d_temp[v] = DistInfinita;
		pos_prox[v] = 0;
		pred[v] = -1;
	}


	d_temp[s] = 0;
	pred[s] = s;

	k = 1;
	pos_prox[s] = k;


	// Agora pode-se atualizar as estimativas pessimistas
	// das distancias.

	for(l=1; l<=CardP[s]; l++)
	{
		v = LisAdjP[s][l];
		if (d_temp[v] > Dist[s][v]) // d_temp[s]+Dist[s][v])
		{
           d_temp[v] = Dist[s][v];//d_temp[s]+Dist[s][v];
		   pred[v] = s;
		}
	}


    for (k=2; k<=Dim; k++)
	{

	   vmin = 0;
	   dmin = DistInfinita;
       for (v=1; v<=Dim; v++)
	   {
		   if ((d_temp[v] < dmin) && (pos_prox[v] == 0))
		   {
               vmin = v;
               dmin = d_temp[v];
		   }
	   }


       printf(" Vmin: %d  DistMin: %d\n", vmin, dmin);

	   pos_prox[vmin] = k;


	  // Atualiza-se as estimativas pessimistas
	  // das distancias.

   	  for(l=1; l<=CardP[vmin]; l++)
	  {
	 	 v = LisAdjP[vmin][l];
		 if ( (pos_prox[v] == 0) && (d_temp[v] > Dist[vmin][v])) //d_temp[vmin]+Dist[s][v])
		 {
            d_temp[v] = Dist[vmin][v]; //d_temp[vmin]+Dist[s][v];
		    pred[v] = vmin;
		 }
	  }

	}

	// Distancias e caminhos mais curtos
/*
	printf(" PRIM - vértice inicial: %d\n",s);
	for (v=1; v<=Dim; v++)
	{
		printf(" %d D=%d %d-esimo mais proximo Caminho: ",
			v, d_temp[v], pos_prox[v]);
		w = v;
		while (pred[w] > 0)
		{
			printf(" %d ",pred[w]);
			w = pred[w];
		}
		printf("\n");
	}
*/
	for (v=1; v<=Dim; v++)
	{
			printf("Aresta: %d %d %d\n",v ,pred[v], Dist[v][pred[v]]);
    }

}

//====================================================

int pont[MaxDim];
int rank[MaxDim];

int find(int v)
{
    if ( pont[v] != v)
    {
         return( find(pont[v]) );
    }
    return(v);
}

void ffunion( int c1, int c2 )
{
   if (rank[c1] < rank[c2])
   {
       pont[c1] = c2;
   }
   else
   {
       pont[c2] = c1;
       if ( rank[c1] == rank[c2] )
       {
            rank[c1]++;
       }
   }
}


void Kruskal( )
{
	int v,w,k, ll;
	int v1, v2, w1, w2, c1, c2;
	int temp;
	int pred[MaxDim];
	int ind_sort[MaxDim*MaxDim];

    // Inicializacao: Caso Base - conhece o CMC de s
	// a todos os outros vertices utilizando no minimo ZERO
	// arcos.

	printf(" Kruskal MST  01 \n");

    // Init Union and Find  ponteiros e ranks

	for (v=1; v<=Dim; v++)
	{
		pont[v] = v;
		rank[v] = 0;
	}

    //  Encontra ordem crescente das arestas
	printf(" Kruskal MST 02 \n");

    for (k=1; k<=M_arcos; k++)
	{
        ind_sort[k] = k;
    }
    for (ll=1; ll<=M_arcos; ll++)
	{
		for(k=1; k<=M_arcos-1; k++)
		{
            v1 = I_arco[ind_sort[k]];
            w1 = J_arco[ind_sort[k]];
            v2 = I_arco[ind_sort[k+1]];
            w2 = J_arco[ind_sort[k+1]];
			if( Dist[v1][w1] > Dist[v2][w2] )
			{
                temp = ind_sort[k];
                ind_sort[k] = ind_sort[k+1];
                ind_sort[k+1] = temp;
			}
		}
	}

	// Distancias e caminhos mais curtos
	printf(" Kruskal MST \n");

    for (k=1; k<=M_arcos - 1; k++)
	{
         v = I_arco[ind_sort[k]];
         w = J_arco[ind_sort[k]];
         printf(" A %d  %d  %d\n", v, w, Dist[v][w]);


         c1 = find(v);
         c2 = find(w);

         if (c1 != c2)
         {
                printf(" Aresta %d  %d  %d\n", v, w, Dist[v][w]);
                ffunion(c1,c2);
         }
     }

}

// Algoritmo de FordBellman para caminho mais curto

void FordBellman(int s)
{
	int v,w,k, ll;
	int d_temp[MaxDim];
	int pred[MaxDim];
	int circ_neg;

	circ_neg = 0;

    // Inicializacao: Caso Base - conhece o CMC de s
	// a todos os outros vertices utilizando no minimo ZERO
	// arcos.

	for (v=1; v<=Dim; v++)
	{
		d_temp[v] = DistInfinita;
		pred[v] = -1;
	}

	d_temp[s] = 0;
	pred[s] = -1;


    // Passo Indutivo
	// Conhece-se caminho mais curto de s a um vertice q utilizando
	// no maximo k-1 arcos.
	// Para obter o cmc de s a q utilizando no maximo k arcos,
	// testa-se para cada arco no grafo (v,w) se d_temp(v) + Dist[v][w]
	// e' menor que d_temp[w], em caso afirmativo o atual cmc de
	// s a w nao e' um cmc e portanto atualizamos d_temp[w] e pred[w]

    for (k=1; k<=Dim - 1; k++)
	{
        printf("ITER k= %d\n",k);

		for(ll=1; ll<=M_arcos; ll++)
		{
			v = I_arco[ll];
			w = J_arco[ll];
			if( d_temp[ v ] +  Dist[v][w] < d_temp[w] )
			{
				d_temp[w] = d_temp[ v ] +  Dist[v][w];
				pred[w] = v;
			}
		}

        /* Imprime dist temps atualizadas */
	    for (v=1; v<=Dim; v++)
        {
		   printf(" %d D=%d Caminho: ",v, d_temp[v]);
/*		   w = v;
		   do
		   {
			  printf(" %d | %d ||", d_temp[w], pred[w]);
			  w = pred[w];
        getch();
	       }
		   while ((w != v)&& (pred[w] > 0)); */
	       printf("\n");
        }


	}

        getchar();

	// Testa se existe circuito negativo

	for(ll=1; ll<=M_arcos; ll++)
	{
			v = I_arco[ll];
			w = J_arco[ll];
			if( d_temp[ v ] +  Dist[v][w] < d_temp[w] )
			{
				d_temp[w] = d_temp[ v ] +  Dist[v][w];
				pred[w] = v;
				printf(" CIRCUITO NEGATIVO DETECTADO\n");
				circ_neg=1;
				w=v;
				printf(" %d ",w);
				do{
					w = pred[w];
                    printf("dep v=%d w=%d \n",pred[w],w);
                    getchar();
				}
				while(w != v);
				printf(" \n\n ");

			}
	}

	
	// Distancias e caminhos mais curtos


    if(circ_neg == 0)
	{
	printf(" Ford Bellman -- CMC a partir de %d \n",s);
	for (v=1; v<=Dim; v++)
	{
		printf(" %d D=%d Caminho: ",v, d_temp[v]);
		w = v;
		while (pred[w] > 0)
		{
			printf(" %d ",pred[w]);
			w = pred[w];
		}
		printf("\n");
	}
	}

}

// Print Alcance via busca em profundidade

void print_alc_buscaP(int v)
{
	int i;
		printf("\n Vertices alcancaveis a partir de %d \n",v);

		for (i=1;i<=Dim;i++)
		{
			if( mark[i] == 1)
			{
				printf(" %d ",i);
			}
		}
		printf(" \n");
}
// INicializa busca em profundidade Color

void init_buscaP_Color(int v)
{
    int i;
    for (i=1;i<=Dim;i++)
    {
        mark[i] = 0;
    }

    mark[v] = 1;
    _MaxColor = 1;
    printf("\n Color em Prof INICIALIZADA \n");
    
}

// Busca em Profundidade a partir da Origem do Alcance

void buscaP_Color(int v)
{
    int u,w,i,j,color;
    
    for (i=1;i<=CardP[v];i++)
    {
       for(j=1; j<=5; j++)
        {
            u=randomn()*(CardP[v]+1);
            if(u==0)u=1;
            if(u==CardP[v]+1)u=CardP[v];
            w=randomn()*(CardP[v]+1);
            if(w==0)w=1;
            if(w==CardP[v]+1)w=CardP[v];
            color=LisAdjP[v][u];
            LisAdjP[v][u]=LisAdjP[v][w];
            LisAdjP[v][w]=color;
        }

        w=LisAdjP[v][i];
        if (mark[w] == 0)
        {
            for(j=1; j<=_MaxColor; j++)_CorUsada[j]=0;
            
            for(j=1; j<=CardP[w]; j++)
            {
                u = LisAdjP[w][j];
                _CorUsada[mark[u]] = 1;
            }
            
            
            j=1;
            color=0;
            do{
                if(_CorUsada[j] == 0)
                {
                    color = j;
                }
                j++;
            }while (color == 0);
        
            if (_MaxColor < color)_MaxColor = color;
           
            mark[w] = color;
            //printf("(%d,%d)\n",v,color);
            buscaP_Color(w);
        }
    }
    
}


// INicializa busca em profundidade

void init_buscaP()
{
	int i;
		for (i=1;i<=Dim;i++)
		{
			mark[i] = 0;
		}

        Hq = 0;
        IndTopol = Dim;
		printf("\n Busca em Prof INICIALIZADA \n");

}

// Busca em Profundidade a partir da Origem do Alcance

void buscaP(int v)
{
	int w,i,j;

    printf("%d \n",v);

//		Hq++;
//		Q[Hq] = v;
		
		for (i=1;i<=CardP[v];i++)
		{
          w=LisAdjP[v][i];
          if (mark[w] == 0)
	      {
		    mark[w] = 3 - mark[v];
            printf("(%d,%d)\n",v,w);
			buscaP(w);
          }
          else
          {
              printf("(%d,%d) m \n",v,w);
              if(mark[w] == mark[v])
              {
                printf(" Nao eh bi-partido!!!\n");
              }
          }
		}
		
		PosTopol[v] = IndTopol;
		IndTopol--;	
//		Hq--;

}

//////////////////////////////////////


//  Encontra Clique

void eClique()
{
    int i, j, v_curr, w, ww, max, imax, cont, vertice_Sclique;
    int S[MaxDim];
    
    imax = 1;
    max = CardP[1];
    mark[1]=0;
    
    for (i=2;i<=Dim;i++)
    {
        S[i] = 0;
        mark[i] = 0;
        _CorUsada[i] = 0;
        if(CardP[i] > max)
        {
            max = CardP[i];
            imax = i;
        }
    }
    
    printf(" 2 \n");
    
    S[imax] = 1;
    mark[imax] = 1;
    for(i=1; i<=CardP[imax]; i++)
    {
        w = LisAdjP[imax][i];
        _CorUsada[w] = 1; // vertice vizinho de imin ---> 1
    }
    
    
    v_curr = imax;
    do
    {
        vertice_Sclique = 0;
        
        max = 0;
        for (i=1;i<=Dim;i++)
        {
            if(_CorUsada[i]==1)
            {
                if (mark[i] == 0)
                {
                    cont = 0;
                    for(j=1; j<= CardP[i]; j++)
                    {
                        ww=LisAdjP[i][j];
                        if(_CorUsada[ww] == 1)
                        {
                            cont++;
                        }
                    }
                    if(cont > max)
                    {
                        max = cont;
                        imax = i;
                    }
                    
                }
                
            }
            
        }
        if (max > 0)
        {
            vertice_Sclique = 1;
            S[imax] = 1;
            v_curr = imax;
            mark[imax] = 1;
            
            printf(" imax: %d\n", imax);
            for(i=1; i<=Dim; i++)
            {
                printf(" %d %d %d |", i, _CorUsada[i], MAdj[v_curr][i]);
                _CorUsada[i] = _CorUsada[i] && MAdj[v_curr][i];
            }
            printf("\n");
        }
        
    } while (vertice_Sclique);
    
    
    printf("\n Clique \n");
    for(i=1; i<=Dim; i++)
    {
        if(S[i] == 1)
        {
            printf(" %d",i);
        }
    }
    printf("\n");
    
    
}


////////////////////////////////////

// Imprime Alcance

void print_alcance(void){
	int i;
	printf("Os vertices atingidos sao:\n");
	for(i=1;i<=Tq;i++){
		printf(" %d ", Q[i]);
	}
	printf("\n");
}

//  Alcance:  Busca em Largura (BFS- Breadth First Search)

void alcance(int v, int n){
	int u,i,k,j,w,I,F;

    // Inicializacao: Nenhum vertice foi alcancado (visitado) mark[w]=-1
    //
    //                ao final, mark[w] conterah a distancia de v a w
    //
	for(i=1;i<=n;i++) mark[i]=-1;

    //
    //  Teorema (K): Conhece-se todos os vertices a uma distancia K
    //               do vertice v (distancia em numero de arestas)
    //


    //
    // Caso Base:   K=0
    //
    // O unico vertice eh o proprio vertice v
    //
	Q[1]=v;
	Hq=1;
	Tq=1;
	mark[v]=0;


	for(k=1;k<=n;k++){

        // Passo indutivo: Se Teo(K-1) eh verdade, i.e. conhece-se todos a dist k-1
        //                 Entao Teo(K) eh verdade
        //
        //


        //
        // Prova para cada vertice com dist k-1 determinar
        // seus vizinhos que ainda nao foram visitados: serao todos
        // os vertices a dist k.

        printf(" Dist: %d %d \n",k,n);

		I=Hq;
		F=Tq;    // Q[j] para j de I a F contem todos os vertices a dist k-1
		for (j=I;j<=F;j++){
		            //
			w=Q[j]; // w eh um vertice a dist k-1 de v
			        //
			for (i=1;i<=CardP[w];i++){
				u=LisAdjP[w][i]; // u eh um vizinho de w, i.e. (w,u) estah em E
				if (mark[u]==-1){
					mark[u]=k;

					printf("%d ",u);

					Q[++Tq]=u;
				}
			}
		}
		Hq=F+1;

        printf("\n");


	}
}

//  Gera Grafo

void gera_grafo(int n, double dens)
{
	int i,j;

	M_arcos = 0;
	DistInfinita = 1;
	printf("\n\n Gerando Grafo Aleatorio de Densidade: %f #Vertices: %d\n\n",dens,n);
	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
		  if(i != j)
		  {
            if(randomn() < dens)
			{
				M_arcos++;
				I_arco[M_arcos] = i;
				J_arco[M_arcos] = j;
				MAdj[i][j]=1;
				Dist[i][j]=(int)(randomn() * 120.);
//				Dist[i][j]=  90 - (int)(randomn() * 120.);
		        DistInfinita += Dist[i][j];
			}
			else
			{
				MAdj[i][j]=0;
			}
          }
          else
          {
			MAdj[i][j]=0;
          }
		}
	}

	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			if(MAdj[i][j]==0)
			{
				Dist[i][j]=DistInfinita;
			}
		}
	}

}


//  Gera Grafo Simetrico

void gera_grafo_sim(int n, double dens)
{
	int i,j;

	M_arcos = 0;
	DistInfinita = 1;
	printf("\n\n Gerando Grafo Aleatorio de Densidade: %f #Vertices: %d\n\n",dens,n);
	for(i=1; i<=n; i++)
	{
		for(j=i; j<=n; j++)
		{
            if(i != j)
            {
			    if(randomn() < dens)
			    {
				   M_arcos++;
				   I_arco[M_arcos] = i;
				   J_arco[M_arcos] = j;
				   MAdj[i][j]=1;
				   MAdj[j][i]=1;

				   Dist[i][j]= (int)(randomn() * 1000.);
				   Dist[j][i]= Dist[i][j];

				   DistInfinita += Dist[i][j];
                 }
				 else
			     {
				   MAdj[i][j]=0;
				   MAdj[j][i]=0;
			     }
              }
              else
			  {
				 MAdj[i][j]=0;
				 MAdj[j][i]=0;
			  }
        }
    }


	for(i=1; i<=n; i++)
	{
		for(j=i; j<=n; j++)
		{
			if(MAdj[i][j]==0)
			{
				Dist[i][j]=DistInfinita;
				Dist[j][i]=DistInfinita;
			}
		}
	}

}


//  Print Grafo

void print_grafo(int n)
{
	int i,j;
	printf("\n GRAFO: Matriz de Adjacencia\n");
	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			printf("%d:%d %d ",i,j,MAdj[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			printf("%d:%d %d ",i,j,Dist[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


// Constroi Lista de Adjacencia

void  constroi_lista(int n)
{
	int i,j,p;

	for (i=1; i<=n; i++)
	{
		CardP[i] = 0;
		CardN[i] = 0;
	}

	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			if (MAdj[i][j] == 1)
			{
				p = CardP[i] + 1;
				LisAdjP[i][p] = j;
				CardP[i] = p;

				p = CardN[j] + 1;
				LisAdjN[j][p] = i;
				CardN[j] = p;
			}
		}
	}

}


// Print Listas

void print_listas(int n)
{
	int i,j, p, sum;
	sum = 0;
	printf("\n GRAFO: Listas de Adjacencia\n");
	printf("\n  Listas de Saida de Arcos \n");
	for(i=1; i<=n; i++)
	{
		printf("%d | %d Arcos Saindo : ", i, CardP[i]);
		p = CardP[i];
		for(j=1; j<=p; j++)
		{
			printf(" %d ",LisAdjP[i][j]);
		}
		printf("\n");
	}
	printf("\n  Listas de Entrada de Arcos \n");
	for(i=1; i<=n; i++)
	{
		printf("%d | %d Arcos Chegando : ", i, CardN[i]);
		p = CardN[i];
		for(j=1; j<=p; j++)
		{
			printf(" %d ",LisAdjN[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


//  Print Grafo em Arquivo

void print_grafo_arquivo(int n)
{
    int i,j, p;
    FILE *p_out;
    p_out = fopen("input.dat","wt");
    fprintf(p_out, "%d \n", n);
    for(i=1; i<=n; i++)
    {
        fprintf(p_out, "%d  %d ", i, CardP[i]);
        p = CardP[i];
        for(j=1; j<=p; j++)
        {
            fprintf(p_out, " %d ",LisAdjP[i][j]);
        }
        fprintf(p_out, "\n");
    }
    fprintf(p_out, "\n");
    fclose(p_out);

}


// System dependent routines
// File: system.cpp


void seedRandom(unsigned int seed)
// seed for random number generator.
{
  srand(seed);
  return;
}

double randomn(void)
// random number between 0.0 and 1.0 (uncluded).
{
  double rrr;

  rrr = (double) rand() / (double) RAND_MAX;
  return rrr;
}

double seconds()
// cpu time in seconds since start of run.
{
  double secs;

  secs = (double)(clock() / 1000.0);
  return(secs);
}


