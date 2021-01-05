#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

const int port = 3000;
const int MAX = 10; /* biggest number of connected clients */
const char IP[] = "192.168.123.4";

int main(){
    int server_fd=socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == -1){
        perror("socket: ");
        return -1;
    }
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY); //inet_addr(IP);   /*  both ways are ok. */
    server.sin_port=htons(port);
    if(bind(server_fd,(struct sockaddr *)&server,sizeof(server))==-1){
        perror("bind: ");
        return -1;
    }
    if(listen(server_fd,MAX) == -1){
        perror("listen: ");
        return -1;
    }

    struct sockaddr_in client;
    int client_len=sizeof(client);
    int conn = accept(server_fd, (struct sockaddr *)&client, &client_len); 
    if(conn == -1){
        perror("accept: ");
        return -1;
    }
    char buff[105];
    while(1){
        memset(buff,0,sizeof(buff));
        if(read(conn,buff,sizeof(buff)) == -1){
            perror("read: ");
            break;
        }
        if(strcmp(buff,"end\n") == 0){
            puts("the server process end. ");
            break;
        }
        printf("recv message from client: %s", buff);
        memset(buff,0,sizeof(buff));
        if(read(STDIN_FILENO,buff,sizeof(buff)) == -1){
            perror("read: ");
            break;
        }
        if(write(conn,buff,strlen(buff)) == -1){
            perror("send: ");
            break;
        }
    }
    close(conn);
    close(server_fd);
    return 0;
}
