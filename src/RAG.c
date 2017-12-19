#include "RAG.h"

/**
 * @file RAG.c
 */

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

/**
 * Créer le graph utiliser pour gérer les blocks
 *
 * @param self image traitée
 * @param n nombre de blocks sur la largeur
 * @param m nombre de blocks sur la longueur
 */

extern rag create_RAG(image self ,int n ,int m) {

    rag graph;

    graph.image = self;
    graph.nb_blocks = n*m;

    init_moments_priv(graph, n, m);
    init_father_priv(graph);
    init_neighbors_priv(graph, n, m);
    init_partition_error_priv(graph);


}

/**
 * Cette fonction associe à chaque block ses moments dans la grap
 * @param graph le graph utilisé
 * @param n nombre de blocks sur la largeur
 * @param m nombre de blocks sur la longueur
 */
static void init_moments_priv(rag graph ,int n ,int m) {

    int i;

    graph.m = malloc(graph.nb_blocks*sizeof(moments));
    
    for ( i=0 ; i<n*m ; i++ ) {
        give_moments(graph.image, i, n, m, &(graph.m[i].M0), graph.m[i].M2, graph.m[i].M3);
    }

}

/**
 * Cette fonction initialise le block père de chaque block sur lui-même 
 * @param graph le graph utilisé
 */
static void init_father_priv(rag graph) {

    int i;

    graph.father = malloc(graph.nb_blocks*sizeof(int));

    for ( i=0 ; i<graph.nb_blocks ; i++ ) {
        graph.father[i] = i;
    }
}

/**
 * Cette fonction donne à chaque block ses voisins de droite et du dessous s'ils existent.
 * @param graph le graph utilisé
 * @param n nombre de blocks sur la largeur
 * @param m nombre de blocks sur la longueur
 */

static void init_neighbors_priv(rag graph ,int n ,int m) {

    int i;
    
    graph.neighbors = malloc(graph.nb_blocks*sizeof(struct cellule));

    for ( i=0 ; i<graph.nb_blocks ; i++ ) {

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

/**
 * Cette fonction calcule l'erreur quadratique de chaque block dans la graph.
 * @param graph le graph utilisé
 */

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

/**
 * Cette fonction parcourt graph pour identifier ,grâce au calcule de l'erreur quadratique engendrée par leur
 * potentielle fusion, la meilleure fusion.
 *
 * @return Cette fonction retourne l'erreur calculée la plus basse et modifie les deux pointeurs sur les indices
 * des blocks correspondants.
 *  
 * @param graph le graph utilisé.
 * @param block1 pointeur sur l'indice du premier block à fusionner.
 * @param block2 pointeur sur l'indice du second block à fusionner.
 */

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


/**
 * Cette fonction merge le i-ème block avec le j-ème block et suppose que i < j.
 * 
 * @param graph le graph utilisé.
 * @param i le block à fusionner.
 * @param j l'autre block à fusionner et le nouveau father de i.
 */
extern void RAG_merge_region(rag graph, int i, int j) {

    graph.father[i] = j;   /* Mise à jour du tableau father */
    update_moment(graph,i,j);
    update_neighbors(graph,i,j);
    update_error(graph,i,j);

}

/**
 * Cette fonction met à jour les moments du block j.
 *
 * @param graph le graph utilisé.
 * @param i le block fusionné utilisé pour le calcul des moments.
 * @param j le block dont on modifie les moments.
 */

static void update_moment(rag graph, int i, int j) {

    int k;

    graph.m[j].M0 += graph.m[i].M0;
    for ( k=0 ; k<3 ; k++ ) {
        graph.m[j].M1 += graph.m[i].M1;
        graph.m[j].M2 += graph.m[i].M2;
    }
}

/**
 * Cette fonction met à jour les voisins des blocks fusionnés.
 *
 * @param graph le graph utilisé.
 * @param i le block qui perd ses voisins.
 * @param j le block qui hérie des voisins de i.
 */

static void update_neighbors(rag graph, int i, int j) {

    cellule cel;

    cel = graph.neighbors[j];
    insert_in_sort_list(cel,(graph.neighbors[i].next).block);
    graph.neighbors[i].block = NULL; /* Mise à zéro des voisin du block i */
    graph.neighbors[i].next = NULL; /* Suppression des voisins du block i */
}

/**
 * Cette fonction met à jour l'erreur quadratique dans le block fusionné.
 *
 * @param graph le graph utilisé.
 * @param i le premier block utilisé
 * @param j le second blok utilisé.
 */

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
