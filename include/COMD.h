#ifndef COMD_H
#define COMD_H
#include "SDS.h"
#include"DateTable.h"
#include"List.h"
#include"HashTable.h"
#include<thread>
class COMD;
class COMD {
public:
    SDS* comd;
    COMD* next;
    SDS head;
    COMD();
    COMD(char *buf,int l,int r);
    ~COMD();
    void run();
    void set();
    void get();
    void delet();
    void odbsave();
    void odbload();
    void resave();
    void save();
    void odbluach();
    void addr();
    void addl();
    void lindex();
    void lrange();
    void popr();
    void popl();
    void hset();
    void hget();
    void sadd();
    void smembers();
    void srem();
};

#endif