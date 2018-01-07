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
 * @file iterative_fusion.c
 */


#include "RAG.h"
#include "image.h"

/**
 * Merges the closest blocks in the RAG until the partition error reach the threshold 
 *
 * @param graph Region adjacency graph to deal with
 * @param threshold Value of the error_max until the merge can perform
 *
 */
void perform_merge(rag graph, double threshold) {

  int indice1, indice2;
  double error;
  do {
    error=RAG_give_closest_region(graph,&indice1,&indice2);
    if (error<threshold) {
      RAG_merge_region(graph, indice1, indice2);
    }
  } while (error/*graph->erreur_partition*/ < threshold );
}

/**
 * Normalizes the father array from the RAG
 *
 * @param graph Region adjacency graph to deal with
 *
 */
extern void RAG_normalize_parents(rag graph) {

    int i;

    for ( i = graph->nb_blocks -1 ; i>=0 ; i-- ) {
        graph->father[i] = graph->father[graph->father[i]];
    }

}

/**
 * Gives the mean color of a block in the thrid argument array 
 *
 * @param graph Region adjacency graph to deal with
 * @param block_id Block index of the block that the mean color is damanded
 * @param avg_color Array that receive the mean color of the block given in second parameter
 *
 */
extern void RAG_give_mean_color(rag graph, int block_id, int* avg_color) { /* On suppose que avg_color est un tableau de 3 cases pour les couleurs */

    int i;

    for ( i=0 ; i<3 ; i++ ) {
        avg_color[i] = graph->m[graph->father[block_id]].M1[i]/graph->m[graph->father[block_id]].M0;
    }
}

/**
 * Creates the output image based on the RAG given in parameter 
 *
 * @param graph Region adjacency graph to deal with
 *
 * @return Returns the output image created according to the RAG
 *
 */
image create_output_image(rag graph) {
    
    int i,j,k;
    int wid,hei;
    image img = NULL;
    int * avg_color;
    int block_width = image_give_largeur(graph->img)/graph->nb_col;
    int block_height = image_give_hauteur(graph->img)/graph->nb_row;
    Point point;

    avg_color = malloc(sizeof(int)*image_give_dim(graph->img));

    img = FAIRE_image();
    
    if (img == NULL) {
        printf("[ERREUR] La structure pointe sur une adresse nulle.\n");
        return NULL;
    }

    wid=block_width*graph->nb_col;
    hei=block_height*graph->nb_row;
    image_initialize(img, image_give_dim(graph->img), wid, hei);
    image_debut(img);
    point.coordx=0;
    point.coordy=0;
    image_move_to(img,&point);
  
    for ( i=0 ; i<graph->nb_blocks ; i++) {
     RAG_give_mean_color(graph, i, avg_color);
     point.coordy = (i%graph->nb_row)*block_height;
     point.coordx = (i/graph->nb_row)*block_width;
     image_move_to(img,&point);
     for ( j=0 ; j<block_height ; j++ ) {
       for ( k=0 ; k<block_width; k++ ) {
	 image_ecrire_pixel(img, avg_color);
	 image_pixel_suivant(img);
       }
       MOVE_DOWN_POINT(point);
       image_move_to(img,&point);
     }
    }
    free(avg_color);
    return img;
}

