/**
 * @file "testRAG.c"
 */

#include <RAG.h>
#include <func.h>
#include <image.h>
/**
 * Test des calculs et des fusions à comparer avec un exemple simple * fait à la main.
 */

int main() {
  char *filename="./../test.ppm";
  image pic;
  int n,m,i,test;
  rag graph;

  pic=FAIRE_image();
  n=4;
  m=4;
  printf("Au moins on est rentré!\n");
  test=image_charger(pic,filename);

  if (test==0) {

    printf("Chargement de %s effectué!\n",filename);
    graph=create_RAG(pic,n,m);
    printf("Toujours en vie?\n");

    for (i=0;i<n*m;i++) {

      printf("\nCaractéristiques du %d e block:\n",i+1);
      /*Attention le block étudié est celui qui correspond 
à la numérotation de l'énoncé pas celle réelle! 
d'où les +1 et -1 un peu partout*/
      printf("M0=%d\n",graph->m[i].M0);
      printf("M1=[%f,%f,%f]\n",graph->m[i].M1[0],
	     graph->m[i].M1[1],
	     graph->m[i].M1[2]);
      printf("father:%d\n",graph->father[i]+1);
      printf("Voisin d'à coté(0=pas de voisin): %d\n",graph->neighbors[i].block+1);
      if (graph->neighbors[i].next!=NULL) {
	printf("Voisin du dessous(-1=pas de voisin): %d\n",graph->neighbors[i].next->block);
      }
      else {
	printf("Pas de voisin du dessous duh\n");
      }
    }
  }
  else {

    if (test==-1) {
      printf("Nom du fichier erronné\n");
    }
    else {

      if (test==-3) {
	printf("Valeurs aberrantes dans l'image\n");
      }
    }
  }
  return 0;
}
      
      
    
    
  
