#ifndef SDS_H
#define SDS_H
#include<iostream>
class SDS {
public:
    void print();
     char *buf;
    SDS();
    SDS(const char *c,int l=0,int r=-1);
    SDS(const SDS& sds);
    ~SDS();
    void addend(char *c);
    int expand(int l);
    bool operator == (const SDS& other);
    void refresh(char *c,int l=0,int r=-1);
    void refresh(SDS& sds,int l=0,int r=0);
    int len,free;
    SDS& operator=(const SDS& other);
    
};

#endif