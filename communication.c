/* 
Le script communication.c contient :
    - Une fonction pour l'identification: identification
    - Une fonction de gestion du compte : compte
    - Une fonction pour gérer l'historique d'un client : DixOperations
    - Une fonction de conversion du type int vers str : itoa
Ces fonctions seront utilisées dans le script server.c notamment.
*/

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
But : Gerer l’idenfication d’un client avec son mot de passe
Input :
    - char* iden = Identifiant du client
    - char* mdp = Mot de passe du client
    - t_client BDD_id[clients] = Tableau de données des clients de la banque avec leur mot de passe
        [ DESCRIPTION FONCTIONNEMENT DU TABLEAU LIGNE COLONNES]
Output : La fonction renvoie :
    - 0 : si l'identifiant est mauvais
    - 1 : si le mdp est mauvais
    - 2 : si tout va bien
*/
int identification(char* iden,char* mdp, t_client BDD_id[clients]){
    printf("identification\n");
   for(int j=0; j<clients; j++){
       if(strcmp(BDD_id[j].iden,iden) == 0){
           if(strcmp(BDD_id[j].mdp,mdp) == 0){
               printf("L'identification s'est déroulé avec succès \n");
               return 2 ;
           }
           else{
               printf("L'identification avec ce mot de passe a échoué\n");
               return 1;
           }
       }
   }
  
   printf("identification identifiant non reussie avec iden :(%s)\n",iden);
        //for(int j=0; j<3;j++){
            //printf("Client %d\n identifiant: (%s)\t mdp: %s\n",j,BDD_id[j].iden,BDD_id[j].mdp);
        //}
   return 0;
}

// Fonction d'interaction avec le compte 
// op : 1 ajouter une somme
//      2 Retirer une somme
//      3 Afiicher le solde
/* Fonction compte
But : Fonction d'interaction avec le compte 
Input :
    - char* iden = Identifiant du client
    - int numCpt = Numéro du compte à utiliser
    - int BDD_c[3][clients] = Tableau de données des clients et de l'état de leur compte
        Il est construit de cette manière :
                    client1  client2  client3
            compte1    x1       x2      x3
            compte2    y1       y2      y3
            compte3    z1       z2      z3
    - t_client BDD_id[clients] = Tableau de données des clients de la banque avec leur mot de passe
        [ DESCRIPTION FONCTIONNEMENT DU TABLEAU LIGNE COLONNES]
    - int op = Type d'opération courante
    - int somme =  Montant de l'opération en cas d'ajout ou de retrait
Output : 
    -  BDD_c[numCpt][cpt] = Tableau de base de données mis à jour en fonction des comptes
*/
int compte(char* iden, int numCpt, int BDD_c[3][clients], t_client BDD_id[clients], int op, int somme){
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

/* Fonction DixOperations
But : Gerer l'historique des opérations d'un client
Input :
    - t_chaine operation = Chaine récapitulative des actions
    - int op = Type d'opération courante
    - char* numCpt = Numéro du compte utilisé
    - char *somme =  Montant de l'opération en cas d'ajout ou de retrait
Output : 
    -  t_chaine operation = Chaine récapitulative des 10 dernières actions avec le compte associé.
*/
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

/* Fonction itoa 
But : Gerer l'historique des opérations d'un client
Input :
    - int num = Nombre à convertir
    - char* buffer = Buffer de conversion vers le type str
Output : 
    -  char* buffer = Buffer de conversion vers le type str
*/
char* itoa(int num, char* buffer)   
{  
int current = 0;  
if (num == 0) {  
    buffer[current++] = '0';  
    buffer[current] = '\0';  
    return buffer;  
}  
}