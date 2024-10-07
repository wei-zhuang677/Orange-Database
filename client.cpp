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
    client_address.sin_port=htons(8880);
    client_address.sin_addr.s_addr=INADDR_ANY;

    if(connect(client,(struct sockaddr*)&client_address,sizeof(client_address))<0){
        perror("Error connecting to server\n");
        close(client);
        return -1;
    }

    while(1)
    {
        printf("输入：\n");
        int l=0;
        char c=getchar();
        while(1)
        {
            if(c=='!')
            {
                write(client,buf,0);
                break;
            }
            buf[l]=c;
            if(c==';'){
                buf[++l]='\0';
              //  printf("%d\n",l);
                write(client,buf,l+1);
                //   printf("返回：%s    %d\n",buf,ret);
                getchar();
                break;
            }
            c=getchar();
            l++;
        }
        if(c=='!'){
            printf("连接断开\n");
            break;
        }
        printf("返回：\n");
        int len;
        while(1){
            len=read(client,buf,BUFSIZ);
          //  printf("len:%d \n",len);
            if(buf[0]!='!'&&len!=0){
                for(int i=0;i<BUFSIZ;i++)
                    printf("%c",buf[i]);
                printf("\n");
            /*   for(int i=0;i<len;i++)
                    buf[i]='\0';*/ 
                
            }
            else{
                printf("返回完毕\n");
                break;
            }

        }
        if(len==0)
            break;

    }
      close(client);

}