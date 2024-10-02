#include "SDS.h"
SDS::SDS(char *c,int l=0,int r=-1){
    len=0;
    free=0;
    while(c[len]!='\0')
        len++;
    if(r==-1)
        r=len;
    buf=new char[len];
    for(int i=l;i<r;i++)
        buf[i]=c[i];
}

SDS::SDS(const SDS& sds){
    len=sds.len;
    free=sds.free;
    buf=new char[len];
    for(int i=0;i<len;i++)
        buf[i]=sds.buf[i];
}
SDS::~SDS(){
    delete[] buf;
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

void SDS::copysds(const SDS& sds){
    len=sds.len;
    free=sds.free;
    delete[] buf;
    buf=new char[len];
    for(int i=0;i<len;i++)
        buf[i]=sds.buf[i];
}

bool SDS::operator==(const SDS& other){
    if(len!=other.len)
        return 0;
    for(int i=0;i<len;i++){
        if(buf[i]!=other.buf[i])
            return 0;
    }
    return 1;
}