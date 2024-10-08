#ifndef DEADVIEW_H
#define DEADVIEW_H
#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <mutex>

class Readview
{
public:
    struct Node
    {
        Node* next=nullptr;
        int at_id=0;
    };
    Node* head;
    int up_limit_id;
    int low_limit_id;
    Readview();
    Readview(Readview& readview);
    ~Readview();
    void add(int id);
    void pop(int id);
    bool check(int id);
    std::mutex id_mtx;
};

#endif