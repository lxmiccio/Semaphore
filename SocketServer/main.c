#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

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

    int error = bind(socketFd, (struct sockaddr*) &server, sizeof(server));
    if(error == -1)
    {
        perror("bind failed!");
        exit (1);
    }

    error = listen(socketFd, 1);
    if(error == -1)
    {
        perror("listen failed!");
        exit (1);
    }

    while(1)
    {
        struct sockaddr_in client;
        socklen_t clientLength = sizeof(client);

        int clientFd = accept(socketFd, (struct sockaddr*) &client, &clientLength);
        if(clientFd < 0)
        {
            perror("accept failed!");
            exit(1);
        }

        printf("Connection accepted\n");
        send(clientFd, "Welcome!\n", 500, 0);

        int inputSize = 500;
        int readBytes;
        char clientInput[inputSize];

        while((readBytes = recv(clientFd, clientInput, inputSize, 0)) > 0)
        {
            printf("Converting the string %s...\n", clientInput);
            for(int i = 0; i < readBytes; ++i)
            {
                clientInput[i] = toupper(clientInput[i]);
            }
            printf("Sending the string %s back to the client...\n", clientInput);
            send(clientFd, clientInput, readBytes, 0);
        }

        close(clientFd);
        printf("Connection closed...\n");
    }

    return 0;
}
