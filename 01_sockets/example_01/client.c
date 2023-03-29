/* biblio: https://man7.org/linux/man-pages/man7/socket.7.html */

/*standard symbols */
#include <unistd.h>  

/* sockets include */
#include <netdb.h>      // direcciones de sockets
#include <sys/types.h>  // TODO: ver si hace falta 
#include <sys/socket.h> // uso de sockets
#include <arpa/inet.h>

/* output and strings */
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDRESS "192.168.58.129" /* server IP */
#define PORT 8080 

char buffer_tx[128] = "Hola servidor, soy un cliente";
char buffer_rx[128] = {0};

int main(int argc, char* argv[])
{
    int socketfd = 0; 
    struct sockaddr_in server_addr = {0};

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == socketfd)
    {
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return -1;        
    }

    /* assign IP, PORT */
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS); 
    server_addr.sin_port = htons(PORT); 
    
    if(0 != connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return -1;  
    }

    printf("connected to the server..\n"); 

    /* send test sequences*/
    write(socketfd, buffer_tx, sizeof(buffer_tx));     
    read(socketfd, buffer_rx, sizeof(buffer_rx));
    printf("CLIENT:Received: %s \n", buffer_rx);
   
       
    /* close the socket */
    close(socketfd);
}


