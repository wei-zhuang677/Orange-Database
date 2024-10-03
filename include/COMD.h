#ifndef COMD_H
#define COMD_H
#include "SDS.h"
#include"HashTable.h"
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
};

#endif