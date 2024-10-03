#include "COMD.h"
<<<<<<< HEAD
#include<iostream>
class COMD;
const char strget[10] = "get";
char strset[10] = "set";
const char strdelete[10] = "delete";
SDS com_get(strget);
SDS com_set(strset);
SDS com_delete(strdelete);
HashTable datetable;
COMD::COMD(){
    comd=nullptr;
    next=nullptr;
  
}
COMD::COMD(char *buf,int l,int r){
=======
#include "SDS.h"
#include<iostream>
COMD::COMD(){
    comd=nullptr;
    next=nullptr;
}
COMD::COMD(char *buf,int l,int r,COMD *nex){
    nex->next=this;
    nex=this;
>>>>>>> 6a8603b0b1dcdef78a1e91f345385e33a42bddcc
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
        get();
    } else if (head == com_set) {
        set();
    } else if (head == com_delete) {
        // code for delete
    } else {
        perror("Unknown command");
    }
}

void COMD::set(){
    int l=4,r=4;
    comd->print();
    while(comd->buf[r]!=','){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    SDS value;
    l=++r;
    while(comd->buf[r]==')'){
        r++;
    }
    value.refresh(*comd,l,r);
    key.print();
    datetable.insert(key,value);
}

void COMD::get(){
    int l=4,r=4;
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    SDS* value=static_cast<SDS*>(datetable.find(key));
    if(value!=nullptr){
        value->print();
    }
}

void COMD::delet(){
    int l=4,r=4;
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    datetable.delet(key);
}
