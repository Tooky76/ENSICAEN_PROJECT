#include "RAG.h"

struct moments
{
    int M0;
    double M1[3];
    double M2[3];
};
typedef struct cellule* cellule;

struct cellule
{
    int block;
    cellule next;
};

typedef struct moments* moments;

typedef struct RAG {
    image  img;
    int nb_blocks;
    long double erreur_partition;
    moments m;
    int * father;
    cellule neighbors;
} rag;

extern rag create_RAG(image self ,int n ,int m) {

    rag graph;

    graph.image = self;
    graph.nb_blocks = n*m;

    init_moments_priv(graph, n, m);
    init_father_priv(graph);
    init_neighbors_priv(graph, n, m);
    init_partition_error_priv(graph);


}



static void init_moments_priv(rag graph ,int n ,int m) {

    int i;

    graph.m = malloc(graph.nb_blocks*sizeof(moments));
    
    for ( i=0 ; i<n*m ; i++ ) {
        give_moments(graph.image, i, n, m, &(graph.m[i].M0), graph.m[i].M2, graph.m[i].M3);
    }

}

static void init_father_priv(rag graph) {

    int i;

    graph.father = malloc(graph.nb_blocks*sizeof(int));

    for ( i=0 ; i<graph.nb_blocks ; i++ ) {
        graph.father[i] = i;
    }
}

static void init_neighbors_priv(rag graph ,int n ,int m) {

    int i;
    
    graph.neighbors = malloc(graph.nb_blocks*sizeof(struct cellule));

    for ( i=1 ; i<=graph.nb_blocks ; i++ ) {

        graph.neighbors[i].block = (i%n != 0) ? i+1 : NULL;
        graph.neighbors[i].next = (i+n < graph.nb_blocks) ? graph.neighbors[i+n] : NULL;
    }
}

static void init_partition_error_priv(rag graph) {

    int i, j;

    graph.erreur_partition = malloc(graph.nb_blocks*sizeof(long double));

    for ( i=0 ; i<graph.nb_blocks ; i++ ) {
        graph.erreur_partition[i] = 0;
        for ( j=0 ; j<3 ; j++ ) {
            graph.erreur_partition[i] += graph.m[i].M2 - (graph.m[i].M1/graph.m[i].M0)*(graph.m[i].M1/graph.m[i].M0);
        }
    }
}

/*Calcul de la meilleure fusion*/

extern double RAG_give_closest_region(rag graph,int* block1,int* block2) {
  int i,j,k;
  double MA,MB,new_error,avg;
  double *M1A,*M1B;
  double min_error=NULL;

  for (i=0;i<graph.nb_blocks;i++) {
    if (graph.m[i].father==i) {
      /*On regarde déjà le couple block i et son voisin d'à côté s'il en a un*/
      if (graph.neighbors[i].block!=NULL) {
	j=graph.neighbors[i].block;
	
	M0A=graph.m[i].M0;
	M0B=graph.m[j].M0;
	
	avg=pow(M1A[0]-M1B[0],2)+pow(M1A[1]-M1B[1],2)+pow(M1A[2]-M1B[2],2);
	new_error=(M0A*M0B)*avg/(M0A+M0B);
	
	if (min_error==NULL || new_error<min_error) {
	  min_error=new_error;
	  *block1=i;
	  *block2=j;
	}
      }
      /* On teste avec son voisin du dessous s'il en a un*/
      if (graph.neighbors[i].next!=NULL) {
	j=graph.neighbors[i].next;
	M0B=graph.m[j].M0;

	avg=pow(M1A[0]-M1B[0],2)+pow(M1A[1]-M1B[1],2)+pow(M1A[2]-M1B[2],2);
	new_error=(M0A*M0B)*avg/(M0A+M0B);


	if (min_error>new_error) {
	  min_error=new_error;
	  *block1=i;
	  *block2=j;
	}
      }
    }
  }
  return min_error;
}
