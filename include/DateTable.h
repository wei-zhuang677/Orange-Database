#ifndef DATETABLE_H
#define DATETABLE_H
#include "Value.h"
#include"List.h"
#include "SDS.h"
#include"HashTable.h"
#include<fstream>
#include<iostream>
#include<thread>
#include <mutex>
class DateTable {
public:
    DateTable();
    ~DateTable();
    void insert(SDS& key,const Value& value);
    Value* find(SDS& key);
    void odbsave();
    void odbload();
    void checkAndSave();
    void updateConfig(int a,int b);
private:
    struct Node{
        Value* value;
        Node* next;
        SDS key;
    }; 
    int savedKeysCount;
    Node** buckets;
    int size;
    int count;
    unsigned int hashCode(SDS& key);
    void resize();
    int interval,threshold;
    std::mutex mtx;
};

#endif