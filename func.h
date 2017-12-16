#ifndef FUNC_H
#define FUNC_H

#include "image.h"

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
extern void give_moments(image, int, int, int, int*, double*, double* );

#endif
