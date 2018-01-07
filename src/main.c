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
 * @file main.c
 */

#include "func.h"
#include "image.h"
#include "objet.h"
#include "classe.h"
#include "point.h"
#include "move_type.h"
#include "RAG.h"
#include "iterative_fusion.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {

  int n,m;
  double threshold;
  image img = NULL;
  image final_image = NULL;
  rag graph = NULL;
  
  if ( argc < 5 || argc > 5 ) {
    printf("Usage: %s <image_filename> <nb_blocks_col> <nb_blocks_row> <threshold>\n", argv[0]);
    return 1;
  } else {
    
    threshold = atof(argv[4]);
    n = atoi(argv[2]);
    m = atoi(argv[3]);
    
        img = FAIRE_image();
	
        if (img == NULL) {
	  printf("[ERREUR] La structure pointe sur une adresse nulle.\n");
	  return 1;
        }
        image_charger(img, argv[1]);    
        printf("Image chargée ...\n");

	if (image_give_largeur(img)%n!=0) {
	  printf("ATTENTION! Perte de %d pixels sur la largeur!\n",image_give_largeur(img)%n);
	}
	if (image_give_hauteur(img)%m!=0) {
	  printf("ATTENTION! Perte de %d pixels sur la hauteur!\n",image_give_hauteur(img)%m);
	}
	
        graph = create_RAG(img, n, m);
        printf("Graph d'adjacence de région terminée ...\n");
	
        perform_merge(graph, threshold);
        printf("Fusion de régions terminée ...\n");
	
        RAG_normalize_parents(graph);
        printf("Normalisation des parents terminée ...\n");

        final_image = create_output_image(graph);
        printf("Création de l'image finale terminée ...\n");
	
        image_sauvegarder(final_image,"./../images/saved_images/save_file.ppm");
        printf("Image sauvegardée ...\n");
	
        return 0;
  }
}

