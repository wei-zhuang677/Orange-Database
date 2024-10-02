#ifndef SDS_H
#define SDS_H

class SDS {
public:
    SDS(char *c,int l=0,int r=-1);
    SDS(const SDS& sds);
    ~SDS();
    void addend(char *c);
    int expand(int l);
    void copysds(const SDS& sds);
    bool operator == (const SDS& other)
    int len,free;
    char *buf;
};

#endif