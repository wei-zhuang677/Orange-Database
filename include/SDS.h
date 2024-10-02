#ifndef SDS_H
#define SDS_H

class SDS {
public:
    SDS(char *c);
    ~SDS();
    void addend(char *c);
    void expand(int l);
    int len,free;
    char *buf;
};

#endif