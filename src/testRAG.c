/**
 * @file "testRAG.c"
 */

#include <RAG.h>
#include <func.h>
/**
 * Test des calculs et des fusions à comparer avec un exemple simple * fait à la main.
 */

int main() {
  char *filename="test.ppm";
  image pic;
  int n,m,i;
  rag graph;
  n=2;
  m=2;

  if (!image_charger(pic,filename)) {
    graph=create_RAG(pic,n,m);
    for (i=0;i>n*m;i++) {
      printf("Caractéristiques du %d e block:\n",i);
      printf("M0=%f\n",graph.m[i].M0);
      printf("M1=[%f,%f,%f]\n",graph.m[i].M1[0],
	     graph.m[i].M1[1],
	     graph.m[i].M1[2]);
      printf("father:%d\n",graph.father[i]);
      printf("Voisin d'à coté: %d\n",graph.neighbors[i].block);
      printf("Voisin du dessous: %d\n",
	     graph.neighbors[i].next.block);
    }
  }
  return 0;
}
      
      
    
    
  
