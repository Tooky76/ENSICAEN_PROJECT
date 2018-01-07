/* -*- c-basic-offset: 3 -*- 
 *
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author Benjamin Monfort <monfort@ecole.ensicaen.fr>
 * @author Benli Huraut <huraut@ecole.ensicaen.fr>
 * @version 1.0.0 / 06-01-2018
 */

/**
 * @file RAG.c
 */


#include <RAG.h>
#include <objet.h>

static void init_moments_priv(rag,int,int);

static void init_father_priv(rag);

static void init_neighbors_priv(rag,int,int);

static void init_partition_error_priv(rag);



/**
 * Creates the Region Adjacency Graph of an image given in first parameter
 *
 * @param img Image to deal with
 * @param n Number of blocks in a row
 * @param m Number of blocks in a column
 *
 * @return Returns the region adjacency graph just created and initialized by the function
 */
extern rag create_RAG(image self ,int n ,int m) {

    rag graph;

    graph = malloc(sizeof(struct RAG));
    graph->nb_blocks = n*m;
    graph->img = self;
    graph->nb_col = n;
    graph->nb_row = m;

    init_moments_priv(graph, n, m);
    init_father_priv(graph);
    init_neighbors_priv(graph, n, m);
    init_partition_error_priv(graph);

    return graph;
}

/**
 * Initializes the momentums of the RAG
 *
 * @param graph Region adjacency graph to deal with
 * @param n Number of blocks in a row
 * @param m Number of blocks in a column
 *
 */
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

/**
 * Initializes the neighbors array of the RAG
 *
 * @param graph Region adjacency graph to deal with
 * @param n Number of blocks in a row
 * @param m Number of blocks in a column
 *
 */
static void init_neighbors_priv(rag graph ,int n ,int m) {

    int i;
    
    graph->neighbors = malloc(graph->nb_blocks*sizeof(struct cellule));

    for ( i=0 ; i<graph->nb_blocks ; i++ ) {

      graph->neighbors[i].block = ((i+1)%n != 0) ? i+1 : -1;
        
        if ( i+1+n< graph->nb_blocks+1 ) {
	  graph->neighbors[i].next = &(graph->neighbors[i+n]);
	  graph->neighbors[i].next->block = i+n;
	  
	} else {
            graph->neighbors[i].next = NULL;
        }
    }
}

/**
 * Initializes the partition error from the RAG
 *
 * @param graph Region adjacency graph to deal with
 *
 */
static void init_partition_error_priv(rag graph) {

    int i, j;

    for ( i=0 ; i<graph->nb_blocks ; i++ ) {
        graph->erreur_partition = 0;
        for ( j=0 ; j<3 ; j++ ) {
            graph->erreur_partition += graph->m[i].M2[j] - (graph->m[i].M1[j]/graph->m[i].M0)*(graph->m[i].M1[j]/graph->m[i].M0);
        }
    }
}

/**
 * Gives the block index of the two closest blocks
 *
 * @param graph Region adjacency graph to deal with
 * @param a Pointer on the block index of one of the closest block
 * @param b Pointer on the block index of the other closest block
 *
 * @return Error added by the fusion of the two closest blocks
 */

extern double RAG_give_closest_region(rag graph, int* a, int* b) { 

  int i,j;
    int k;
    cellule current;
    double error = -1.0;
    double denominator = 0.0;
    double avg,sqravg;

    *a = 0;
    *b = 0;

    for ( i=1 ; i<=graph->nb_blocks ; i++ ) {
        if ( graph->father[i] == i ) {

            current = &(graph->neighbors[i]);  
            while (current->next != NULL) {
	      avg=0.0;
	      j = current->next->block;
	      denominator = (graph->m[i].M0*graph->m[j].M0)/(graph->m[i].M0+graph->m[j].M0);
	      for ( k=0 ; k<image_give_dim(graph->img) ; k++) {
		sqravg=(graph->m[i].M0/graph->m[i].M1[k]-graph->m[j].M0/graph->m[j].M1[k]);
		sqravg=sqravg*sqravg;
		avg+=sqravg;
	      }
	      denominator=denominator*avg;
	      
	      if ( error < 0  || denominator < error ) {
		error = denominator;
		*a = i;
		*b = j;
	      }
	      current = current->next;
	    }
        }
    }
    return error;
}


/**
 * Inserts in orger in a list an element 
 *
 * @param neighbors RAG neighbors array
 * @param element Element to insert in the neighbors array
 *
 */
static void insert_in_sort_list(cellule neighbors, int element) { 

    cellule previous;
    cellule current;
    cellule cel;
    int find;

    cel = malloc(sizeof(struct cellule));

    find = 0;
    cel->block = element;
    if ( neighbors->block <0  ) { 
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

/**
 * Updates moments from the RAG for a merge
 *
 * @param graph Region adjacency graph to deal with
 * @param i Block index of the first block to merge
 * @param j Block index of the second block to merge
 *
 */
static void update_moment(rag graph, int i, int j) {

    int k;

    graph->m[j].M0 += graph->m[i].M0;
    for ( k=0 ; k<3 ; k++ ) {
        graph->m[j].M1[k] += graph->m[i].M1[k];
        graph->m[j].M2[k] += graph->m[i].M2[k];
    }
}

/**
 * Updates the neighbors array from the RAG for a merge
 *
 * @param graph Region adjacency graph to deal with
 * @param i Block index of the first block to merge
 * @param j Block index of the second block to merge
 *
 */
static void update_neighbors(rag graph, int i, int j) {

    cellule previous, current;

    current= &(graph->neighbors[i]);   
    previous = current;
    while (current->next != NULL) {
      if ( current->block > j ) {
	insert_in_sort_list(&(graph->neighbors[j]),current->block);
      }
      current->block = -1;
      previous = current;
      current = current->next;
      previous->next=NULL;
    }
    insert_in_sort_list(&(graph->neighbors[j]),current->block);
    current->block = -1;
}

/**
 * Updates the error partition from the RAG for a merge
 *
 * @param graph Region adjacency graph to deal with
 * @param i Block index of the first block to merge
 * @param j Block index of the second block to merge
 *
 */
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


/**
 * Merges regions in the region adjacency graph between two closest regions
 *
 * @param graph Region adjacency graph to deal with
 * @param i Block index of the first block to merge
 * @param j Block index of the second block to merge
 *
 */
extern void RAG_merge_region(rag graph, int i, int j) {
  int tmp;
  
  if ( i > j ) {
    tmp = i;
    i = j;
    j = tmp;
  }

    graph->father[i] = j;   
    update_moment(graph,i,j);
    update_neighbors(graph,i,j);
    update_error(graph,i,j);
}


