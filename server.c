/* 
Le script server.c contient :
    - Une fonction pour le déroulement de la connexion: fonc
    - Une fonction main
Ces fonctions seront utilisées pour mettre en place la connexion client-serveur avec le protocole TCP
*/

#include "communication.h"
#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#define MAX 2000

/* Fonction func 
But : Contient tout le déroulement de l'algorithme pour effectuer les opérations
Input :
    - int client_sock = Socket du client
Output : void
*/
void func(int client_sock)
{
    //Initialisation des variables nécesaires
    char buff[MAX];
    char send_buff[MAX];
    int k = 0;
    char iden[80];
    char mdp[80];
    int BDD_c[3][clients];
    t_client BDD_id[clients];
    t_chaine dix_op[10];

    // On initialise les 3 clients et leurs comptes respectifs
    strcpy(BDD_id[0].iden, "julian");
    strcpy(BDD_id[0].mdp, "1234");
    strcpy(BDD_id[1].iden, "camille");
    strcpy(BDD_id[1].mdp, "1234");
    strcpy(BDD_id[2].iden, "maria");
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

    int n = 0;
    char numCpt[10];
    char op[10];
    char somme[80];
    int x = 0;
    int read_size;
    // Premier message de bienvenue pour initialiser dans connexion
    strcpy(send_buff, "Bienvenue dans la BarkouDouzetBanque ! Appuyez sur n'importe quel bouton pour vous identifier\n");
    write(client_sock, send_buff, sizeof(send_buff));
    for (;;)
    { // Boucle infinie
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
        // Demander l'identifiant du client
        else if (k == 0)
        { 
            printf("Boucle k = 0\n");
            k = 1;
            strcpy(send_buff, "Veuillez saisir votre numero identifiant");
        }
        // Demander le mot de passe du client
        else if (k == 1)
        {
            printf("boucle k = 1\n");
            k = 2;
            strcpy(iden, buff);
            bzero(send_buff, MAX);
            strcpy(send_buff, "Veuillez saisir votre mot de passe");
        }
        // Deux possibilités :
        // 1. Redemander l'identifiant ou le mdp si l'identification s'est mal déroulée
        // 2.  demander le numero de compte si l'identification est un succès
        else if (k == 2)
        {
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
                strcpy(send_buff, "Mauvais mot de passe\n Resaisissez votre mot de passe\n");
            }
            else if (identification(iden, mdp, BDD_id) == 2)
            {
                k = 3;
                strcpy(send_buff, "Selectionnez votre numéro de compte");
            }
        }
        // Deux possibilités :
        // 1. Redemander le numero de compte s'il était mauvais
        // 2. Demander l'operation à effectuer si le compte est existant
        else if 
        (k == 3)
        {
            strcpy(numCpt, buff);
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
        // Quasiment le même cas que k=3 mais on ne l'utilise pas au même endroit, simplement pour une question de logique dans les messages
        else if (k == 31)
        {
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
        // Deux étapes :
        // 1. Execution des operations 3 et 4 (cad afficher le solde ou les 10 dernières op)
        // 2. Demander le montant pour les operations 1 et 2 (en cas d'ajout ou de retrait)
        else if (k == 4)
        {
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
        // Execution des operation 1 et 2 (ajout ou retrait)
        else if (k == 5)
        {
        printf("boucle k 5\n");
            // Si c'est un ajout
            if (atoi(op) == 1)
            {
                BDD_c[atoi(numCpt)][cpt(iden,BDD_id)] = compte(iden, atoi(numCpt), BDD_c, BDD_id, atoi(op), atoi(buff));
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
                strcpy(send_buff, "Montant ajouté ! \n Sélectionnez votre prochaine operation : \n 1.Changer de client \n 2. Changer de compte \n 3. Une autre opération\n");
                k = 6;
            }
            // Si c'est un retrait
            else if (atoi(op) == 2)
            {
                if (compte(iden, atoi(numCpt), BDD_c, BDD_id, 3, 0) - atoi(buff) < 0)
                {
                    bzero(send_buff, MAX);
                    strcpy(send_buff, "Solde insufisant de  ");
                    strcat(send_buff, itoa(compte(iden, atoi(numCpt), BDD_c, BDD_id, 3, 0), somme, 10));
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
                bzero(op,10);
                bzero(somme,80);
                if(atoi(buff) == 1){
                    strcpy(send_buff, "Confirmez vous cette opertaion ? o|n");
                    k = 0;
                    bzero(iden, 80);
                    bzero(mdp, 80);
                }
                else if(atoi(buff) == 2){
                    strcpy(send_buff, "Veuillez saisire le numéro de votre compte\n");
                    k=3;
                    bzero(numCpt,10);
                }
                else if(atoi(buff) == 3){
                    strcpy(send_buff, "Confirmez vous cette opertaion ? o|n");
                     k=31;
                }
            }
        write(client_sock, send_buff, sizeof(send_buff));
        printf("To Client: %s\n",send_buff);
    }
}

/* Fonction main 
But : Fonction main du projet coté serveur avec TCP
*/
int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];

    //Création du socket TCP
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Préparation de la structure sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Etablissement de la liaison
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        // Message d'erreur
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Mode écoute
    listen(socket_desc, 3);

    //En attente d'une connexion entrante
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    // Acceptation  d'une connexion entrante d'un client
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