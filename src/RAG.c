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
	if ( i+n < graph.nb_blocks ) {
            graph.neighbors[i].next = graph.neighbors[i+n];
            (graph.neighbors[i].next).block = i+n;
            (graph.neighbors[i].next).next = NULL;    
        } else {
            graph.neighbors[i].next = NULL;
        }
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


/*
 * 
 * On merge le i-ème block avec le j-ème block et on suppose que i < j
 *
 */
extern void RAG_merge_region(rag graph, int i, int j) {

    graph.father[i] = j;   /* Mise à jour du tableau father */
    update_moment(graph,i,j);
    update_neighbors(graph,i,j);
    update_error(graph,i,j);

}

static void update_moment(rag graph, int i, int j) {

    int k;

    graph.m[j].M0 += graph.m[i].M0;
    for ( k=0 ; k<3 ; k++ ) {
        graph.m[j].M1 += graph.m[i].M1;
        graph.m[j].M2 += graph.m[i].M2;
    }
}

static void update_neighbors(rag graph, int i, int j) (

    cellule cel;

    cel = graph.neighbors[j];
    insert_in_sort_list(cel,(graph.neighbors[i].next).block);
    graph.neighbors[i].block = NULL; /* Mise à zéro des voisin du block i */
    graph.neighbors[i].next = NULL; /* Suppression des voisins du block i */
)

static void update_error(rag graph, int i, int j) {

    long double tmp;
    long double avg_i;
    long double avg_j;

    avg_i = (graph.m[i].M1/graph.m[i].M0);
    avg_j = (graph.m[j].M1/graph.m[j].M0);
    tmp = (graph.m[i].M0*graph.m[j].M0)/(graph.m[i].M0 + graph.m[j].M0) * (avg_i - avg_j)*(avg_i - avg_j);
    graph.erreur_partition += tmp;
}

static void insert_in_sort_list(cellule neighbors, int element) { /* TD d'algo insertion dans une liste simple triée */

    cellule previous;
    cellule current;
    cellule cel;
    int find;

    find = 0;

    cel.block = element;
    if ( neighbors.block == NULL ) {
        cel.next = NULL;
        neighbors = cel;
        return;
    }
    if ( neighbors.block > element ) {
        cel.next = neighbors
        neighbors = cel;
        return;
    }
    current = neighbors;
    do {
        previous = current;
        current = current.next;
        find = ( current == NULL ) ? 1 : 0;
        if ( !find ) {
            find = ( current.block > element ) ? 1 :0;
        }
    } while ( !find );
    cel.next = current;
    previous.next = cel;
}
