// Write C code here
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <protocol.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 4096
#define PORT 8080
#define SA struct sockaddr

typedef struct client_st
{
    int id;
    int dummy;
    char buff[64];
} client_st;

void signal_handler(int sig)
{
}

static int getSelection(void);

void func(int sockfd)
{
    int idx = 0;
    protocol_t proto[] = {
            { 0, 12, "move forward", 0xFFFF},
			{ 1, 9 , "turn left"   , 0xAAAA},
            { 2, 8 , "distance"   , 0xCCCC},
        };
    client_st cl;
    char server_b[MAX];
    
    for (;;)
    {
        bzero(cl.buff, sizeof(cl.buff));
        bzero(server_b, sizeof(server_b));

        idx = getSelection();

        if(idx >= 0){

            if(idx == 9){
                printf("Quitting...\n");
                break; ;
            }

        protocol_mount(&proto[idx], server_b, MAX);       
        

        write(sockfd, server_b, sizeof(server_b));
        if ((strncmp(cl.buff, "quit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
         bzero(cl.buff, sizeof(cl.buff));
        recv(sockfd, cl.buff, sizeof(cl.buff), 0);
        printf("%s\n", cl.buff);
        }      
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    signal(SIGINT, signal_handler);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;    
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}

static int getSelection(void)
{
    int choose;
    printf("[0] - Send to Motor Command.\n");
    printf("[1] - Send to Servo Command.\n");
    printf("[2] - Send to Ultrasound Command.\n");
    printf("[9] - Quit.\n");

    scanf("%d", &choose);

    if(choose >= 0 && choose <= 2){
        return choose;
    }

    return -1;
}