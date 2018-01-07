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
 * @file RAG.h
 */




#ifndef RAG_H
#define RAG_H

#include "func.h"
#include <stdlib.h>
#include <stdio.h>


struct moments
{
    int M0;
    double M1[3];
    double M2[3];
};

typedef struct moments* moments;

typedef struct cellule* cellule;

struct cellule
{
    int block;
    cellule next;
};





struct RAG {
    image  img;
    int nb_blocks;
    long double erreur_partition;
    moments m;
    int nb_row;
    int nb_col;
    int * father;
    cellule neighbors;
};

typedef struct RAG* rag;

/**
 * Creates the Region Adjacency Graph of an image given in first parameter
 */
extern rag create_RAG(image, int,int);

/**
 * Gives the block index of the two closest blocks
 */
extern double RAG_give_closest_region(rag,int*,int*);

/**
 * Merges regions in the region adjacency graph between two closest regions
 */
extern void RAG_merge_region(rag, int, int);

#endif
