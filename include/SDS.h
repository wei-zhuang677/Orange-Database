#ifndef SDS_H
#define SDS_H
#include "Value.h"
class SDS : public Value{
public:
    SDS();
    SDS(char *c,int l=0,int r=-1);
    SDS(const SDS& sds);
    ~SDS();
    void addend(char *c);
    int expand(int l);
    bool operator == (const SDS& other);
    void refresh(char *c,int l=0,int r=-1);
    void refresh(SDS& sds,int l=0,int r=-1);
    int len,free;
    SDS& operator=(const SDS& other);
    char *buf;
};

#endif