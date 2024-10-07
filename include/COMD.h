#ifndef COMD_H
#define COMD_H
#include "SDS.h"
#include"DateTable.h"
#include"List.h"
#include"HashTable.h"
#include<thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
class COMD;
class COMD {
public:
    SDS* comd;
    COMD* next;
    SDS head;
    COMD();
    COMD(char *buf,int l,int r);
    ~COMD();
    void run(int clientst);
    void set(int clientst);
    void get(int clientst);
    void delet(int clientst);
    void odbsave(int clientst);
    void odbload();
    void resave();
    void save(int clientst);
    void odbluach();
    void addr(int clientst);
    void addl(int clientst);
    void lindex(int clientst);
    void lrange(int clientst);
    void popr(int clientst);
    void popl(int clientst);
    void hset(int clientst);
    void hget(int clientst);
    void sadd(int clientst);
    void smembers(int clientst);
    void srem(int clientst);
};

#endif