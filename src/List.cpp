#include"List.h"

List::List(){
    head=nullptr;
    tail=nullptr;
    len=0;
}

List::~List(){
    Node* current=head;
    while(current!=nullptr){
       // std::cout<<current<<"  "<<current->next<<std::endl;
        Node* next=current->next;
        delete current->value;
        delete current;
        current =next;
        
    }
}

List::List(List& list){
    head=nullptr;
    tail=nullptr;
    len=0;
    Node* onode=list.head;
    while(onode!=nullptr){
        this->addr(*onode->value);
    //    onode->value->print();
        onode=onode->next;
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

List::Node* List::lindex(int index){
    if(index>=len||index<0)
        return nullptr;
    Node* current=head;
    for(int i=0;i<index;i++){
        if(current->next!=nullptr){
            current=current->next;
        }
    }
    return current;
}

void List::popr(){
    if(len==0)
        return;
    tail=tail->prev;
    delete tail->next;
    tail->next=nullptr;
   // this->print();
    //printf("pop finished\n");
}

void List::popl(){
    if(len==0)
        return;
    head=head->next;
    delete head->prev;
    head->prev=nullptr;
    //this->print();
}

void List::print(){
    printf("List:\n");
    Node* node=head;
    while(node!=nullptr){
        node->value->print();
        node=node->next;
    }
}