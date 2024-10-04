#include "COMD.h"
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
    //head.print();
    if (head == com_get) {
        get();
    } else if (head == com_set) {
        set();
    } else if (head == com_delete) {
        delet();
    } else {
        perror("Illegal Input");
    }
}

void COMD::set(){
    int l=4,r=4;
   // comd->print();
    while(comd->buf[r]!=','){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    SDS sds;
    l=++r;
    while(comd->buf[r]!=')'){
        r++;
    }
   // printf("L:%d R:%d ",l,r);
    sds.refresh(*comd,l,r);
    Value value(sds);
  //  printf("value:");
 //   sds.print();
  //  printf("SET:");
  //  value.sds.print();
    
    datetable.insert(key,value);
}

void COMD::get(){
    int l=4,r=4;
   // comd->print();
    while(comd->buf[r]!=')'){
        r++;
    }
    
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if (value != nullptr&&value->tpye==1&&!(value->sds.ld)) {
        printf("GET:");
        value->sds.print();
    } else {
        perror("Value not found");
    }
 
}

void COMD::delet(){
    int l=7,r=7;
    //printf("Deleting\n");
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    //key.print();
    Value* value=datetable.find(key);
    if(value!=nullptr&&!(value->sds.ld)&&value->tpye==1){
        value->sds.ld=1;
    }
}
