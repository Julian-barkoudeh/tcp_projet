#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "communication.h"
#define MAX 1000
#define PORT 8080
#define SA struct sockaddr
#define clients 3

/*
 void func(int sockfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        //printf("buffer avant premier bzero : %s\n",buff);
        bzero(buff, MAX);
  
        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("sockfd : %d\n",sockfd);
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;
  
        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));
        //printf("buffer après write : %s\n",buff);
        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}*/

// Fonction principale 
void func(int sockfd){
    char buff[MAX];
   int k=0;
   char iden[80];
   char mdp[80];
    //int numCpt;
    //int somme;
    int BDD_c[3][clients];
    t_client BDD_id[clients];
	t_chaine dix_op[10];
    strcpy(BDD_id[0].iden,"julian\n");
    strcpy(BDD_id[0].mdp,"1234\n");
    strcpy(BDD_id[1].iden,"camille\n");
    strcpy(BDD_id[1].mdp,"1234\n");
    strcpy(BDD_id[2].iden,"douzet\n");
    strcpy(BDD_id[2].mdp,"1234\n");
    BDD_c[0][0] = 100;
    BDD_c[1][0] = -1;
    BDD_c[2][0] = -1;
    //int op;
    int n=0;
    char numCpt[10];
    char op[10];
    char somme[80];
    int x=0;
    for (;;) {  // Boucle à l'infini
        printf("Debut de boucle\n");
        bzero(buff, MAX);
        read(sockfd, buff, sizeof(buff));
        printf("From CLient : (%s)\n",buff);
        if (strncmp("exit", buff, 4) == 0) {
           printf("Server Exit...\n");
           break;
       }
       else if(k == 0){  //Demander l'identifiant du client 
             k = 1;
             bzero(buff, MAX);
            strcpy(buff,"Veuillez saisir votre numero identifiant");
       }
       else if(k == 1){ // Demander le mdp de client 
            printf("boucle k = 1\n");
            printf("From client: %s\n", buff);
            k = 2;
           strcpy(iden,buff);
           bzero(buff, MAX);
           strcpy(buff,"Veuillez saisir votre mdp");
       }
       else if(k == 2){ // 1. Redemander l'identifiant ou le mdp  2.  demander le numero de compte
            printf("boucle k = 2\n");
           strcpy(mdp,buff);
           bzero(buff, MAX);
           if(identification(iden,mdp,BDD_id) == 0){
               k = 0;
               strcpy(buff,"Identifiant inexistant");
           }
           else if(identification(iden,mdp,BDD_id) == 1){
               k = 1;
               strcpy(buff,"Mauvais mdp");
           }
           else if(identification(iden,mdp,BDD_id) == 2){
               k = 3;
               strcpy(buff,"Selectionnez votre numéro de compte");
           }
       }
       else if(k == 3){ // 1. Redemander le numero de compte 2. Demander l'operation
           strcpy(numCpt,buff);
           printf("From client: %s\n", buff);
		    bzero(buff, MAX);
           if(compte(iden,atoi(numCpt),BDD_c,BDD_id,3,0) == -1){
              strcpy(buff,"Ce compte n'existe pas, selectionnez un autre compte\n compte:");
           }
           else{
               k = 4;
               strcpy(buff,"operations(1:Ajout 2:Retrait 3:Solde 4:derniers 10 op\n OP :");
           }
       }
       else if(k == 4){   // 1. Execution des operations 3 et 4 2. Demander le montant pour les operations 1 et 2
           strcpy(op,buff);
		bzero(buff, MAX);
           if(atoi(op) == 3){
               strcpy(somme,itoa(compte(iden,atoi(numCpt),BDD_c,BDD_id,3,0),somme,10));
               if(n==10){
                   for(int i=1;i<10;i++){
                      strcpy(dix_op[i-1].tab,dix_op[i].tab);
                   }
                   n=9;
               }
               dix_op[n]= DixOperations(dix_op[n],atoi(op),numCpt,somme);
               n++;
               strcpy(buff,"Votre solde est de ");
               strcat(buff,somme);
           }
		    // Affichage des 10 dernieres operations d’un client
		    else if (atoi(op) == 4){
                for(int i =0; i<10;i++){
                    strcpy(buff,dix_op[i].tab);
                }
		    }
           else {
               strcpy(buff,"Montant en euro : ");
               k = 5;
           }
       }
       else if(k == 5){ // Execution des operation 1 et 2
           if(atoi(op) == 1){
               compte(iden,numCpt,BDD_c,BDD_id,op,atoi(buff));
               strcpy(somme,buff);
               if(n==10){
                   for(int i=1;i<10;i++){
                      strcpy(dix_op[i-1].tab,dix_op[i].tab);
                   }
                   n=9;
               }
               dix_op[n]= DixOperations(dix_op[n],atoi(op),numCpt,somme);
               n++;
               bzero(buff, MAX);
               strcpy(buff,"Montant ajouté ");
           }
           else if(atoi(op) == 2){
               if(compte(iden,atoi(numCpt),BDD_c,BDD_id,3,0) - atoi(buff) <= 0){
                   bzero(buff, MAX);
                   strcpy(buff,"Solde insufisant de  ");
                   strcat(buff,itoa(compte(iden,numCpt,BDD_c,BDD_id,3,0),somme,10));
                   strcat(buff,"\n Entrez un montant plus petit : ");
               }
               else{
                   compte(iden,numCpt,BDD_c,BDD_id,2,atoi(buff));
                    strcpy(somme,buff);
                   if(n==10){
                        for(int i=1;i<10;i++){
                            strcpy(dix_op[i-1].tab,dix_op[i].tab);
                        }
                        n=9;
                    }
                   dix_op[n]= DixOperations(dix_op[n],atoi(op),numCpt,somme);
                   n++;
                   bzero(buff, MAX);
                   strcpy(buff,"Montant retiré");
               }
           }
       }
        write(sockfd, buff, sizeof(buff));
        printf("To CLient: %s\n", buff);
       
    }
}
 // Driver function
int main()
{
   int sockfd, connfd, len;
   struct sockaddr_in servaddr, cli;
    // socket create and verification
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd == -1) {
       printf("socket creation failed...\n");
       exit(0);
   }
   else
       printf("Socket successfully created..\n");
   bzero(&servaddr, sizeof(servaddr));
    // assign IP, PORT
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(PORT);
    // Binding newly created socket to given IP and verification
   if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
       printf("socket bind failed...\n");
       exit(0);
   }
   else
       printf("Socket successfully binded..\n");
    // Now server is ready to listen and verification
   if ((listen(sockfd, 5)) != 0) {
       printf("Listen failed...\n");
       exit(0);
   }
   else
       printf("Server listening..\n");
   len = sizeof(cli);
    // Accept the data packet from client and verification
   connfd = accept(sockfd, (SA*)&cli, &len);
   if (connfd < 0) {
       printf("server acccept failed...\n");
       exit(0);
   }
   else
       printf("server acccept the client...\n");
    // Function for chatting between client and server
   func(connfd);
    // After chatting close the socket
   close(sockfd);
}

