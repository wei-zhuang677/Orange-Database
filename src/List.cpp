#include"List.h"

List::List(){
    head=nullptr;
    tail=nullptr;
    len=0;
}

List::~List(){
    Node* current=head;
    while(current!=nullptr){
        Node* next=current->next;
        delete current->value;
        delete current->next;
        current =next;
        
    }
}

List::List(List& list){
    head=nullptr;
    tail=nullptr;
    len=0;
    Node* onode=list.head;
    while(onode!=nullptr){
        Node* node=new Node;
        if(tail==nullptr){
        tail=head=node;
        }
        else{
            tail->next=node;
            node->prev=tail;
            tail=node;
        }
        node->value=new SDS(*(onode->value));
        onode=onode->next;
        len++;
    }
}
void List::addr(SDS& value){
    Node* node=new Node;
    if(tail==nullptr){
        tail=head=node;
    }
    else{
        tail->next=node;
        node->prev=tail;
        tail=node;
    }
    node->value=new SDS(value);
    len++;
}

void List::addl(SDS& value){
    Node* node=new Node;
    if(head==nullptr){
        tail=head=node;
    }
    else{
        head->prev=node;
        node->next=head;
        head=node;
    }
    node->value=new SDS(value);
    len++;
}

SDS* List::lindex(int index){
    if(index>=len||index<0)
        return nullptr;
    Node* current=head;
    for(int i=0;i<index;i++){
        if(current->next!=nullptr){
            current=current->next;
        }
    }
    return current->value;
}