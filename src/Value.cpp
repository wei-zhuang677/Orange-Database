#include "Value.h"

Value::Value(){
    tpye=0;
}

Value::Value(SDS& sds){
    this->sds=sds;
    tpye=1;
}

Value::Value(const Value& value){
    tpye=value.tpye;
    switch (tpye)
    {
    case 1:
        sds=value.sds;
        break;
    
    default:
        break;
    }
}

Value& Value::operator=(const Value& value){
    if(this!=&value){
    tpye=value.tpye;
    switch (tpye)
    {
    case 1:
        sds=value.sds;
        break;
    
    default:
        break;
    }
    }
    return *this;
}