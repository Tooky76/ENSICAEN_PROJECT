#include "func.h"


extern void give_moments(image self, int block_id, int n, int m, int *M1, double *M2, double *M3 ) {

    Point coord; /* Top left block coord */

    int i, j, d;

    int block_width = Largeur/n;
    int block_heigth = Hauteur/m;

    /* Initialisation des moments */

    *M1 = 0;
    *m2[0] = 0.0;
    *M2[1] = 0.0;
    *M2[2] = 0.0;
    *M3[0] = 0.0;
    *M3[1] = 0.0;
    *M3[2] = 0.0;

    /* Fin de l'initialisation */

    *M1 = block_width * block_heigth; /* Valeur du moment d'ordre 1 */

    COORDX(coord) = (block_id / m) * block_width;
    COORDY(coord) = ((block_id % m) -1) * block_heigth;

    for ( d=0 ; d<Dim ; d++ ) {
        for ( i=0 ; i<block_heigth ; i++ ) {
            for ( j=0 ; j<block_width ; j++ ) {

                *M2[d] += image_get_comp(self, coord, d);
                *M3[d] += (*M2[d])*(*M2[d]);

                COORDX(coord)++;
            }
            COORDX(coord) = (block_id / m) * block_width;
            COORDY(coord)++;
        }
    COORDX(coord) = (block_id / m) * block_width;
    COORDY(coord) = ((block_id % m) -1) * block_heigth;
    }
}
