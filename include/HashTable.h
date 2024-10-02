#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Value.h"
#include "SDS.h"
class HashTable {
public:
    HashTable();
    ~HashTable();
private:
    struct Node{
        Value* value;
        Node* next;
        SDS key;
    };
    Node** buckets;
    int size;
    int count;
    unsigned int hashCode(SDS &key);
    void insert(SDS& key,const Value& value);
    void resize();
};

#endif