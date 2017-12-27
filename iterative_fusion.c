#include "RAG.h"
#include "image.h"

void perform_merge(rag graph, double threshold) {

    int indice1, indice2;

    do {
        RAG_give_closest_region(graph,&indice1,&indice2);
        RAG_merge_region(graph, indice1, indice2);
    } while ( graph->erreur_partition < threshold );

}


extern void RAG_normalize_parents(rag graph) {

    int i;

    for ( i = graph->nb_blocks -1 ; i>=0 ; i-- ) {
        graph->father[i] = graph->father[graph->father[i]];
    }

}


extern void RAG_give_mean_color(rag graph, int block_id, int* avg_color) { /* On suppose que avg_color est un tableau de 3 cases pour les couleurs */

    int i;

    for ( i=0 ; i<3 ; i++ ) {
        avg_color[i] = graph->m[graph->father[block_id]].M1[i]/graph->m[graph->father[block_id]].M0;
    }
}


image create_output_image(rag graph) {
    
    int i;
    image img = NULL;
    int avg_color[3];

    img = FAIRE_image();
    
    if (img == NULL) {
        printf("[ERREUR] La structure pointe sur une adresse nulle.\n");
        return -1;
    }


    for ( i=0 ; i<graph->nb_blocks ; i++) {
        RAG_give_mean_color(graph, i, avg_color);
    }

    return img;
}
