/* 
Le script server.c contient :
    - Une fonction pour le déroulement de la connexion: fonc
    - Une fonction main
Ces fonctions seront utilisées pour mettre en place la connexion client-serveur.
*/

//#include <stdio.h>
//#include <netdb.h>
//#include <netinet/in.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/socket.h>
//#include <sys/types.h>
#include "communication.h"
#define MAX 2000
//#define PORT 8080
//#define SA struct sockaddr
//#define clients 3

#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

/* Fonction func 
But : Contient tout le déroulement de l'algorithme pour effectuer les opérations
Input :
    - int client_sock = Socket du client
Output : void
*/
void func(int client_sock)
{
    char buff[MAX];
    char send_buff[MAX];
    int k = 0;
    char iden[80];
    char mdp[80];
    //int numCpt;
    //int somme;
    int BDD_c[3][clients];
    t_client BDD_id[clients];
    t_chaine dix_op[10];
    strcpy(BDD_id[0].iden, "julian");
    strcpy(BDD_id[0].mdp, "1234");
    strcpy(BDD_id[1].iden, "camille");
    strcpy(BDD_id[1].mdp, "1234");
    strcpy(BDD_id[2].iden, "douzet");
    strcpy(BDD_id[2].mdp, "1234");
    for(int i=0;i<clients;i++){
        for(int j=0;j<3;j++){
            BDD_c[i][j] = -1;
        }
    }
     BDD_c[0][0] = 100;
     BDD_c[1][0] = 3000;
     BDD_c[0][1] = 500;
     BDD_c[0][2] = 10000;
    //int op;
    int n = 0;
    char numCpt[10];
    char op[10];
    char somme[80];
    int x = 0;
    int read_size;
    strcpy(send_buff, "Bienvenue dans la PolyBanque ! Appuyez sur n'importe quel bouton pour vous identifier\n");
     write(client_sock, send_buff, sizeof(send_buff));
    for (;;)
    { // Boucle à l'infini
        printf("Debut de boucle\n");
        bzero(send_buff, MAX);
         bzero(buff, MAX);
        read_size = recv(client_sock, buff, 2000, 0);
        printf("From CLient : (%s)\n", buff);
       if(strcmp(buff,"exit") == 0)
       {
           break;
       }
       else if(strcmp(buff,"prev") ==0)
       {
           if(k>0){
            k = k -1;
           }
       }
       else if (k == 0)
        { //Demander l'identifiant du client
            printf("boucle k = 0\n");
            k = 1;
            //bzero(send_buff, MAX);
            strcpy(send_buff, "Veuillez saisir votre numero identifiant");
             printf("Buffer = %s\n",send_buff);
        }
        else if (k == 1)
        { // Demander le mdp de client
            printf("boucle k = 1\n");
            k = 2;
            strcpy(iden, buff);
            bzero(send_buff, MAX);
            strcpy(send_buff, "Veuillez saisir votre mot de passe");
        }
        else if (k == 2)
        { // 1. Redemander l'identifiant ou le mdp  2.  demander le numero de compte
            printf("boucle k = 2\n");
            strcpy(mdp, buff);
            bzero(send_buff, MAX);
            if (identification(iden, mdp, BDD_id) == 0)
            {
                k = 1;
                strcpy(send_buff, "Identifiant inexistant\n Resaisissez votre identifiant\n");
            }
            else if (identification(iden, mdp, BDD_id) == 1)
            {
                k = 1;
                strcpy(send_buff, "Mauvais mot de passe\n Resaisissez votre mot de passe\n");
            }
            else if (identification(iden, mdp, BDD_id) == 2)
            {
                k = 3;
                strcpy(send_buff, "Selectionnez votre numéro de compte");
            }
        }
        else if (k == 3)
        { // 1. Redemander le numero de compte 2. Demander l'operation
            strcpy(numCpt, buff);
            bzero(send_buff, MAX);
            if (compte(iden, atoi(numCpt), BDD_c, BDD_id, 3, 0) == -1)
            {
                strcpy(send_buff, "Ce compte n'existe pas, selectionnez un autre compte\n Compte:");
            }
            else
            {
                k = 4;
                strcpy(send_buff, "Operations possibles (1:Ajout 2:Retrait 3:Solde 4:les derniers 10 op\n OP :");
            }
        }
        else if (k == 31)
        { // 1. Redemander le numero de compte 2. Demander l'operation
            bzero(send_buff, MAX);
            if (compte(iden, atoi(numCpt), BDD_c, BDD_id, 3, 0) == -1)
            {
                strcpy(send_buff, "Ce compte n'existe pas, selectionnez un autre compte\n Compte:");
            }
            else
            {
                k = 4;
                strcpy(send_buff, "Operations : Ecrivez \n 1. Pour ajouter une somme \n 2. Pour retirer une somme \n 3. Pour afficher votre solde \n 4. Pour afficher les dernières 10 op\n OP :");
            }
        }
        else if (k == 4)
        { // 1. Execution des operations 3 et 4 2. Demander le montant pour les operations 1 et 2
            strcpy(op, buff);
            bzero(send_buff, MAX);
            if (atoi(op) == 3)
            {
                strcpy(somme, itoa(compte(iden, atoi(numCpt), BDD_c, BDD_id, 3, 0), somme, 10));
                if (n == 10)
                {
                    for (int i = 1; i < 10; i++)
                    {
                        strcpy(dix_op[i - 1].tab, dix_op[i].tab);
                    }
                    n = 9;
                }
                dix_op[n] = DixOperations(dix_op[n], atoi(op), numCpt, somme);
                n++;
                strcpy(send_buff, "Votre solde est de ");
                strcat(send_buff, somme);
                strcat(send_buff," euro(s)");
                strcat(send_buff, "\n Sélectionnez votre prochaine operation : \n 1.Changer de client\n 2. Changer de compte\n 3. Une autre opération\n");
                k = 6;
            }
            // Affichage des 10 dernieres operations d’un client
            else if (atoi(op) == 4)
            {
                strcpy(send_buff,"Dernières dix opérations sur vos comptes : \n");
                for (int i = 0; i < 10; i++)
                {
                    strcat(send_buff, dix_op[i].tab);
                }
                 strcat(send_buff, "\n Sélectionnez prochaine operation : \n 1.Changer de client\n 2. Changer de compte\n 3. Une autre opération\n");
                k = 6;
            }
            else
            {
                strcpy(send_buff, "Montant en euro : ");
                k = 5;
            }
        }
        else if (k == 5)
        { // Execution des operation 1 et 2
        printf("boucle k 5\n");
            if (atoi(op) == 1)
            {
                BDD_c[atoi(numCpt)][cpt(iden,BDD_id)] = compte(iden, atoi(numCpt), BDD_c, BDD_id, atoi(op), atoi(buff));
                printf("problème 0 ici\n");
                strcpy(somme, buff);
                printf("problème 1 ici\n");
                if (n == 10)
                {
                    for (int i = 1; i < 10; i++)
                    {
                        strcpy(dix_op[i - 1].tab, dix_op[i].tab);
                    }
                    n = 9;
                }
                dix_op[n] = DixOperations(dix_op[n], atoi(op), numCpt, somme);
                n++;
                bzero(send_buff, MAX);
                strcpy(send_buff, "Montant ajouté ! \n Sélectionnez votre prochaine operation : \n 1.Changer de client \n 2. Changer de compte \n 3. Une autre opération\n");
                k = 6;
            }
            else if (atoi(op) == 2)
            {
                if (compte(iden, atoi(numCpt), BDD_c, BDD_id, 3, 0) - atoi(buff) < 0)
                {
                    bzero(send_buff, MAX);
                    strcpy(send_buff, "Solde insufisant de  ");
                    printf("problème 1 ici\n");
                    strcat(send_buff, itoa(compte(iden, atoi(numCpt), BDD_c, BDD_id, 3, 0), somme, 10));
                    printf("problème là\n");
                    strcat(send_buff, "\n Entrez un montant plus petit : ");
                }
                else 
                {
                    BDD_c[atoi(numCpt)][cpt(iden,BDD_id)] = compte(iden, atoi(numCpt), BDD_c, BDD_id, 2, atoi(buff));
                    bzero(somme, 10);
                    strcpy(somme, buff);
                    if (n == 10)
                    {
                        for (int i = 1; i < 10; i++)
                        {
                            strcpy(dix_op[i - 1].tab, dix_op[i].tab);
                        }
                        n = 9;
                    }
                    dix_op[n] = DixOperations(dix_op[n], atoi(op), numCpt, somme);
                    
                    n++;
                    bzero(send_buff, MAX);
                    strcpy(send_buff, "Montant retiré ! \n Sélectionnez votre prochaine operation : \n 1.Changer de client \n 2. Changer de compte \n 3. Une autre opération\n");
                    k=6;
                }
            }
        }
            else if(k == 6){
                bzero(send_buff, MAX);
                strcpy(send_buff, "Confirmez vous cette opertaion ? o|n");
                bzero(op,10);
                bzero(somme,80);
                if(atoi(buff) == 1){
                    k = 0;
                    bzero(iden, 80);
                    bzero(mdp, 80);
                }
                else if(atoi(buff) == 2){
                    k=3;
                    bzero(numCpt,10);
                }
                else if(atoi(buff) == 3){
                     k=31;
                }
            printf("sending buffer : (%s)\n",send_buff);
            }
        //bzero(buff, MAX);
        write(client_sock, send_buff, sizeof(send_buff));
        printf("To CLient: %s\n",send_buff);
    }
}

/* Fonction main 
But : Fonction main du projet
*/
int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
    char message[80];
    func(client_sock);
    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}