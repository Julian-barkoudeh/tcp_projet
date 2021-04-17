#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#define MAX 1000
#define PORT 8080
#define SA struct sockaddr
#define clients 3
typedef struct {
char iden[80];
char mdp[80];
} t_client ;  
typedef  struct{
   char tab[80];
}t_chaine;

int identification(char* iden,char* mdp, t_client BDD_id[clients]);
int cpt(char* iden,t_client BDD_id[clients]);
int compte(char* iden, int numCpt,int BDD_c[3][clients],t_client BDD_id[clients], int op, int somme);
t_chaine DixOperations(t_chaine operation, int op,char* numCpt, char *somme);
void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
char* itoa(int num, char* buffer, int base);
#endif