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


/* server parameters defines */
#define SERV_PORT 8080
#define SERV_HOST_ADDR "192.168.58.129" 
#define BUFFER_SIZE 128
#define BACKLOG 5   // max. clients pending connections


int main(int argc, char* argv[])
{
    int socketfd = 0;   // listening socket file discriptor
    int connfd = 0;     // connection socket file descriptor
    unsigned int len;   // use to length save of client structure.

    struct sockaddr_in servaddr = {0};  // server socket structure
    struct sockaddr_in  client = {0};   // client socket structure

    int len_rx = 0;  
    int len_tx = 0; 

    char buffer_tx[BUFFER_SIZE] = "Hola cliente, soy el servidor\n";
    char buffer_rx[BUFFER_SIZE] = {0};
  
    /* socket creation */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == socketfd)
    {
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    }

    /* assign IP, SERV_PORT, IPV4 */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    servaddr.sin_port = htons(SERV_PORT);

    /* Bind socket */
    if(0 != bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    }

    /* start listen */
    if(0 != listen(socketfd, BACKLOG))
    {
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    }

    printf("[SERVER]: Listening on SERV_PORT %d \n\n", ntohs(servaddr.sin_port)); 

    len = sizeof(client); // tranquilamente podria estar arriba en la misma declaracion

    while(1)
    {
        connfd = accept(socketfd, (struct sockaddr *)&client, &len);
        if(-1 != connfd)
        {
            /* read data from a client socket till it is closed */ 
            while(1)
            {
                len_rx = read(connfd, &buffer_rx[0], sizeof(buffer_tx));

                if(len_rx > 0)
                {
                    /* client info */
                    char ipaddres[INET_ADDRSTRLEN];
                    printf("client port: %d\n", ntohs(client.sin_port));
                    printf("client port: %d\n", client.sin_port);
                    printf("client address: %s\n", inet_ntop(AF_INET, (char *)&client.sin_addr.s_addr, ipaddres, sizeof(ipaddres)));    // ta mas fiera esta forma.
                    printf("client address: %s\n", inet_ntoa(client.sin_addr));
                    write(connfd, buffer_tx, strlen(buffer_tx));
                    printf("[SERVER]: %s \n", buffer_rx);                    
                }
                else if(0 == len_rx)
                {
                    printf("[SERVER]: client socket closed \n\n");
                    close(connfd);
                    break;                     
                }
                else
                {
                    fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
                }
            }
        }
        else
        {
            fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
            return -1;   
        }
    }

    return 0;
}
