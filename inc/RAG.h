#ifndef RAG_H
#define RAG_H

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
    int * father;
    cellule neighbors;
};

typedef struct RAG * rag;

extern rag create_RAG(image, int,int);

static void init_moments_priv(rag,int,int);

static void init_father_priv(rag);

static void init_neighbors_priv(rag,int,int);

static void init_partition_error_priv(rag);

extern double RAG_give_closest_region(rag,int*,int*);

#endif
