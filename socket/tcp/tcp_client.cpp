#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

const int port = 3000;
const int MAX = 10; /* biggest number of client_fdected clients */
const char IP[] = "192.168.123.4";

int main(){
    int client_fd=socket(AF_INET,SOCK_STREAM,0);
    if(client_fd == -1){
        perror("socket: ");
        return -1;
    }
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr(IP);
    server.sin_port=htons(port);
    if(connect(client_fd,(struct sockaddr *)&server,sizeof(server))==-1){
        perror("client_fdect: ");
        return -1;
    }
    char sends[105],recvs[105];
    while(1){
        read(STDIN_FILENO,sends,sizeof(sends));
        if(send(client_fd,sends,strlen(sends),0)==-1){
            perror("send: ");
            break;
        }
        if(strcmp(sends,"end\n") == 0){
            puts("the client end.");
            break;
        }
        memset(recvs,0,sizeof(recvs));
        if(read(client_fd,recvs,sizeof(recvs))==-1){
            perror("recv: ");
            break;
        }
        printf("recv message from server: %s",recvs);
        memset(sends,0,sizeof(sends));
    }
    close(client_fd);
    return 0;
}
