#include "COMD.h"
#include "SDS.h"
#include<iostream>
COMD::COMD(){
    comd=nullptr;
    next=nullptr;
}
COMD::COMD(char *buf,int l,int r,COMD *nex){
    nex->next=this;
    nex=this;
    comd=new SDS(buf,l,r);
    int t=l;
    while(buf[t]!='(')
        t++;
    head.refresh(buf,l,t);
}

COMD::~COMD(){
    delete comd;
}

void COMD::run(){
    if (head == com_get) {
        // code for get
    } else if (head == com_set) {
        // code for set
    } else if (head == com_delete) {
        // code for delete
    } else {
        perror("Unknown command");
    }
}
const char strget[10] = "get";
const char strset[10] = "set";
const char strdelete[10] = "delete";
SDS com_get(strget),com_get(strset),com_delete(strdelete);