/*
Le script serverUDP.c contient :
    - Une fonction die en cas de problème
    - Une fonction main qui contient tout l'algorithme
Ces fonctions seront utilisées pour mettre en place la connexion client-serveur avec le protocole UDP
*/
#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include"communication.h"
#define BUFLEN 512	//Longueur max du buffer
#define PORT 8888	//Port d'écoute pour les données entrantes
#define MAX 2000

/* Fonction die 
But : Exit en cas de problèmes
Input :
    - char *s
Output : void
*/
void die(char *s)
{
	perror(s);
	exit(1);
}

/* Fonction main 
But : Fonction main du projet avec protocole UDPN coté serveur
*/
int main(void)
{
    //Initialisation des variables nécesaires
	struct sockaddr_in si_me, si_other;
	int s, i, slen = sizeof(si_other) , recv_len;
    int read_size;
	char buf[BUFLEN];
	char message[BUFLEN];
	char buff[BUFLEN];
    char send_buff[BUFLEN];
    int k = 0;
    char iden[80];
    char mdp[80];
    int BDD_c[3][clients];

    // Initialisation base de données client et compte
    t_client BDD_id[clients];
    t_chaine dix_op[10];
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
	//Création du socket UDP
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	
	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//Liaison du socket au port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}

	bzero(send_buff, BUFLEN);

    for (;;)
    { // Boucle infinie
        printf("Debut de boucle\n");
        bzero(send_buff, BUFLEN);
        bzero(buff, BUFLEN);
        recvfrom(s, buff, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
        printf("From Client : (%s)\n", buff);
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
            printf("boucle k = 0\n");
            k = 1;
            //bzero(send_buff, MAX);
            strcpy(send_buff, "Veuillez saisir votre numero identifiant");
             printf("Buffer = %s\n",send_buff);
        }
        // Demander le mdp du client
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
        else if (k == 3)
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
                bzero(somme, 10);
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
            // Demander de confirmer la nouvelle operation
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
        sendto(s, send_buff, strlen(send_buff), 0, (struct sockaddr*) &si_other, slen);
        printf("To Client: %s\n",send_buff);
    }
	close(s);
	return 0;
}