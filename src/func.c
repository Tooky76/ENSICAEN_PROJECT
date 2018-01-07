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
 * @file func.c
 */


#include "func.h"
#include "objet.h"

/**
 * Computes the 0 to 2 order momentums given in second parameter
 *
 * @param img Image to deal with
 * @param block_id Index of the block to deal with in the first parameter
 * @param n Number of blocks in a row
 * @param m Number of blocks in a column
 * @param M0 Pointer on the 0 order momentum of the relevant block
 * @param M1 Pointer on the 1 order momentum of the relevant block
 * @param M2 Pointer on the 2 order momentum of the relevant block
 *
 */
extern void give_moments(image self, int block_id, int n, int m, int *M0, double *M1, double *M2 ) {


    Point coord; /* Coordonnées du coin en haut à gauche */


    int i, j, d, k;

    int block_width = image_give_largeur(self)/n;
    int block_height = image_give_hauteur(self)/m;
    image_debut(self);

    /* Initialisation des moments */

    *M0 = 0;
    for ( k=0 ; k<3 ; k++ ) {
        M1[k] = 0.0;
        M2[k] = 0.0;
    }

    /* Fin de l'initialisation */

    /* Valeur du moment d'ordre 1 */
    *M0 = block_width * block_height;

    /* Valeur des moments d'ordre 1 et 2 */
    COORDX(coord) = (block_id / m) * block_width;
    COORDY(coord) = (block_id % m) * block_height;

    for ( d=0 ; d<image_give_dim(self) ; d++ ) {
        for ( i=0 ; i<block_height ; i++ ) {
            for ( j=0 ; j<block_width ; j++ ) {
                if ( image_move_to(self,&coord) == faux ) {
                    printf("[ERREUR] 1\n");
                    return;
                }
                M1[d] += image_lire_pixel(self)[d];
                M2[d] += (M1[d])*(M1[d]);

                COORDX(coord)++;
                /*if ( image_move_to(self,&coord) == faux ) {
                    printf("[ERREUR] 2\n");
                    return;
		    }*/
            }
            COORDX(coord) = (block_id / m) * block_width;
	    COORDY(coord)++;
            if ( image_move_to(self,&coord) == faux ) {
                printf("[ERREUR] 3\n");
                return;
            }
        }
    COORDX(coord) = (block_id / m) * block_width;
    COORDY(coord) = (block_id % m) * block_height;
    if ( image_move_to(self,&coord) == faux ) {
        printf("[ERREUR] 4\n");
        return;
    }
    }
}
