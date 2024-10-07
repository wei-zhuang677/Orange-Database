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

void COMD::run(int clientst){
   //head.print();
    if (head == com_get) {
        get(clientst);
    } else if (head == com_set) {
        set(clientst);
    } else if (head == com_delete) {
        delet(clientst);
    }else if(head == com_odbsave){
        odbsave(clientst);
    } else if(head == com_odbload){
        odbload();
    }else if(head ==com_resave){
        resave();
    }else if(head ==com_save){
        save(clientst);
    }else if(head == com_addl){
        addl(clientst);
    }else if(head ==com_addr){
        addr(clientst);
    }else if(head ==com_lindex){
        lindex(clientst);
    }else if(head==com_lrange){
        lrange(clientst);
    }else if(head==com_popr){
        popr(clientst);
    }
    else if(head==com_popl){
        popl(clientst);
    }else if(head==com_hset){
        hset(clientst); 
    }else if(head==com_hget){
        hget(clientst);
    }else if(head==com_sadd){
        sadd(clientst);
    }else if(head==com_smembers){
        smembers(clientst);
    }else if(head==com_srem){
        srem(clientst);
    }else{
        perror("Illegal Input");
    }
}

void COMD::set(int clientst){
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
        char buf[BUFSIZ]="Setting Success";
        write(clientst,buf,BUFSIZ);
    }
    else{
        datetable.insert(key,value);
        char buf[BUFSIZ]="That is not a SDS.Pleaes delete frist";
        write(clientst,buf,BUFSIZ);
    }
}

void COMD::get(int clientst){
    int l=4,r=4;
   // comd->print();
    while(comd->buf[r]!=')'){
        r++;
    }
    
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if (value != nullptr&&value->tpye==1){
        char buf[BUFSIZ]="GET:";
        write(clientst,buf,BUFSIZ);
        value->sds->print(clientst);
        value->sds->print();
    } else {
        char buf[BUFSIZ]="Value not found";
        write(clientst,buf,BUFSIZ);
    }
 
}

void COMD::delet(int clientst){
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
        char buf[BUFSIZ]="Delete success";
        write(clientst,buf,BUFSIZ);
    }
    else{
        char buf[BUFSIZ]="Value not exist";
        write(clientst,buf,BUFSIZ);
    }
}

void COMD::odbsave(int clientst){
    char buf[BUFSIZ]="Saving";
    write(clientst,buf,BUFSIZ);
    std::thread t(&DateTable::odbsave,&datetable);
    t.join();
    char buff[BUFSIZ]="Save finished";
    write(clientst,buff,BUFSIZ);
}

void COMD::odbload(){
    printf("loading\n");
    datetable.odbload();
  //  perror("njknn");
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

void COMD::save(int clientst){
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
    char buf[BUFSIZ]="Update finished";
    write(clientst,buf,BUFSIZ);
}

void COMD::addl(int clientst){
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
    sds.print();
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        List list;
        list.addl(sds);
        Value nvalue(list);
        datetable.insert(key,nvalue);
        char buf[BUFSIZ]="Setting Success";
        write(clientst,buf,BUFSIZ);
    }
    else
    {
        if(value->tpye==2){
            List list(*value->list);
            list.addl(sds);
            Value nvalue(list);
            datetable.insert(key,nvalue);
            char buf[BUFSIZ]="Setting Success";
            write(clientst,buf,BUFSIZ);
        }
        else{
            char buf[BUFSIZ]="That is not a list.Pleaes delete frist";
            write(clientst,buf,BUFSIZ);
        }
    }
    
    
}

void COMD::addr(int clientst){
    int l=5,r=5;
    comd->print();
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
        datetable.insert(key,nvalue);
        char buf[BUFSIZ]="Setting Success";
        write(clientst,buf,BUFSIZ);
    }
    else
    {
        if(value->tpye==2){
            List list(*value->list);
            list.addr(sds);
            Value nvalue(list);
            datetable.insert(key,nvalue);
            char buf[BUFSIZ]="Setting Success";
            write(clientst,buf,BUFSIZ);
        }
        else{
            char buf[BUFSIZ]="That is not a list.Pleaes delete frist";
            write(clientst,buf,BUFSIZ);
        }
    }
    
    
}

void COMD::lindex(int clientst){
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
        char buf[BUFSIZ]="List not exist";
        write(clientst,buf,BUFSIZ);
        return;
    }
    if(value->tpye==2){
        List::Node* node=value->list->lindex(index);
        if(node!=nullptr){
            node->value->print(clientst);
        }
        else{
            char buf[BUFSIZ]="Value not exist";
            write(clientst,buf,BUFSIZ);
        }
    }
    else{
        char buf[BUFSIZ]="That is not a list";
        write(clientst,buf,BUFSIZ);
    }
}

void COMD::lrange(int clientst){
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
        char buf[BUFSIZ]="Start is bigger than stop";
        write(clientst,buf,BUFSIZ);
        return;
    }
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0)
    {   
        char buf[BUFSIZ]="List not exist";
        write(clientst,buf,BUFSIZ);
        return;
    }
    if(value->tpye==2){
        if(stop>=value->list->len){
            char buf[BUFSIZ]="List is not enough long";
            write(clientst,buf,BUFSIZ);
            return;
        }
        List::Node* node=value->list->lindex(start);
        if(node==nullptr){
            char buf[BUFSIZ]="Wrong start";
            write(clientst,buf,BUFSIZ);
            return;
        }
        char buf[BUFSIZ]="List:";
        write(clientst,buf,BUFSIZ);
        for(int i=start;i<=stop;i++){
            node->value->print(clientst);
            node=node->next;
        }
    }
    else{
        char buf[BUFSIZ]="That is not a list";
        write(clientst,buf,BUFSIZ);
    }
}

