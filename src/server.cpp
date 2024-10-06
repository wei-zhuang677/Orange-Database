#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<iostream>
#include"SDS.h"
#include"COMD.h"
#include"HashTable.h"
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
    server_address.sin_port=htons(8888);
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
    int client=accept(server,NULL,NULL);
    Comd.odbluach();
    while(1)
    {
      //  printf("read succ\n");
        char c=getchar();
        if(c=='!')
            break;
        int ret=read(client,buf,BUFSIZ);
        int l=0,r=0;
        COMD *first=nullptr;
        COMD *next=nullptr;
        COMD *comd=nullptr;
        first=new COMD();
        next=first;
     //   int num=0;
        while(buf[l]!=';'){
//std::cout<<"first"<<"  "<<first<<"  "<<"first->next"<<"   "<<first->next<<std::endl;
            if(buf[r]=='\n'&&r-l>2){
                comd=new COMD(buf,l,r+1);
                next->next=comd;
                next=comd;
                l=r+1;
             //   num++;
            
            }
            if(buf[r]==';'){
                comd=new COMD(buf,l,r+1);
                next->next=comd;
                next=comd;
                l=r;
            //    num++;
            
            }
            r++;
        }
        //std::cout<<"first"<<"  "<<first<<"  "<<"first->next"<<"   "<<first->next<<std::endl;
       // std::cout<<"next"<<"  "<<next<<"  "<<"next->next"<<"   "<<next->next<<std::endl;
        next=first;
        first=first->next;
        delete next;
      //  std::cout<<num<<std::endl;
        while(first!=nullptr){
            
            
            first->run();
            next=first;
            first=first->next;
            delete next;
        }
        write(client,buf,ret); 
        for(int i=0;i<ret;i++)
            buf[i]='\0';
        
    }
    
    Comd.odbsave();
    close(client);
    close(server);


    

}
