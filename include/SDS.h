#ifndef SDS_H
#define SDS_H

class SDS {
public:
    SDS(char *c);
    SDS(const SDS& sds);
    ~SDS();
    void addend(char *c);
    int expand(int l);
    void copysds(const SDS& sds);
    int len,free;
    char *buf;
};

#endif