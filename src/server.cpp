#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<iostream>
#include"SDS.h"
#include"COMD.h"
#include"DateTable.h"
#include<thread>
char buf[BUFSIZ];
int strlen(char *str){
    int len=0;
    while(str[len]!='\0')
        len++;
    return len;
}
void clientwork(int clientst){
    while(1)
    {
        int ret=read(clientst,buf,BUFSIZ);
        if(ret==0)
            break;
        int l=0,r=0;
        COMD *first=nullptr;
        COMD *next=nullptr;
        COMD *comd=nullptr;
        first=new COMD();
        next=first;
        int num=0;
        while(buf[l]!=';'){
            if(buf[r]=='\n'){
                comd=new COMD(buf,l,r);
                next->next=comd;
                next=comd;
                l=r+1;
                num++;
            
            }
            if(buf[r]==';'){
                comd=new COMD(buf,l,r);
                next->next=comd;
                next=comd;
                l=r;
                num++;
            
            }
            r++;
        }
        next=first;
        first=first->next;
        delete next;
        while(first!=nullptr){
            
            
            first->run();
            next=first;
            first=first->next;
            delete next;
        }
        buf[0]='!';
        write(clientst,buf,1); 
        for(int i=0;i<ret;i++)
            buf[i]='\0';
        
    }
    COMD Comd;
    Comd.odbsave();
    close(clientst);
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
    server_address.sin_port=htons(8880);
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
    COMD Comd;
    Comd.odbload();
    Comd.odbluach();
    while(1){
        int client=accept(server,NULL,NULL);
        std::thread t(clientwork,client);
        t.detach();
    }
    close(server);


    

}
