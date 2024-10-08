#ifndef VALUE_H
#define VALUE_H
#include"HashTable.h"
#include "SDS.h"
#include"List.h"
class Value;
class Value {
public:
    Value();
    Value(const Value& value);
    Value(List& list);
    Value(SDS& sds);
    Value(HashTable& hashtable);
    ~Value();
    Value* add( Value& value);
    SDS* sds;
    List* list;
    HashTable* hashtable;
    int work_id=0;
    Value* pre;
   // SDS* key;
    int tpye=0;
};

#endif