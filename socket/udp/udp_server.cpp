#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 3000

int main(){ 
    int s=socket(AF_INET,SOCK_DGRAM,0);
        if(s==-1){
        perror("create socket error: ");
        exit(1);
    }
    struct sockaddr_in serv,client;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;   
    serv.sin_addr.s_addr=inet_addr("10.21.1.229");//htol(INADDR_ANY); 
    serv.sin_port=htons(PORT);  //网络字节序
    if(bind(s,(struct sockaddr*)&serv,sizeof(serv))==-1){//绑定套接字和地址
        perror("bind error: ");
        exit(1);
    }
    while(1){
        char buff[105];
        int addr_len=sizeof(client);
        int ret=recvfrom(s,buff,105,0,(struct sockaddr *)&client,&addr_len); // once success, we get client.
        if(ret==-1){
            perror("recvfrom error: ");
            break;
        }
        buff[ret]=0;
        if(strcmp(buff,"end\n")==0){
            printf("server process end.\n");
            break;
        }
        printf("receive message from client: %s",buff);
            ret=read(STDIN_FILENO,buff,sizeof(buff));
        if(ret<0){
            perror("read error: ");
            break;
        }
        if(sendto(s,buff,ret,0,(struct sockaddr *)&client,addr_len)==-1){
            perror("sendto error: ");
            break;
        }
    }
    close(s);
    return 0;
}
