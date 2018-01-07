#ifndef ITERATIVE_FUSION_H
#define ITERATIVE_FUSION_H

#include "func.h"
#include "RAG.h"
#include <stdlib.h>
#include <stdio.h>

void perform_merge(rag graph, double threshold);

extern void RAG_normalize_parents(rag graph);

extern void RAG_give_mean_color(rag graph, int block_id, int* avg_color);

image create_output_image(rag graph);


#endif