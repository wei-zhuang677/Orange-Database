#ifndef SDS_H
#define SDS_H

class SDS {
public:
    SDS(char *c);
    ~SDS();
    int length();
    char get(int n);
    int set(int n,char c);
    void addend(char *c);
    void expand(int l);
private:
    int len,free;
    char *buf;
};

#endif