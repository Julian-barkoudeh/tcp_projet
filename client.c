/* 
Le script client.c contient :
    - Une fonction main 
Ces fonctions seront utilisées pour mettre en place la connexion client-serveur avec le protocole TCP
*/

#include <stdio.h>    //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h>    //inet_addr
#include <unistd.h>



/* Fonction main 
But : Fonction main du client TCP
*/
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    
    //Creation du socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect au serveur
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    
    puts("Connected\n");
    recv(sock , server_reply , 2000 , 0);
    puts("Server reply :");
        puts(server_reply);
    // Pour garder la connexion avec le serveur
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        if(strcmp(message,"exit") == 0){
            break;
        }
        // Envoyer des données
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        
        // Recevoir une réponse du serveur
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        
        puts("Server reply :");
        puts(server_reply);
    }
    
    close(sock);
    return 0;
}