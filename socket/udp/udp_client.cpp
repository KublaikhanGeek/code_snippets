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
        struct sockaddr_in serv;
        bzero(&serv,sizeof(serv));
        serv.sin_family=AF_INET;   
        serv.sin_addr.s_addr = inet_addr("10.21.1.229");    //htonl(INADDR_ANY); 
        serv.sin_port=htons(PORT);
        char buff[105];
        int ret;
        while(1){
             ret=read(STDIN_FILENO,buff,105);
             if(ret==-1){
                  perror("read error: ");
                  break;
             }
             buff[ret]=0;
             sendto(s,buff,strlen(buff),0,(struct sockaddr *)&serv,sizeof(serv));
             if(strcmp(buff,"end\n")==0){
                  printf("client process end.\n");
                  break;
             }
             int addr_len=sizeof(serv);
             if((ret=recvfrom(s,buff,105,0,(struct sockaddr *)&serv,&addr_len))==-1){
                  perror("recvform error: ");
                  break;
             }
             buff[ret]=0;
             printf("receive message from server: %s",buff);
        }
        close(s);
        return 0;
}
