/* 
Le script communication.c contient :
    - Une fonction pour l'identification: identification
    - Une fonction pour récupérer les modifications dans la base de données : cpt
    - Une fonction de gestion du compte : compte
    - Une fonction pour gérer l'historique d'un client : DixOperations
    - Deux fonctions utiles pour la fontion itoa : swap et reverse
    - Une fonction de conversion du type int vers str : itoa
Ces fonctions seront utilisées dans le script server.c et serverUDP.c notamment.
Ce sont les fonctions communes aux deux protocoles.
*/

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "communication.h"
 
/* Fonction identification
But : Gerer l’idenfication d’un client avec son mot de passe
Input :
    - char* iden = Identifiant du client
    - char* mdp = Mot de passe du client
    - t_client BDD_id[clients] = Tableau de données des clients de la banque avec leur mot de passe
                 client1  client2  client3
            id     x1       x2      x3
            mdp    y1       y2      y3
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
   return 0;
}

/* Fonction cpt
But : Récupérer les modifications dans la base de données 
Input :
    - char* iden = Identifiant du client
    - t_client BDD_id[clients] = Tableau de données des clients de la banque avec leur mot de passe
                client1  client2  client3
            id     x1       x2      x3
            mdp    y1       y2      y3
Output : 
    -  i = Indice du tableau de base de données où il y a eu des modifications
*/
int cpt(char* iden,t_client BDD_id[clients]){
     for(int i = 0; i<clients; i++){
         if(strcmp(iden,BDD_id[i].iden) == 0){
             return i;
         }
     }
}

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
                client1  client2  client3
            id     x1       x2      x3
            mdp    y1       y2      y3
    - int op = Type d'opération courante
    - int somme =  Montant de l'opération en cas d'ajout ou de retrait
Output : 
    -  BDD_c[numCpt][cpt] = Tableau de base de données mis à jour en fonction des comptes
*/
int compte(char* iden, int numCpt, int BDD_c[3][clients], t_client BDD_id[clients], int op, int somme){
            printf("op = %d\n",op);
          if(op == 1){
              BDD_c[numCpt][cpt(iden,BDD_id)] = BDD_c[numCpt][cpt(iden,BDD_id)] + somme;
              printf("Nouvelle somme : %d \n", BDD_c[numCpt][cpt(iden,BDD_id)]);
          }
          else if(op == 2){
              BDD_c[numCpt][cpt(iden,BDD_id)] = BDD_c[numCpt][cpt(iden,BDD_id)] - somme;
          }
  return BDD_c[numCpt][cpt(iden,BDD_id)];
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

// Fonction utile pour la fonction itoa
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Fonction utile pour la fonction itoa
char* reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);
 
    return buffer;
}

/* Fonction itoa 
But : Gerer l'historique des opérations d'un client
Input :
    - int num = Nombre à convertir
    - char* buffer = Buffer de conversion vers le type str
    - int base : base de conversion (decimal, hexadecimal etc)
Output : 
    -  char* buffer = Buffer de conversion vers le type str
*/
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
        return buffer;
 
    // consider absolute value of number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) 
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;
 
        n = n / base;
    }
 
    // if number is 0
    if (i == 0)
        buffer[i++] = '0';
 
    // If base is 10 and value is negative, the resulting string 
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}