#ifndef VALUE_H
#define VALUE_H
#include"HashTable.h"
#include "SDS.h"
#include"List.h"
class Value {
public:
    Value();
    Value(const Value& value);
    Value(List& list);
    Value(SDS& sds);
    Value(HashTable& hashtable);
    ~Value();
    Value& operator=(const Value& sds);
    SDS* sds;
    List* list;
    HashTable* hashtable;
   // SDS* key;
    int tpye=0;
};

#endif