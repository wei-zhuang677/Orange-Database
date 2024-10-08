#ifndef COMD_H
#define COMD_H
#include "SDS.h"
#include"DateTable.h"
#include"List.h"
#include"HashTable.h"
#include"Readview.h"
#include<thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<mutex>
class COMD;
class COMD {
public:
    SDS* comd;
    COMD* next;
    SDS head;
    COMD();
    COMD(char *buf,int l,int r);
    ~COMD();
    void run(int clientst,int id,Readview* readview);
    void set(int clientst,int id,Readview* readview);
    void get(int clientst,int id,Readview* readview);
    void delet(int clientst,int id,Readview* readview);
    void odbsave(int clientst,int id);
    void odbload();
    void resave();
    void save(int clientst,int id);
    void odbluach();
    void addr(int clientst,int id,Readview* readview);
    void addl(int clientst,int id,Readview* readview);
    void lindex(int clientst,int id,Readview* readview);
    void lrange(int clientst,int id,Readview* readview);
    void popr(int clientst,int id,Readview* readview);
    void popl(int clientst,int id,Readview* readview);
    void hset(int clientst,int id,Readview* readview);
    void hget(int clientst,int id,Readview* readview);
    void sadd(int clientst,int id,Readview* readview);
    void smembers(int clientst,int id,Readview* readview);
    void srem(int clientst,int id,Readview* readview);
};

#endif