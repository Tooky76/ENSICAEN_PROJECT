#ifndef RAG_H
#define RAG_H

#include "func.h"
#include <stdlin.h>
#include <stdio.h>

typedef struct RAG * rag;

extern rag create_RAG(image, int,int);

static void init_moments_priv(rag,int,int);

static void init_father_priv(rag);

static void init_neighbors_priv(rag,int,int);

static void init_partition_error_priv(rag);

extern double RAG_give_closest_region(rag,int*,int*);

#endif
