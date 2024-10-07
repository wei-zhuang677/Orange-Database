#include "Value.h"

Value::Value(){
    tpye=0;
    sds=nullptr;
    list=nullptr;
    hashtable=nullptr;
//    key=nullptr;
}

Value::Value(SDS& sds){
    this->sds=new SDS(sds);
    list=nullptr;
    hashtable=nullptr;
    tpye=1;
}

Value::Value(List& list){
    this->list=new List(list);
    sds=nullptr;
    hashtable=nullptr;
    tpye=2;
}

Value::Value(HashTable& hashtable){
    //rintf("djsjoi\n");
    this->hashtable=new HashTable(hashtable);
    sds=nullptr;
    list=nullptr;
    tpye=3;
}
Value::Value(const Value& value){
    tpye=value.tpye;
    
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

Value& Value::operator=(const Value& value){
    if(this!=&value){
    delete this->sds;
    this->sds=nullptr;
    delete this->list;
    this->list=nullptr;
    delete this->hashtable;
    this->hashtable=nullptr;
    tpye=value.tpye;
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
        break;
        }
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
    return *this;
}