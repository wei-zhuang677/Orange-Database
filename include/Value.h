#ifndef VALUE_H
#define VALUE_H
#include "SDS.h"
class Value {
public:
    Value();
    Value(const Value& value);
    Value(SDS& sds);
    Value& operator=(const Value& sds);
    SDS sds;
    int tpye=0;
};

#endif