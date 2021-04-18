/* 
Le script clientUDP.c contient :
    - Une fonction main 
Ces fonctions seront utilisées pour mettre en place la connexion client-serveur avec le protocole UDP
*/
#include<stdio.h>    //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512    //Max length of buffer
#define PORT 8888    //The port on which to send data

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
But : Fonction main du clientUDP
*/
int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    printf("Bienvenue dans la BarkouDouzetBanque! Appuyez sur n'importe quel bouton pour vous identifier\n");
    while(1)
    {

        printf("Enter message : ");
        gets(message);
        
        //Envoyer le message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }
        if(strcmp(message,"exit") == 0){
			break; 
		}
        // Reception de la réponse et affichage de cette dernière
        // On vide le buffer de la potentielle precedente réponse
        memset(buf,'\0', BUFLEN);
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }
        
        puts(buf);
    }

    close(s);
    return 0;
}