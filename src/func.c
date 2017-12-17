#include "func.h"

/**
 * Calcul les moments d'ordre 0 à 2 du bloc donné en second paramètre
 *
 * @param self Image à traiter
 * @param block_id Indice du block à traiter dans l'image donnée en premier paramètre
 * @param n Nombre de blocs sur la largeur
 * @param m Nombre de blocs sur la hauteur
 * @param M0 Pointeur sur le moment d'ordre 0 du block correspondant
 * @param M1 Pointeur sur le moment d'ordre 1 du block correspondant
 * @param M2 Pointeur sur le moment d'ordre 2 du block correspondant
 *
 */
extern void give_moments(image self, int block_id, int n, int m, int *M0, double *M1, double *M2 ) {

    Point coord; /* Coordonnées du coin en haut à gauche */

    int i, j, d;

    int block_width = Largeur/n;
    int block_heigth = Hauteur/m;

    /* Initialisation des moments */

    *M0 = 0;
    for ( i=0 ; i<3 ; i++ ) {
        M1[i] = 0.0;
        M2[i] = 0.0;
    }

    /* Fin de l'initialisation */

    /* Valeur du moment d'ordre 1 */
    *M0 = block_width * block_heigth;

    /* Valeur des moments d'ordre 1 et 2 */
    COORDX(coord) = (block_id / m) * block_width;
    COORDY(coord) = ((block_id % m) -1) * block_heigth;

    for ( d=0 ; d<Dim ; d++ ) {
        for ( i=0 ; i<block_heigth ; i++ ) {
            for ( j=0 ; j<block_width ; j++ ) {

                *M1[d] += image_get_comp(self, coord, d);
                *M2[d] += (*M2[d])*(*M2[d]);

                COORDX(coord)++;
            }
            COORDX(coord) = (block_id / m) * block_width;
            COORDY(coord)++;
        }
    COORDX(coord) = (block_id / m) * block_width;
    COORDY(coord) = ((block_id % m) -1) * block_heigth;
    }
}
