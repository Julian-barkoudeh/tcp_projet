#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "communication.h";

 
/* Fonction identification
But : Gerer l’idenfication d’un client a un compte bancaire
Entree :
char* iden
char* mdp
// La fonction envoie :
// 0 : si l'identifiant est mauvais
// 1  :si le mdp est mauvais
// 2 : si tout va bien
*/
int identification(char* iden,char* mdp, t_client BDD_id[clients]){
    printf("identifiaction\n");
   for(int j=0; j<clients; j++){
       if(strcmp(BDD_id[j].iden,iden) == 0){
           if(strcmp(BDD_id[j].mdp,mdp) == 0){
               printf("identifiaction mdp reussie \n");
               return 2 ;
           }
           else{
               printf("identifiaction mdp non reussie \n");
               return 1;
           }
       }
   }
  
   printf("identifiaction identifiant non reussie avec iden :(%s)\n",iden);
        //for(int j=0; j<3;j++){
            //printf("Client %d\n identifiant: (%s)\t mdp: %s\n",j,BDD_id[j].iden,BDD_id[j].mdp);
        //}
   return 0;
}
// Fonction d'interaction avec le compte 
// op : 1 ajouter une somme
//      2 Retirer une somme
//      3 Afiicher le solde
int compte(char* iden, int numCpt,int BDD_c[3][clients],t_client BDD_id[clients], int op, int somme){
   int cpt;
  for(int i = 0; i<clients; i++){
      if(strcmp(iden,BDD_id[i].iden) == 0){
           cpt = i;
          if(op == 1){
              BDD_c[numCpt][i] == BDD_c[numCpt][i] + somme;
          }
          else if(op == 2){
              BDD_c[numCpt][i] == BDD_c[numCpt][i] - somme;
          }
      }
  }
  return BDD_c[numCpt][cpt];
}
//Fonction de gestion de l'historique des operations
t_chaine DixOperations(t_chaine operation, int op,char* numCpt, char *somme){
    strcat(operation.tab,"Compte ");
    strcat(operation.tab,numCpt);
    strcat(operation.tab,": ");
    if(op == 1){
       strcat(operation.tab,"+");
    }
    else if(op == 2){
       strcat(operation.tab,"-");
    }
    strcat(operation.tab,somme);
    strcat(operation.tab,"\n");

    return operation; 
}
char* itoa(int num, char* buffer, int base)   
{  
int current = 0;  
if (num == 0) {  
buffer[current++] = '0';  
buffer[current] = '\0';  
return buffer;  
}  
}
