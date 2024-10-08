#include "Readview.h"

Readview::Readview(){
    head=nullptr;
    up_limit_id=100000001;
    low_limit_id=-1;
}

Readview::Readview(Readview& readview){
    up_limit_id=readview.up_limit_id;
    low_limit_id=readview.low_limit_id;
    Node* current=readview.head;
    while(current!=nullptr){
        Node* node=new Node;
        node->at_id=current->at_id;
        node->next=current->next;
        current=current->next;
    }
}

Readview::~Readview(){
    while(head!=nullptr){
        Node* node=head->next;
        delete head;
        head=node;
    }
}

void Readview::add(int id){
    if(id+1>low_limit_id)
        low_limit_id=id+1;
    if(id<up_limit_id)
        up_limit_id=id;
    Node* node=new Node;
    node->at_id=id;
    node->next=head;
    head=node;
}

void Readview::pop(int id){
    if(head->at_id==id){
        Node* next=head->next;
        delete head;
        head=next;
        return;
    }
    Node* current=head;
    Node* next=current->next;
    while(next!=nullptr){
        if(next->at_id==id){
            next=next->next;
            delete current->next;
            current->next=next;
            break;
        }
        current=current->next;
        next=next->next;
    }
    if(id==up_limit_id){
        current=head;
        up_limit_id=100000001;
        while(current!=nullptr){
            if(up_limit_id>current->at_id){
                up_limit_id=current->at_id;
            }
            current=current->next;
        }
    }
}

bool Readview::check(int id){
    if(id<up_limit_id)
        return 1;
    if(id>=low_limit_id)
        return 0;
    Node* current=head;
    while(current!=nullptr){
        if(current->at_id==id)
            return 0;
        current=current->next;
    }
    return 1;
}