void COMD::popr(int clientst){
    int l=5,r=5; 
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        char buf[BUFSIZ]="List not exist";
        write(clientst,buf,BUFSIZ);
        return;
    }
    else{
        if(value->tpye==2){
            if(value->list->len>0){
                Value nvalue(*value->list);
             //   nvalue.list->print();
                nvalue.list->popr();
                datetable.insert(key,nvalue);
                char buf[BUFSIZ]="Delete finished";
                write(clientst,buf,BUFSIZ);
            }
            else{
                char buf[BUFSIZ]="This list is empty";
                write(clientst,buf,BUFSIZ);
                perror("This list is empty");
            }
        }
        else{
            char buf[BUFSIZ]="TThat is not a list";
            write(clientst,buf,BUFSIZ);
            perror("That is not a list");
        }
    }
    
    
}

void COMD::popl(int clientst){
    int l=5,r=5; 
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        char buf[BUFSIZ]="List not exist";
        write(clientst,buf,BUFSIZ);
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
                char buf[BUFSIZ]="Delete finished";
                write(clientst,buf,BUFSIZ);
            }
            else{
                char buf[BUFSIZ]="This list is empty";
                write(clientst,buf,BUFSIZ);
                perror("This list is empty");
            }
        }
        else{
            char buf[BUFSIZ]="This is not a list";
            write(clientst,buf,BUFSIZ);
            perror("This is not a list");
        }
    }
    
}

void COMD::hset(int clientst){
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
        char buf[BUFSIZ]="Setting success";
        write(clientst,buf,BUFSIZ);
    }
    else{
        if(value->tpye==3){
            HashTable hashtable(*value->hashtable);
            hashtable.insert(field,sds);
            Value nvalue(hashtable);
            datetable.insert(key,nvalue);
            char buf[BUFSIZ]="Setting success";
            write(clientst,buf,BUFSIZ);
        }
        else{
            char buf[BUFSIZ]="This is not a HashTable";
            write(clientst,buf,BUFSIZ);
            perror("This is not a HashTable");
        }
    }
    

}

void COMD::hget(int clientst){
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
        char buf[BUFSIZ]="Value not exist";
        write(clientst,buf,BUFSIZ);
        perror("Value not exist");
    }
    else{
        if(value->tpye==3){
           /* char buf[BUFSIZ]="HashTable:";
            write(clientst,buf,BUFSIZ);
            printf("HashTable:\n");*/
            SDS* sds=value->hashtable->find(field);
            if(sds!=nullptr){
                sds->print(clientst);
            }
            else{
                char buf[BUFSIZ]="This is not in HashTable";
                write(clientst,buf,BUFSIZ);
                perror("This is not in HashTable");
            }
        }
    }
}

void COMD::sadd(int clientst){
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
        char buf[BUFSIZ]="Setting success";
        write(clientst,buf,BUFSIZ);
        
    }
    else{
        if(value->tpye==4){
            if(value->hashtable->find(field)!=nullptr){
                char buf[BUFSIZ]="This value is already in the set";
                write(clientst,buf,BUFSIZ);
                perror("This value is already in the set");
                return;
            }
            HashTable hashtable(*value->hashtable);
            hashtable.insert(field,field);
            Value nvalue(hashtable);
            nvalue.tpye=4;
            datetable.insert(key,nvalue);
            char buf[BUFSIZ]="Setting success";
            write(clientst,buf,BUFSIZ);
        }
        else{
            char buf[BUFSIZ]="This is not a SET";
            write(clientst,buf,BUFSIZ);
            perror("This is not a SET");
        }
    }
}

void  COMD::smembers(int clientst){
    int l=9,r=9;
    while(comd->buf[r]!=')'){
        r++;
    }
    SDS key;
    key.refresh(*comd,l,r);
    Value* value=datetable.find(key);
    if(value==nullptr||value->tpye==0){
        char buf[BUFSIZ]="Value not exist";
        write(clientst,buf,BUFSIZ);
        perror("Value not exist");
    }
    else{
        if(value->tpye==4){
            char buf[BUFSIZ]="Set:";
            write(clientst,buf,BUFSIZ);
            printf("SET:\n");
            HashTable* hashtable=value->hashtable;
            for(int i=0;i<hashtable->size;i++){
                HashTable::Node* current=hashtable->buckets[i];
                while(current!=nullptr){
                    current->value->print(clientst);
                    current=current->next;
                }
            }
            }
            else{
                char buf[BUFSIZ]="This is not a SET";
                write(clientst,buf,BUFSIZ);
                perror("This is not a set");
            }
        }
}

void COMD::srem(int clientst){
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
        char buf[BUFSIZ]="Value not exist";
        write(clientst,buf,BUFSIZ);
        perror("Value not exist");
    }
    else{
        if(value->tpye==4){
            if(value->hashtable->find(field)==nullptr){
                char buf[BUFSIZ]="This value is not in the set";
                write(clientst,buf,BUFSIZ);
                perror("This value is not in the set");
                return;
            }
            HashTable hashtable(*value->hashtable);
            hashtable.delet(field);
            Value nvalue(hashtable);
            nvalue.tpye=4;
            datetable.insert(key,nvalue);
            char buf[BUFSIZ]="Delete finished";
            write(clientst,buf,BUFSIZ);
        }
        else{
            char buf[BUFSIZ]="This is not a SET";
            write(clientst,buf,BUFSIZ);
            perror("This is not a SET");
        }
    }
}