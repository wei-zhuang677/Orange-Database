#include "Value.h"

Value::Value(){
    tpye=0;
    sds=nullptr;
    list=nullptr;
    hashtable=nullptr;
    pre=nullptr;
//    key=nullptr;
}

Value::Value(SDS& sds){
    this->sds=new SDS(sds);
    list=nullptr;
    hashtable=nullptr;
    pre=nullptr;
    tpye=1;
}

Value::Value(List& list){
    this->list=new List(list);
    sds=nullptr;
    hashtable=nullptr;
    pre=nullptr;
    tpye=2;
}

Value::Value(HashTable& hashtable){
    //printf("djsjoi\n");
    this->hashtable=new HashTable(hashtable);
    sds=nullptr;
    list=nullptr;
    pre=nullptr;
    tpye=3;
}
Value::Value(const Value& value){
    tpye=value.tpye;
    work_id=value.work_id;
    switch (tpye)
    {
    case 1:{
        this->sds=new SDS(*value.sds);
        list=nullptr;
        hashtable=nullptr;
        break;}
    case 2:{
        this->list=new List(*value.list);
        sds=nullptr;
        hashtable=nullptr;
        break;}
    case 4:
    case 3:{
        this->hashtable=new HashTable(*value.hashtable);
        sds=nullptr;
        list=nullptr;
        break;
    }
    default:
        break;
    }
}

Value::~Value(){
    delete sds;
    delete list;
    delete hashtable;
    //delete key;
}

Value* Value::add( Value& value){
    Value* nvalue=new Value(value);
    value.pre=this;
    return nvalue;
}