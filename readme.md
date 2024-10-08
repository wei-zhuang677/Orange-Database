# Orange数据库

## 1.简介
这是一个由c++编写的键值对数据库,支持字符串、列表、哈希表、集合四种数据结构,用MVCC支持并行


## 2.设计框架
服务端与客户端通过socket通信（详见Orange通讯协议），服务端**server.cpp**负责传入数据的命令提取与事务的封装，  
**COND.cpp**负责命令的解析与运行，**DateTable.cpp**是数据库的主体，数据库中所有的键为**SDS.cpp**实现的SDS类，  
值均为**Value.cpp**中实现的Value类，由Value实例中不同的对象的指针实现不同数据结构的存储，**SDS.cpp**实现了字符串，  
**HashTable.cpp**实现了哈希表和集合，**List.cpp**实现了列表  

## 3.代码解释
 1. client.cpp
   ~~~c++
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
        }                                             //从上面到这里是一次输入
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

        }                                             //到这是接受完一次返回
        if(len==0)
            break;

    }
   ~~~

   2. server.cpp
 ~~~c++
        while(1){
        int client=accept(server,NULL,NULL);
        printf("Accpet\n");
        std::thread t(clientwork,client);
        t.detach();                 //这里用于循环接受请求并创建线程响应
    }
~~~

- clientwork函数用于提取命令并封装成事务
- work函数分配事务id,创建readview并执行事务

