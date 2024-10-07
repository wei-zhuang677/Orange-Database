#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<iostream>
#include"SDS.h"
#include"COMD.h"
#include"DateTable.h"
#include<thread>
const char beginn[20]="begin";
const char committ[20]="commit";
SDS com_commit(committ);
SDS com_beginn(beginn);
char buf[BUFSIZ];
int strlen(char *str){
    int len=0;
    while(str[len]!='\0')
        len++;
    return len;
}
int clientt[1000];
void work(COMD* first,int clientst){
    printf("Working\n");
    while(first!=nullptr){            
        first->run(clientst);
        COMD* next=first;
        first=first->next;
        delete next;
    }
}
void clientwork(int client){
    int* clientst=new int(client);
       
    int ret;
    int num=0;
    COMD *first=nullptr;
    COMD *next=nullptr;
    COMD *comd=nullptr;
    while(1){
        ret=read(*clientst,buf,BUFSIZ);
        printf("read\n");
        if(ret==0)
            break;
        int l=0,r=0;
        while(buf[l]!=';'){
            if(buf[r]=='\n'){
                    comd=new COMD(buf,l,r);
                    
                    if(first!=nullptr){
                        if(comd->head==com_commit){
                            if(first!=nullptr){
                                work(first->next,*clientst);
                                delete first;
                                first=nullptr;
                                delete comd;
                            }
                        }
                        else{
                            next->next=comd;
                            next=comd;
                            char buf[BUFSIZ]="ADDED";
                            write(*clientst,buf,BUFSIZ);
                            num++;
                        }
                    }
                    else{
                        if(comd->head==com_beginn){
                            if(first!=nullptr){
                                char buf[BUFSIZ]="Do not begin twice";
                                write(*clientst,buf,BUFSIZ);
                            }
                            else{
                                first=comd;
                                next=first;
                            }
                        }
                        else{
                             char buf[BUFSIZ]="Illegal Input";
                             write(*clientst,buf,BUFSIZ);
                        }
                        
                    }
                    l=r+1;
                }
            if(buf[r]==';'){
                    comd=new COMD(buf,l,r);
                    if(first!=nullptr){
                        if(comd->head==com_commit){
                            if(first!=nullptr){
                                work(first->next,*clientst);
                                delete first;
                                first=nullptr;
                                delete comd;
                            }
                        }
                        else{
                            next->next=comd;
                            next=comd;
                            char buf[BUFSIZ]="ADDED";
                            write(*clientst,buf,BUFSIZ);
                            num++;
                        }
                    }
                    else{
                        if(comd->head==com_beginn){
                            if(first!=nullptr){
                                char buf[BUFSIZ]="Do not begin twice";
                                write(*clientst,buf,BUFSIZ);
                            }
                            else{
                                first=comd;
                                next=first;
                            }
                        }
                        else{
                             char buf[BUFSIZ]="Illegal Input";
                             write(*clientst,buf,BUFSIZ);
                        }
                        
                    }
                    l=r;
                
                }
            r++;
        }
        buf[0]='!';
        buf[1]='\0';
        write(*clientst,buf,BUFSIZ); 
        for(int i=0;i<ret;i++)
            buf[i]='\0';
    }
       
    COMD Comd;
    Comd.resave();
    close(*clientst);
    delete clientst;
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
        printf("Accpet\n");
        std::thread t(clientwork,client);
        t.detach();
    }
    
    close(server);


    

}
