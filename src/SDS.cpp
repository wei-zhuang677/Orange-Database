#include "SDS.h"
SDS::SDS(char *c){
    len=0;
    free=0;
    while(c[len]!='\0')
        len++;
    buf=new char[len];
    for(int i=0;i<len;i++)
        buf[i]=c[i];
}
SDS::~SDS(){
    delete[] buf;
}
int SDS:: getlen(){
    return len;
}

char SDS:: get(int n){
    if(n>=len){
        return '\0';
    }
    return buf[n];
}

int SDS::set(int n,char c){
    if(n>=len){
        return -1;
    }
    buf[n]=c;
    return 0;
}

void SDS::addend(char *c){
        int clen=0;
        while(c[clen]!='\0')
            clen++;
        if(clen>free){
            expand((clen+len)*2);
        }
         for(int i=0;i<clen;i++)
                buf[len+i]=c[i];
        len+=clen;
        free-=clen;
}

int SDS::expand(int l){
    if(l<len)
        return -1; 
    char *bbuf=new char[l];
    for(int i=0;i<len;i++)
        bbuf[i]=buf[i];
    delete[] buf;
    buf=bbuf;
    free=l-len;
    return l;
}