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
 * @file func.h
 */

#ifndef FUNC_H
#define FUNC_H

#include "image.h"

/**
 * Computes the 0 to 2 order momentums given in second parameter
 */
extern void give_moments(image, int, int, int, int*, double*, double* );

#endif
