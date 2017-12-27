#include "func.h"
#include "image.h"
#include "objet.h"
#include "classe.h"
#include "point.h"
#include "move_type.h"
#include "RAG.h"
#include "iterative_fusion.h"


int main() {

    char* nom = "./../IMAGES/fille.ppm";
    image img = NULL;
    long double error = 0.0;
    int indice1;
    int indice2;
    int *color;
    int n = 10;
    int m = 10;
    
    img = FAIRE_image();

    if (img == NULL) {
        printf("[ERREUR] La structure pointe sur une adresse nulle.\n");
        return -1;
    }

    image_charger(img,nom);    
    printf("Image chargée ...\n");

    rag graph = create_RAG(img ,n ,m);
    printf("Fonction create_RAG terminée ...\n");

    perform_merge(graph, 0.5*graph->erreur_partition);
    printf("Perform merge done ...\n");

    RAG_normalize_parents(graph);
    printf("RAG_normalize_parents done ...\n");

    color = malloc(3*sizeof(int));

    RAG_give_mean_color(graph, 2, color);
    printf("RAG_give_mean_color done ...\n");

    create_output_image(graph);
    printf("Create_output_image done ...\n");

    return 0;
}
