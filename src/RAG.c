#include <RAG.h>
#include <objet.h>



extern rag create_RAG(image self ,int n ,int m) {

    rag graph;

    graph = malloc(sizeof(struct RAG));
    graph->nb_blocks = n*m;
    graph->img = self;

    init_moments_priv(graph, n, m);
    init_father_priv(graph);
    init_neighbors_priv(graph, n, m);
    init_partition_error_priv(graph);

    return graph;
}

static void init_moments_priv(rag graph ,int n ,int m) {

    int i;
    
    graph->m = malloc(graph->nb_blocks*sizeof(struct moments));

    for ( i=0 ; i<n*m ; i++ ) {
        give_moments(graph->img, i, n, m, &(graph->m[i].M0), graph->m[i].M1, graph->m[i].M2);
    }
}

static void init_father_priv(rag graph) {

    int i;

    graph->father = malloc(graph->nb_blocks*sizeof(int));

    for ( i=0 ; i<graph->nb_blocks ; i++ ) {
        graph->father[i] = i;
    }
}

static void init_neighbors_priv(rag graph ,int n ,int m) {

    int i;
    
    graph->neighbors = malloc(graph->nb_blocks*sizeof(struct cellule));

    for ( i=0 ; i<graph->nb_blocks ; i++ ) {

      graph->neighbors[i].block = ((i+1)%n != 0) ? i+1 : -1;
	/*Impossible d'associer NULL  à block je lui donne -1, j'adapte le reste du code*/
        if ( i+1+n< graph->nb_blocks+1 ) {
	  graph->neighbors[i].next = &(graph->neighbors[i+n]);
	  graph->neighbors[i].next->block = i+n;
	  /* graph->neighbors[i].next->next = NULL;*/
	} else {
            graph->neighbors[i].next = NULL;
        }
    }
}

static void init_partition_error_priv(rag graph) {

    int i, j;

    for ( i=0 ; i<graph->nb_blocks ; i++ ) {
        graph->erreur_partition = 0;
        for ( j=0 ; j<3 ; j++ ) {
            graph->erreur_partition += graph->m[i].M2[j] - (graph->m[i].M1[j]/graph->m[i].M0)*(graph->m[i].M1[j]/graph->m[i].M0);
        }
    }
}


extern double RAG_give_closest_region(rag graph, int* a, int* b) {

    int i;
    int k;
    cellule current;
    double error = -1.0;
    long double tmp = 0.0;
    long double denominator = 0.0;
    int fatherof;

    *a = 0;
    *b = 0;

    for ( i=1 ; i<=graph->nb_blocks ; i++ ) {
        if ( graph->father[i] == i ) {

            current = &(graph->neighbors[i]);  
            while (current->next != NULL) {
                fatherof = graph->father[current->block];
                denominator = graph->m[i].M0*graph->m[fatherof].M0*(graph->m[i].M0+graph->m[fatherof].M0);
                for ( k=0 ; k<image_give_dim(graph->img) ; k++) {
                    tmp += graph->m[fatherof].M0*graph->m[i].M1[k] - graph->m[i].M0*graph->m[fatherof].M1[k];
                }
                tmp /= denominator;
                if ( error < 0  || tmp < error ) {
                    error = tmp;
                    *a = i;
                    *b = fatherof;
                }
                current = current->next;
            }

            fatherof = graph->father[current->block];
            denominator = graph->m[i].M0*graph->m[fatherof].M0*(graph->m[i].M0+graph->m[fatherof].M0);
            for ( k=0 ; k<image_give_dim(graph->img) ; k++) {
                tmp += graph->m[fatherof].M0*graph->m[i].M1[k] - graph->m[i].M0*graph->m[fatherof].M1[k];
            }
            tmp /= denominator;
            if ( error < 0  || tmp < error ) {
                error = tmp;
                *a = i;
                *b = fatherof;
            }
        }
    }
    return error;
}

static void insert_in_sort_list(cellule neighbors, int element) { /* TD d'algo insertion dans une liste simple triée */

    cellule previous;
    cellule current;
    cellule cel;
    int find;

    cel = malloc(sizeof(struct cellule));

    find = 0;
    cel->block = element;
    if ( neighbors->block <0  ) { /*remplacé comparaison avec NULL avec le nouveau cas block=-1)*/
        cel->next = NULL;
        neighbors = cel;
        return;
    }

    if ( neighbors->block > element ) {
        cel->next = neighbors;
        neighbors = cel;
        return;
    }
    current = neighbors;

    do {
        previous = current;
        current = current->next;
        find = ( current == NULL ) ? 1 : 0;
        if ( !find ) {
            find = ( current->block > element ) ? 1 :0;
        }
    } while ( !find );
    cel->next = current;
    previous->next = cel;
}

static void update_moment(rag graph, int i, int j) {

    int k;

    graph->m[j].M0 += graph->m[i].M0;
    for ( k=0 ; k<3 ; k++ ) {
        graph->m[j].M1[k] += graph->m[i].M1[k];
        graph->m[j].M2[k] += graph->m[i].M2[k];
    }
}

static void update_neighbors(rag graph, int i, int j) {

    cellule previous, current;

    current= &(graph->neighbors[i]);    /* Suppression en cascade des blocs précedents de neighbors[i] */
    previous = current;
    while (current->next != NULL) {
      if ( current->block > j ) {
	insert_in_sort_list(&(graph->neighbors[j]),current->block);
      }
      current->block = -1;/*remplacé NULL par -1*/
      previous = current;
      current = current->next;
      previous->next=NULL;
    }
    insert_in_sort_list(&(graph->neighbors[j]),current->block);
    current->block = -1;/*remplacé NULL par -1*/
}

static void update_error(rag graph, int i, int j) {

    int k;
    long double tmp = 0.0;
    long double denominator = 0.0;

    denominator = graph->m[i].M0*graph->m[j].M0*(graph->m[i].M0+graph->m[j].M0);
    for ( k=0 ; k<image_give_dim(graph->img) ; k++) {
        tmp += graph->m[j].M0*graph->m[i].M1[k] - graph->m[i].M0*graph->m[j].M1[k];
    }
    tmp /= denominator;

    graph->erreur_partition += tmp;
}


/*
 * 
 * On merge le i-ème block avec le j-ème block et on suppose que i < j
 *
 */
extern void RAG_merge_region(rag graph, int i, int j) {
  int tmp;
  
  if ( i > j ) {
    tmp = i;
    i = j;
    j = tmp;
  }

    graph->father[i] = j;   /* Mise à jour du tableau father */
    update_moment(graph,i,j);
    update_neighbors(graph,i,j);
    update_error(graph,i,j);
}


