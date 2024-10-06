#ifndef LIST_H
#define LIST_H
#include"SDS.h"
#include<iostream>
class List {
public:
    List();
    ~List();
    List(List& list);
    struct Node
    {
        Node* prev=nullptr;
        Node* next=nullptr;
        SDS *value;
    };
    Node* head;
    Node* tail;
    void addl(SDS& value);
    void addr(SDS& value);
    void popr();
    void popl();
    Node* lindex(int index);
    void print();
    int len;
};

#endif