#ifndef LIST_H
#define LIST_H
#include"SDS.h"
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
    SDS* lindex(int index);
    int len;
};

#endif