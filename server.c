#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int bufsize = 1024;
    char *buffer = malloc(bufsize);
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd > 0)
           printf("The Client is connected...\n");
        recv(newsockfd, buffer, bufsize, 0);
        printf("%s\n", buffer);
        write(newsockfd, "HTTP/1.1 200 OK\n", 16);
        write(newsockfd, "Content-length: 46\n", 19);
        write(newsockfd, "Content-Type: text/html\n\n", 25);
        write(newsockfd, "<html><body><H1>Hello world</H1></body></html>",46);
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}