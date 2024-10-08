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
    void insert(SDS& key, Value& value);
    Value* find(SDS& key);
    void odbsave();
    void odbload();
    void checkAndSave();
    void updateConfig(int a,int b);
private:
    struct Node{
        Value* value=nullptr;
        Node* next=nullptr;
        SDS key;
        std::mutex write_mtx;
    }; 
    int savedKeysCount;
    Node** buckets;
    int size;
    int count;
    unsigned int hashCode(SDS& key);
    void resize();
    int interval,threshold;
    std::mutex mtx;
    std::mutex* bk_mtx;
};

#endif