#include "SDS.h"

SDS::SDS(){
    len=0;
    free=0;
    buf=new char[0];
}
SDS::SDS(const char* c,int l,int r){
    len=0;
    free=0;
    if(r==-1){ 
    while(c[len]!='\0')
        len++;
        r=len;
    }
    else
        len=r-l;
    buf=new char[len];
    for(int i=l;i<r;i++)
        buf[i-l]=c[i];
}

void SDS::refresh(char *c,int l,int r){
    len=0;
    free=0;
    if(r==-1){ 
    while(c[len]!='\0')
        len++;
        r=len;
    }
    else
        len=r-l;
    delete[] buf;
    buf=new char[len];
    for(int i=l;i<r;i++)
        buf[i-l]=c[i];
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


bool SDS::operator==(const SDS& other){
    if(len!=other.len)
        return 0;
    for(int i=0;i<len;i++){
        if(buf[i]!=other.buf[i])
            return 0;
    }
    return 1;
}
SDS& SDS:: operator=(const SDS& sds){
    if(this != &sds){
        len=sds.len;
        free=sds.free;
        delete[] buf;
        buf=new char[len+free];
        for(int i=0;i<len;i++)
            buf[i]=sds.buf[i];
    }
    return *this;
}

void SDS::refresh(SDS& sds,int l,int r){
    len=r-l;
    free=0;
    delete[] buf;
    buf=new char[len];
    for(int i=l;i<r;i++)
        buf[i-l]=sds.buf[i];
}

void SDS::print(){
    for(int i=0;i<len;i++)
        printf("%c",buf[i]);
    printf("\n");
}