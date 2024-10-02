#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<iostream>
char buf[BUFSIZ];
int strlen(char *str){
    int len=0;
    while(str[len]!='\0')
        len++;
    return len;
}
int main()
{
    int  server=socket(AF_INET,SOCK_STREAM,0);
    if(server<0){
         perror("Error creating socket");
        return -1;
    }

    struct sockaddr_in server_address;
    int addrlen=sizeof(server_address);
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(8000);
    server_address.sin_addr.s_addr=INADDR_ANY;

    if(bind(server,(struct sockaddr*)&server_address,addrlen)){
         perror("bind failed");
         close(server);
         return -1;
    }

    if(listen(server,1)<0)
    {
        perror("listen failed");
        close(server);
        return -1;
    }

    int client=accept(server,NULL,NULL);
    while(1)
    {
        int ret=read(client,buf,BUFSIZ);
        write(client,buf,strlen(buf)+1);
    }
    



    

}