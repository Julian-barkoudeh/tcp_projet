/* 
Le script communication.h contient :
    - Deux définitions de type : t_client et t_chaine
    - Toutes les prototypes de fonctions définies dans communication.c
*/

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#define MAX 1000
#define PORT 8080
#define SA struct sockaddr
#define clients 3

// Création du type t_client. Il contient un identifiant et un mot de passe
typedef struct {
char iden[80];
char mdp[80];
} t_client ;

// Création du type t_chaine. Pour créer des chaines de caractère facilement
typedef  struct{
   char tab[80];
}t_chaine;

// Prototype de la fonction identification
int identification(char* iden,char* mdp, t_client BDD_id[clients]);

// Prototype de la fonction cpt
int cpt(char* iden,t_client BDD_id[clients]);

// Prototype de la fonction compte
int compte(char* iden, int numCpt,int BDD_c[3][clients],t_client BDD_id[clients], int op, int somme);

// Prototype de la fonction DixOperations
t_chaine DixOperations(t_chaine operation, int op,char* numCpt, char *somme);

// Prototype de la fonction swap
void swap(char *x, char *y);

// Prototype de la fonction reverse
char* reverse(char *buffer, int i, int j);

// Prototype de la fonction itoa
char* itoa(int num, char* buffer, int base);
#endif