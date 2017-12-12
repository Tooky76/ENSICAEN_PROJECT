#include "RAG.h"

struct moments
{
    int M0;
    double M1[3];
    double M2[3];
};
typedef struct cellule* cellule;

struct cellule
{
    int block;
    cellule next;
};

typedef struct moments* moments;

typedef struct RAG {
    image  img;
    int nb_blocks;
    long double erreur_partition;
    moments m;
    int * father;
    cellule neighbors;
} rag;

extern rag create_RAG(image self ,int n ,int m) {

    rag graph;

    graph.image = self;
    graph.nb_blocks = n*m;

    init_moments_priv(graph, n, m);
    init_father_priv(graph);
    init_neighbors_priv(graph, n, m);
    init_partition_error_priv(graph);


}



static void init_moments_priv(rag graph ,int n ,int m) {

    int i;

    graph.m = malloc(graph.nb_blocks*sizeof(moments));
    
    for ( i=0 ; i<n*m ; i++ ) {
        give_moments(graph.image, i, n, m, graph.m[i].M0;, graph.m[i].M2, graph.m[i].M3);
    }

}

static void init_father_priv(rag graph) {

    int i;

    graph.father = malloc(graph.nb_blocks*sizeof(int));

    for ( i=0 ; i<graph.nb_blocks ; i++ ) {
        graph.father[i] = i;
    }
}

static void init_neighbors_priv(rag graph ,int n ,int m) {

    int i;
    
    graph.neighbors = malloc(graph.nb_blocks*sizeof(struct cellule));

    for ( i=1 ; i<=graph.nb_blocks ; i++ ) {

        graph.neighbors[i].block = (i%n != 0) ? i+1 : NULL;
        graph.neighbors[i].next = (i+n < graph.nb_blocks) ? graph.neighbors[i+n] : NULL;
    }
}

static void init_partition_error_priv(rag graph) {

    int i, j;

    graph.erreur_partition = malloc(graph.nb_blocks*sizeof(long double));

    for ( i=0 ; i<graph.nb_blocks ; i++ ) {
        graph.erreur_partition[i] = 0;
        for ( j=0 ; j<3 ; j++ ) {
            graph.erreur_partition[i] += graph.m[i].M2 - (graph.m[i].M1/graph.m[i].M0)*(graph.m[i].M1/graph.m[i].M0);
        }
    }
}
