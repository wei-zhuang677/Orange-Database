#include "Value.h"

Value::Value(){
    tpye=0;
    sds=nullptr;
    list=nullptr;
//    key=nullptr;
}

Value::Value(SDS& sds){
    this->sds=new SDS(sds);
    list=nullptr;
    tpye=1;
}

Value::Value(List& list){
    this->list=new List(list);
    sds=nullptr;
    tpye=2;
}
Value::Value(const Value& value){
    tpye=value.tpye;
    
    switch (tpye)
    {
    case 1:
        this->sds=new SDS(*value.sds);
        list=nullptr;
        tpye=1;
        break;
    case 2:
        this->list=new List(*value.list);
        sds=nullptr;
        tpye=2;
 //   case 0:
       // sds= new SDS(*(value.sds));
    default:
        break;
    }
}

Value::~Value(){
    delete sds;
    delete list;
    //delete key;
}

Value& Value::operator=(const Value& value){
    if(this!=&value){
    delete sds;
    delete list;
    tpye=value.tpye;
    switch (tpye)
    {
    case 1:
        this->sds=new SDS(*value.sds);
        list=nullptr;
        tpye=1;
        break;
    case 2:
        this->list=new List(*value.list);
        sds=nullptr;
        tpye=2;
 //   case 0:
       // sds= new SDS(*(value.sds));
    default:
        break;
    }
    }
    return *this;
}