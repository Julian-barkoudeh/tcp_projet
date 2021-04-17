/*
	Simple udp server
*/
#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include"communication.h"
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data
#define MAX 2000
void die(char *s)
{
	perror(s);
	exit(1);
}

int main(void)
{
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
    //int numCpt;
    //int somme;
    int BDD_c[3][clients];
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
    //int op;
    int n = 0;
    char numCpt[10];
    char op[10];
    char somme[80];
    int x = 0;
	//create a UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	
	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));
	
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}

	bzero(send_buff, BUFLEN);
    strcpy(send_buff, "Bienvenue dans la PolyBanque ! Appuyez sur n'importe quel bouton pour vous identifier\n");
    sendto(s, send_buff, strlen(send_buff), 0, (struct sockaddr*) &si_other, slen);
    for (;;)
    { // Boucle à l'infini
        printf("Debut de boucle\n");
        bzero(send_buff, BUFLEN);
        bzero(buff, BUFLEN);
        recvfrom(s, buff, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
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
                strcpy(send_buff, "Operations : Ecrivez \n 1. Pour ajouter une somme \n 2. Pour retirer une somme \n 3. Pour afficher votre solde \n 4. Pour afficher les dernières 10 op\n OP :");
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
        else if (k == 5)
        { // Execution des operation 1 et 2
        printf("boucle k 5\n");
            if (atoi(op) == 1)
            {
                compte(iden, atoi(numCpt), BDD_c, BDD_id, atoi(op), atoi(buff));
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
                    compte(iden, atoi(numCpt), BDD_c, BDD_id, 2, atoi(buff));
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
       sendto(s, send_buff, strlen(send_buff), 0, (struct sockaddr*) &si_other, slen);
        printf("To CLient: %s\n",send_buff);
    }
	close(s);
	return 0;
}
