#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1)
    {
        perror("socket failed!");
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = 10000;

    int error = connect(socketFd, (struct sockaddr*) &server, sizeof(server));
    if(error == -1)
    {
        perror("connect failed!");
        exit(1);
    }

    int inputSize = 500;
    int readBytes;
    char buffer[inputSize];

    readBytes = recv(socketFd, buffer, sizeof(buffer), 0);
    if(readBytes <= 0)
    {
        perror("recv failed!");
        exit(1);
    }

    printf("String from server is %s\n", buffer);

    printf("Enter a string: ");
    scanf("%s", buffer);

    error = send(socketFd, (void*) buffer, sizeof(buffer), 0);
    if(error == -1)
    {
          perror("send failed!");
          exit(1);
    }

    readBytes = recv(socketFd, buffer, sizeof(buffer), 0); //MSG_DONTWAIT
    if(readBytes <= 0)
    {
        perror("recv failed!");
        exit(1);
    }

    printf("String from server is %s\n", buffer);

    close(socketFd);
    printf("Connection closed...\n");

    return 0;
}
