#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "SDS.h"
#include<iostream>
class HashTable {
public:
    HashTable();
    ~HashTable();
    HashTable(HashTable& hashtable);
    void insert(SDS& key, SDS& value);
    SDS* find(SDS& key);
    int size;
   
    struct Node{
        SDS* value=nullptr;
        Node* next=nullptr;
        SDS key;
    };
      Node** buckets;
private:
    
    
    
    int count;
    unsigned int hashCode(SDS& key);
    void resize();
};

#endif