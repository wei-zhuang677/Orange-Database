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
    int  client=socket(AF_INET,SOCK_STREAM,0);
    if(client<0){
        perror("Error creating socket\n");
    }

    struct sockaddr_in client_address;
    client_address.sin_family=AF_INET;
    client_address.sin_port=htons(8888);
    client_address.sin_addr.s_addr=INADDR_ANY;

    if(connect(client,(struct sockaddr*)&client_address,sizeof(client_address))<0){
        perror("Error connecting to server\n");
        close(client);
        return -1;
    }

    while(1)
      {
         printf("输入：");
         int l=0;
         while(1)
         {
            char c=getchar();
            if(c=='!')
              break;
            buf[l]=c;
            if(c==';'){
                buf[++l]='\0';
                printf("%d\n",l);
                write(client,buf,l+1);
                int ret=read(client,buf,BUFSIZ);
             //   printf("返回：%s    %d\n",buf,ret);
                l=-1;
                getchar();

            }
            l++;
            
         }
         
        break;
        
      }
      close(client);

}