#include "COMD.h"
#include<iostream>
class COMD;
const char strget[20] = "get";
const char strset[20] = "set";
const char strdelete[20] = "delete";
const char dateodbsave[20]="SAVE";
const char dateodbload[20]="odbload";
const char dateresave[20]="RESAVE";
const char save[20]="save";
const char addl[20]="addl";
const char addr[20]="addr";
const char lindex[20]="lindex";
SDS com_get(strget);
SDS com_set(strset);
SDS com_delete(strdelete);
SDS com_odbsave(dateodbsave);
SDS com_odbload(dateodbload);
SDS com_resave(dateresave);
SDS com_save(save);
SDS com_addr(addr);
SDS com_addl(addl);
SDS com_lindex(lindex);
HashTable datetable;
COMD::COMD(){
    comd=nullptr;
    next=nullptr;
  
}
COMD::COMD(char *buf,int l,int r){
    comd=new SDS(buf,l,r);
    next=nullptr;
    int t=l;
    while(buf[t]!='('&&buf[t]!=';')
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
    }else if(head == com_odbsave){
        odbsave();
    } else if(head == com_odbload){
        odbload();
    }else if(head ==com_resave){
        resave();
    }else if(head ==com_save){
        save();
    }else if(head == com_addl){
        addl();
    }else if(head ==com_addr){
        addr();
    }else if(head ==com_lindex){
        lindex();
    }else{
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
    Value* vvalue=datetable.find(key);
    if(vvalue==nullptr||vvalue->tpye==1){
          datetable.insert(key,value);
    }
    else{
         perror("That is not a list.Pleaes delete frist");
    }
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
    if (value != nullptr&&value->tpye==1){
        printf("GET:");
        value->sds->print();
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
    if(value!=nullptr){
        value= new Value();
        datetable.insert(key,*(value));
    }
    else{
        perror("Value not exist");
    }
}

void COMD::odbsave(){
    printf("saving\n");
    std::thread t(&HashTable::odbsave,&datetable);
    t.join();
}

void COMD::odbload(){
    printf("loading\n");
    datetable.odbload();
}

void COMD::resave(){
    printf("saving\n");
    std::thread t(&HashTable::odbsave,&datetable);
    t.detach();
}
void COMD::odbluach(){
    std::thread t(&HashTable::checkAndSave,&datetable);
    t.detach();
}

void COMD::save(){
    int l=5,r=5,a=0,b=0;
    while(comd->buf[r]!=','){
        a=a*10+(int)comd->buf[r]-48;
        r++;
    }
    l=++r;
    while(comd->buf[r]!=')'){
        b=b*10+(int)comd->buf[r]-48;
        r++;
    }
    datetable.updateConfig(a,b);
}

void COMD::addl(){
    int l=5,r=5;
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
    sds.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr){
        List list;
        list.addl(sds);
        value=new Value(list);
        datetable.insert(key,*value);
    }
    else
    {
        if(value->tpye==2){
            value->list->addl(sds);
        }
        else{
            perror("That is not a list.Pleaes delete frist");
        }
    }
    
    
}

void COMD::addr(){
    int l=5,r=5;
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
    sds.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr){
        List list;
        list.addr(sds);
        value=new Value(list);
        datetable.insert(key,*value);
    }
    else
    {
        if(value->tpye==2){
            value->list->addr(sds);
        }
        else{
            perror("That is not a list.Pleaes delete frist");
        }
    }
    
    
}

void COMD::lindex(){
    int l=7,r=7;
    int index=0;
    while(comd->buf[r]!=','){
      
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r); 
    l=++r;
    while(comd->buf[r]!=')'){
      index=index*10+(int)comd->buf[r]-48;
        r++;
    }
    
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0)
    {
        perror("Value not exist");
        return;
    }
    if(value->tpye==2){
        value->list->lindex(index)->print();
    }
    else{
        perror("That is not a list");
    }
}