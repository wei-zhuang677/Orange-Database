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
const char lrange[20]="lrange";
const char popr[20]="popr";
const char popl[20]="popl";
const char hget[20]="hget";
const char hset[20]="hset";
const char sadd[20]="sadd";
const char smembers[20]="smembers";
const char srem[20]="srem";
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
SDS com_lrange(lrange);
SDS com_popr(popr);
SDS com_popl(popl);
SDS com_hset(hset);
SDS com_hget(hget);
SDS com_sadd(sadd);
SDS com_smembers(smembers);
SDS com_srem(srem);
DateTable datetable;
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
    }else if(head==com_lrange){
        lrange();
    }else if(head==com_popr){
        popr();
    }
    else if(head==com_popl){
        popl();
    }else if(head==com_hset){
        hset(); 
    }else if(head==com_hget){
        hget();
    }else if(head==com_sadd){
        sadd();
    }else if(head==com_smembers){
        smembers();
    }else {
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
    if(vvalue==nullptr||vvalue->tpye==1||value.tpye==0){
          datetable.insert(key,value);
    }
    else{
         perror("That is not a SDS.Pleaes delete frist");
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
        Value value;
        datetable.insert(key,value);
    }
    else{
        perror("Value not exist");
    }
}

void COMD::odbsave(){
    printf("saving\n");
    std::thread t(&DateTable::odbsave,&datetable);
    t.join();
}

void COMD::odbload(){
    printf("loading\n");
    datetable.odbload();
}

void COMD::resave(){
    printf("saving\n");
    std::thread t(&DateTable::odbsave,&datetable);
    t.detach();
}
void COMD::odbluach(){
    std::thread t(&DateTable::checkAndSave,&datetable);
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
    if(value==nullptr||value->tpye==0){
        List list;
        list.addl(sds);
        Value nvalue(list);
        datetable.insert(key,*value);
    }
    else
    {
        if(value->tpye==2){
            List list(*value->list);
            list.addl(sds);
            Value nvalue(list);
            datetable.insert(key,*value);
        }
        else{
            perror("That is not a list.Pleaes delete frist");
        }
    }
    
    
}

void COMD::addr(){
    int l=5,r=5;
    
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
    if(value==nullptr||value->tpye==0){
        List list;
        list.addr(sds);
        Value nvalue(list);
        datetable.insert(key,*value);
    }
    else
    {
        if(value->tpye==2){
            List list(*value->list);
            list.addr(sds);
            Value nvalue(list);
            datetable.insert(key,*value);
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
        List::Node* node=value->list->lindex(index);
        if(node!=nullptr){
            node->value->print();
        }
        else{
            perror("Value not exist");
        }
    }
    else{
        perror("That is not a list");
    }
}

void COMD::lrange(){
    int l=7,r=7;
    int start=0,stop=0;
    while(comd->buf[r]!=','){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r); 
    l=++r;
    while(comd->buf[r]!=','){
      start=start*10+(int)comd->buf[r]-48;
        r++;
    }
    l=++r;
    while(comd->buf[r]!=')'){
      stop=stop*10+(int)comd->buf[r]-48;
        r++;
    }
    if(start>stop){
        perror("Start is bigger than stop");
        return;
    }
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0)
    {
        perror("Value not exist");
        return;
    }
    if(value->tpye==2){
        if(stop>=value->list->len){
            perror("List is not enough");
            return;
        }
        List::Node* node=value->list->lindex(start);
        if(node==nullptr){
            perror("Wrong start");
            return;
        }
        printf("List:\n");
        for(int i=start;i<=stop;i++){
            node->value->print();
            node=node->next;
        }
    }
    else{
        perror("That is not a list");
    }
}

void COMD::popr(){
    int l=5,r=5; 
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        perror("Value not exist");
        return;
    }
    else{
        if(value->tpye==2){
            if(value->list->len>0){
                Value nvalue(*value->list);
             //   nvalue.list->print();
                nvalue.list->popr();
                datetable.insert(key,nvalue);
            }
            else{
                perror("This list is empty");
            }
        }
        else{
            perror("That is not a list");
        }
    }
    
    
}

void COMD::popl(){
    int l=5,r=5; 
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        perror("Value not exist");
        return;
    }
    else{
        if(value->tpye==2){
            if(value->list->len>0){
                Value nvalue(*value->list);
//                nvalue.list->print();
                nvalue.list->popl();
                datetable.insert(key,nvalue);
            }
            else{
                perror("This list is empty");
            }
        }
        else{
            perror("This is not a list");
        }
    }
    
}

void COMD::hset(){
    int l=5,r=5;
    while(comd->buf[r]!=','){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    l=++r;
    while(comd->buf[r]!=','){
        r++;
    }
    SDS field;
    field.refresh(*comd,l,r);
    l=++r;
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS sds;
    sds.refresh(*comd,l,r);
    if(value==nullptr||value->tpye==0){
        HashTable hashtable;
        hashtable.insert(field,sds);
        Value nvalue(hashtable);
        datetable.insert(key,nvalue);
    }
    else{
        if(value->tpye==3){
            HashTable hashtable(*value->hashtable);
            hashtable.insert(field,sds);
            Value nvalue(hashtable);
            datetable.insert(key,nvalue);
        }
        else{
            perror("This is not a HashTable");
        }
    }
    

}

void COMD::hget(){
    int l=5,r=5;
    
    while(comd->buf[r]!=','){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    l=++r;
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS field;
    field.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        perror("Value not exist");
    }
    else{
        if(value->tpye==3){
            printf("GETing\n");
            SDS* sds=value->hashtable->find(field);
            if(sds!=nullptr){
                sds->print();
            }
            else{
                perror("Value not exist");
            }
        }
    }
}

void COMD::sadd(){
    int l=5,r=5;
    while(comd->buf[r]!=','){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    l=++r;
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS field;
    field.refresh(*comd,l,r);
    if(value==nullptr||value->tpye==0){
        HashTable hashtable;
        hashtable.insert(field,field);
        Value nvalue(hashtable);
        nvalue.tpye=4;
        datetable.insert(key,nvalue);
    }
    else{
        if(value->tpye==4){
            if(value->hashtable->find(field)!=nullptr){
                perror("This value is already in the set");
                return;
            }
            HashTable hashtable(*value->hashtable);
            hashtable.insert(field,field);
            Value nvalue(hashtable);
            nvalue.tpye=4;
            datetable.insert(key,nvalue);
        }
        else{
            perror("This is not a SET");
        }
    }
}

void  COMD::smembers(){
    int l=9,r=9;
    printf("GETing\n");
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        perror("Value not exist");
    }
    else{
        if(value->tpye==4){
            printf("SET:\n");
            HashTable* hashtable=value->hashtable;
            for(int i=0;i<hashtable->size;i++){
                HashTable::Node* current=hashtable->buckets[i];
                while(current!=nullptr){
                    current->value->print();
                    current=current->next;
                }
            }
            }
            else{
                perror("This is not a set");
            }
        }
    
}